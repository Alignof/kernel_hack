obj-m := my_procfs.o

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
