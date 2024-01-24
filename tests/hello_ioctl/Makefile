obj-m += hello_ioctl.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -std=c99 -Wall -o main.o main.c
	sudo insmod hello_ioctl.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod hello_ioctl.ko