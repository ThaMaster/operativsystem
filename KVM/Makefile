obj-m += kvm_module.o
kvm_module-objs := kvm_ioctl.o kvm.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod kvm_module.ko
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod kvm_module.ko