CC = gcc
FLAGS = -std=gnu11 -Wall -Wextra

all: single multi dumper

single: singleEntryTest.c kvm_interface.h kvm_interface.c
	$(CC) $(FLAGS) $^ -o $@

multi: multiThreadedTest.c kvm_interface.h kvm_interface.c
	$(CC) $(FLAGS) $^ -fopenmp -o $@

dumper: dumpTest.c kvm_interface.h kvm_interface.c
	$(CC) $(FLAGS) $^ -o $@

clean: 
	rm single multi dumper