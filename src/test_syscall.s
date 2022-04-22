.intel_syntax noprefix
.global main

.LC1:
    .string "./test_dir\n"

main:
    push rbp
    mov rbp, rsp

    mov rax, 40 # rmdir
    mov rdi, OFFSET FLAT:.LC1

    syscall

    mov rsp, rbp
    pop rbp
    ret
