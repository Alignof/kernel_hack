#include <stdio.h>
#include "call_mysyscall.h"

int main(void) {
        printf("my_syscall(10) -> %ld\n", my_syscall(10));
        return 0;
}
