obj-m := kthread_test.o

all:
	make INCDIR=/usr/include -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

load:
	sudo insmod kthread_test.ko
	#sudo mknod --mode=666 /dev/kthread_test c `grep kthread_test /proc/devices | awk '{print $$1;}'` 0

unload:
	sudo rmmod kthread_test.ko
	#sudo rm /dev/kthread_test


.PHONY: load remove
