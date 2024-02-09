CC = gcc
FLAGS = -std=gnu11 -Werror -Wall -Wextra

single: singleEntryTest.c kvm_interface.h kvm_interface.c
	$(CC) $(FLAGS) $^ -o $@

# multi: multiThreadedTest.c
# 	$(CC) $(FLAGS) $^ -o $@

clean: 
	rm single multi