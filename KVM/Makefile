obj-m += kvm.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -std=c99 -Wall -o main.o main.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean