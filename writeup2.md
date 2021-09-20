# WRITEUP2

When we saw that the vm was booting in linux system, we search a way to modify the booting, but we don't have `GRUB`, the vm is working on syslinux.

After some research, we found a way to find boot users with a cfg file 
[syslinux](https://wiki.syslinux.org/wiki/index.php?title=Config)

```bash
> find / -type f -name *.cfg | grep -v denied > file
> cat file

/boot/grub/grub.cfg
/etc/dpkg/dpkg.cfg
/etc/perl/Net/libnet.cfg
/usr/share/doc/grub-common/examples/grub.cfg
/usr/share/squirrelmail/locale/timezones.cfg
/usr/share/update-manager/DistUpgrade.cfg
/usr/share/update-manager/additional_pkgs.cfg
/usr/share/update-manager/demoted.cfg
/usr/share/update-manager/mirrors.cfg
/usr/share/update-manager/removal_blacklist.cfg
/cdrom/isolinux/isolinux.cfg
/rofs/boot/grub/grub.cfg
/rofs/etc/dpkg/dpkg.cfg
/rofs/etc/perl/Net/libnet.cfg
/rofs/usr/share/doc/grub-common/examples/grub.cfg
/rofs/usr/share/squirrelmail/locale/timezones.cfg
/rofs/usr/share/update-manager/DistUpgrade.cfg
/rofs/usr/share/update-manager/additional_pkgs.cfg
/rofs/usr/share/update-manager/demoted.cfg
/rofs/usr/share/update-manager/mirrors.cfg
/rofs/usr/share/update-manager/removal_blacklist.cfg
```
we find isolinux.cfg : 

```bash
default live
prompt 0
timeout 0

menu title BornToSec
menu background splash.png
menu color title 1;37;44 #c0ffffff #00000000 std

label live
  menu label live - boot the Live System
  kernel /casper/vmlinuz
  append  file=/cdrom/preseed/custom.seed boot=casper initrd=/casper/initrd.gz quiet splash --
```

We can see that the default user is live, so we can use a tip found on stack overflow

[booting into a kernel image](https://unix.stackexchange.com/questions/151969/booting-into-a-kernel-image)

We found the way to launch the boot manager from syslinux, hold shift or alt when the vm is starting then we use `init=/bin/sh` to force the kernel to launch with live and execute the command:

```bash
boot: live init=/bin/sh
```


