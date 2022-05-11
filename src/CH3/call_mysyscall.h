#ifndef __LINUX_NEW_SYSCALL_H
#define __LINUX_NEW_SYSCALL_H
#include "unistd_64.h"
#include <unistd.h>
#include <asm/unistd.h>
#define my_syscall(x) syscall(__NR_my_syscall, x)
#endif
