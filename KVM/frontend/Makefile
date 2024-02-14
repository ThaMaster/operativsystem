CC = gcc
FLAGS = -std=gnu11 -Wall -Wextra

all: single multi

single: singleEntryTest.c kvm_interface.h kvm_interface.c
	$(CC) $(FLAGS) $^ -o $@

multi: multiThreadedTest.c kvm_interface.h kvm_interface.c
	$(CC) $(FLAGS) $^ -fopenmp -o $@

clean: 
	rm single multi