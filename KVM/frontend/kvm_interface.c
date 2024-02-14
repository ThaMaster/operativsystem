// gcc -std=c99 -Wall -o kvm_interface.o kvm_interface.c kvm_interface.h
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../kvm_ioctl.h"

int open_driver_fd(char *file);

/**
 * Function: kvs_insert
 * -------------------------
 * Inserts a new value to the key-value-store.
 * 
 * key:   The key of the entry to store.
 * value: The value of the entry to store.
 *
 * returns: 0 on success
 *          -1 on error
 */
int kvs_insert(char *key, void *value, int value_size) 
{
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return -1;
    }

    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    IO->key = key;
    IO->key_size = strlen(key);
    IO->value = value;
    IO->value_size = value_size;

    if (ioctl(fd, INSERT, IO) != 0) {
        close(fd);
        return -1;
    }

    // close(fd);
    // free(IO);
    // free(key);
    // free(value);

    return 0;
}

/**
 * Function: kvs_remove
 * -------------------------
 * Removes the entry in the key-value-store with 
 * the given key.
 * 
 * key: The key of the entry to remove.
 *
 * returns: 0 on success
 *          -1 on error
 */
int kvs_remove(char *key) 
{
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return -1;
    }
    
    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    IO->key = key;
    IO->key_size = strlen(key);

    if (ioctl(fd, REMOVE, IO) != 0){
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

/**
 * Function: kvs_lookup
 * -------------------------
 * Attempts to find the value in the key-value-store 
 * with the given key.
 * 
 * key: The key of the entry to remove.
 *
 * returns: struct InputOutput * on success
 *          NULL otherwise
 */
struct InputOutput *kvs_lookup(char *key) 
{
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return NULL;
    }

    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    IO->key = key;
    IO->key_size = strlen(key)*sizeof(char);
    IO->value = calloc(500, sizeof(char));
    
    if (ioctl(fd, LOOKUP, IO) != 0) {
        close(fd);
        return NULL;
    }

    // printf("\n\n\n");
    // printf("IO pointer: %p\n", IO);
    // fflush(stdout);
    // printf("key pointer: %p\n", IO->key);
    // fflush(stdout);
    // printf("key value: %s\n", IO->key);
    // fflush(stdout);
    // printf("value pointer: %p\n", IO->value);
    // fflush(stdout);
    // printf("value value: %s\n", (char *)IO->value);
    // printf("\n\n\n");


    close(fd);

    return IO;
}

/**
 * Function: open_driver_fd
 * -------------------------
 * Opens a file descriptor from the given file path.
 * 
 * file: The location of the driver file descriptor.
 *
 * returns: The file descriptor (fd) on success
 *          -1 on error
 */
int open_driver_fd(char *file) 
{
    int fd;
    printf("\nOpening Driver\n");
    fd = open(file, O_RDWR);
    if(fd < 0) {
        printf("Error: Cannot open device file...\n");
        return -1;
    }
    return fd;
}