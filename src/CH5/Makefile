obj-m := my_procfs.o
EXTRA_CFLAGS := -I/home/tsukubataro/kernel_hack/linux-5.17.1/arch

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm a.out

load:
	sudo insmod my_procfs.ko

unload:
	sudo rmmod my_procfs.ko


.PHONY: load remove
