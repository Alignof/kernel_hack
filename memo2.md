```.s
.intel_syntax noprefix
.global main

.LC1:
    .string "test_dir"

main:
    push rbp
    mov rbp, rsp

    mov rax, 84 # rmdir
    lea rdi, .LC1[rip]

    syscall

    mov rsp, rbp
    pop rbp
    ret
```

```.zsh
$ peda
gdb-peda$ file vmlinux
gdb-peda$ target remote localhost:8864
gdb-peda$ lx-symbols
```
