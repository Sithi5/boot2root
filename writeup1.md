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
Plusieurs adresses répondes, on peut les essayer à l'aide d'un navigateur.
L'adresse `192.168.1.22` nous affiche une page avec ecrit `HACK ME`. Après différents tests et quelques recherche on trouve différents moyen de découvrir des fails et d'autres pages accessibles.
Nous avons utilisez le `web scanner` `Arachni` . Une alternative plus light pour Arachni est le package `dirb` pour linux.