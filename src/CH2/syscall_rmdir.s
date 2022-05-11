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
