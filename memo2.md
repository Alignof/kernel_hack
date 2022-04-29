```.s
.intel_syntax noprefix
.global main

.LC1:
    .string "target_dir"

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
$ gdb
>>> file vmlinux
>>> target remote localhost:8864
>>> lx-symbols
>>> b __x64_sys_rmdir (see https://elixir.bootlin.com/linux/v5.17.1/source/arch/x86/entry/syscalls/syscall_64.tbl)
>>> c
>>> b do_rmdir (see https://elixir.bootlin.com/linux/v5.17.1/source/fs/namei.c#L4037)
```

