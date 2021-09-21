# BONUS3 :

After a lot of investigation and a lot of fail routes, we tried to open the iso directly to explore the file inside it (with `winrar` on `windows 10`). We came to a file `filesystem.squashfs`, after some research it seems that this file is a compressed read-only Filesystem of all the file of the vm. From here we can `unsquash` it with different tools (`unsquashfs` with `wsl ubuntu` for exemple). From here we can access all the file of the VM and get directly access of all the home of users (\home\*).

We can directly find the  `README` of `thor` user and the `turle` file. From there we can extract the turtle file and follow the step of the `writeup1`.