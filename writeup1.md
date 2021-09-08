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
Nous avons utilisez le `web scanner` `Arachni` . Une alternative plus light pour Arachni est le package `dirb` pour linux.


Nous avons travailler avec l'os `Kali` sur `wsl` a partir de maintenant car celui-ci inclut la plupart des outils de `Pen Testing` dont nous aurons besoin par la suite .

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

Maintenant que l'on a un accès root à la db SQL, on cherche a faire une exploit appeler `web shell`. Grâce a `dirb` on peut obtenir une liste des dossiers utilisé par le site:
`
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