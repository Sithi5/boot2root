# boot2root
This project aims to make you discover, via several small challenges, security
in computer science in several fields.

![hacking pic](./resources/fkthesystem.png)

# Usefull commands
```bash
arp -a # Affiche les entrées concernant l'hôte spécifié.
nmap -F 192.168.1.1 # Scan 100 most common ports (Fast)
nmap -p- 192.168.1.1 # Scan all 65535 ports
sed 's/\xc2\xa0/ /g' {file} # Remove m-bm- format from windows text file.
```

gdb commands :
```bash
disas {label}# disassemble the givent label/function
b *{address}# Put a breakpoint at the given address
commands {breakpoint number} # Allow to insert commands to execute at the given breakpoint.
```
# Resources
### Penetration testing
###### Os
* [Kali](https://www.kali.org/)
* [Kali tools-listing](https://tools.kali.org/tools-listing)

###### Web Scanners
* [Web scanners lists](https://github.com/bkapsec/fuzzing#web-scanners)
* [Install Arachni](https://www.linuxhelp.com/how-to-install-arachni-web-application-scanner-in-ubuntu-2)

### Backdoor
* [backdoor website](https://www.malcare.com/blog/backdoor-website/)
* [backdoor using mysql injection](https://cloudinvent.com/blog/backdoor-webserver-using-mysql-sql-injection/)

### nmap
* [Nmap Cheat Sheet](https://hackertarget.com/nmap-cheatsheet-a-quick-reference-guide/)

### SQL Injection
* [Use SQL Injection to Run OS Commands & Get a Shell](https://null-byte.wonderhowto.com/how-to/use-sql-injection-run-os-commands-get-shell-0191405/)

### Others
* [Reset the Root Password](https://www.maketecheasier.com/reset-root-password-linux/)
