#include<stdio.h>
#include<unistd.h>
#include<sys/syscall.h>

int main(void) {
    scanf("%d");
    return syscall(84, "test_dir");
}
