# config
host
```.sh
$ cd kernel_hack/linux-5.17.1/
$ make defconfig
$ make menuconfig
$ vim .config (CONFIG_RANDOMIZE_BASE=n)
$ ./scripts/config --disable SYSTEM_TRUSTED_KEYS
$ ./scripts/config --disable SYSTEM_REVOCATION_KEYS
$ ./scripts/config --disable MODULE_SIG_KEY
$ make -j8 bindeb-pkg  KDEB_PKGVERSION=5.17.1-DisableRandomizeBase
```

guest
```.sh
$ cd kernel_hack/
$ dpkg --install *.deb
$ uname -a
Linux ubuntu 5.17.1 #2 SMP PREEMPT Thu Apr 21 19:27:13 JST 2022 x86_64 x86_64 x86_64 GNU/Linux
```
