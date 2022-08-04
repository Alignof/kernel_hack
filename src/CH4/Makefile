obj-m := my_chdev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm a.out

load:
	sudo insmod my_chdev.ko
	sudo mknod --mode=666 /dev/mydevice0 c `grep MyDevice /proc/devices | awk '{print $$1;}'` 0
	sudo mknod --mode=666 /dev/mydevice1 c `grep MyDevice /proc/devices | awk '{print $$1;}'` 1
	sudo mknod --mode=666 /dev/mydevice2 c `grep MyDevice /proc/devices | awk '{print $$1;}'` 2

unload:
	sudo rmmod my_chdev.ko
	sudo rm /dev/mydevice0
	sudo rm /dev/mydevice1
	sudo rm /dev/mydevice2


.PHONY: load remove
