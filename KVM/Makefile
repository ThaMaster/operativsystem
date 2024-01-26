obj-m += kvm_module.o
kvm_module-objs := kvm_ioctl.o kvm.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean