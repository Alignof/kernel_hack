#include <linux/syscall.h>

SYSCALL_DEFINE1(mysyscall, int, x) {
    printk("hello world!");
    return x;
}
