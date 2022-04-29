```.sh
$ cd ~/kernel_hack/linux-5.17.1/arch/x86/kernel
$ cp ~/kernel_hack/myrepo/src/my_syscall.c  .
$ vim ~/kernel_hack/linux-5.17.1/arch/x86/kernel/Makefile (add "obj-y           += my_syscall.o" to line 58)
$ vim ../../../include/linux/syscalls.h (add "asmlinkage long sys_new_syscall( int x );" to line 999)
$ vim ../../../arch/x86/entry/syscalls/syscall_64.tbl (add "335 common  my_syscall          sys_my_syscall")
$ cd ~/kernel_hack/linux-5.17.1/
$ make -j8 bindeb-pkg  KDEB_PKGVERSION=5.17.1-AddMySyscall
```
