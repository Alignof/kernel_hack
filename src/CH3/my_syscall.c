#include <linux/syscall.h>

// https://elixir.bootlin.com/linux/v5.17.1/source/tools/include/nolibc/nolibc.h#L210
#ifndef AT_FDCWD
#define AT_FDCWD    -100
#endif

// https://elixir.bootlin.com/linux/v5.17.1/source/fs/namei.c#L4097
SYSCALL_DEFINE2(swapdir, const char __user *, dir1, const char __user *, dir2) {
    return do_swapdir(AT_FDCWD, dir1, dir2);
}

int do_swapdir(int dfd, struct filename *dir1, struct filename *dir2) {
    printk("did swapdir");
    return 42;
}
