# tutorial
## host
```.sh
$ cd ~/kernel_hack/linux-5.17.1/arch/x86/kernel
$ cp ~/kernel_hack/myrepo/src/my_syscall.c  .
$ vim ~/kernel_hack/linux-5.17.1/arch/x86/kernel/Makefile (add "obj-y           += my_syscall.o" to line 58)
$ vim ../../../include/linux/syscalls.h (add "asmlinkage long sys_new_syscall( int x );" to line 999)
$ vim ../../../arch/x86/entry/syscalls/syscall_64.tbl (add "335 common  my_syscall          sys_my_syscall")
$ cd ~/kernel_hack/linux-5.17.1/
$ make -j8 bindeb-pkg  KDEB_PKGVERSION=5.17.1-AddMySyscall
```
## guest
```.sh
$ cd kernel_hack/
$ sudo dpkg -r linux-image-5.17.1
$ sudo dpkg -r linux-image-5.17.1-dbg
$ sudo dpkg --install *AddMySyscall*.deb
$ uname -a
Linux ubuntu 5.17.1 #AddMySyscall SMP PREEMPT Sat Apr 30 02:00:12 JST 2022 x86_64 x86_64 x86_64 GNU/Linux
$ cd ~/tmp
$ cp ~/kernel_hack/linux-5.17.1/arch/x86/include/generated/uapi/asm/unistd_64.h .
$ cp ~/kernel_hack/my_repo/src/call_mysyscall* .
$ gcc gcc call_mysyscall.c -o CallMySyscall
$ ./CallMySyscall 
my_syscall(10) -> 10
$ dmesg | tail -n 1
[  245.026362] hello world!
```
# create my syscall
## host
```.sh
$ cd ~/kernel_hack/linux-5.17.1/arch/x86/kernel
$ cp ~/kernel_hack/myrepo/src/CH3/my_syscall.c  ./median.c
$ vim ~/kernel_hack/linux-5.17.1/arch/x86/kernel/Makefile (add "obj-y           += median.o" to line 59)
$ vim ../../../include/linux/syscalls.h (add "asmlinkage long sys_array_median( int __user *array, const int size, float __user *result);" to line 1000)
$ vim ../../../arch/x86/entry/syscalls/syscall_64.tbl (add "336	common	array_median			sys_array_median")
$ cd ~/kernel_hack/linux-5.17.1/
$ make -j8 bindeb-pkg  KDEB_PKGVERSION=5.17.1-AddMedian
```
## guest 
```.sh
$ cd kernel_hack/
$ sudo dpkg -r linux-image-5.17.1
$ sudo dpkg -r linux-image-5.17.1-dbg
$ sudo dpkg --install *AddMedian*.deb
$ uname -a
Linux ubuntu 5.17.1 #AddMedian SMP PREEMPT Sat May 28 00:35:01 JST 2022 x86_64 x86_64 x86_64 GNU/Linux
$ cd ~/tmp/CH3
$ cp ~/kernel_hack/linux-5.17.1/arch/x86/include/generated/uapi/asm/unistd_64.h .
$ cp ~/kernel_hack/my_repo/src/CH3/call_median* .
$ gcc call_median.c -o CallMedian
$ ./CallMedian
```







memo: f73d4540-be45-49c2-be75-570045f01eb2
