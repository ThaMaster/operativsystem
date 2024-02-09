// gcc -std=c99 -Wall -o kvm_interface.o kvm_interface.c kvm_interface.h
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
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
int kvs_insert(char *key, void *value) 
{
    int status;

    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return -1;
    }
    
    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    struct KeyValuePair *kvp = calloc(1, sizeof(struct KeyValuePair));
    kvp->key = key;
    kvp->value = value;
    IO->kvp = kvp;
    
    ioctl(fd, INSERT, (struct InputOutput *)IO);
    status = IO->status;

    close(fd);
    free(IO);

    return status;
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

    ioctl(fd, REMOVE, (struct InputOutput *)IO);
    if (IO->status < 0) {
        close(fd);
        free(IO);
        return -1;
    }

    close(fd);

    free(IO->kvp->key);
    free(IO->kvp->value);
    free(IO->kvp);
    free(key);
    free(IO);

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
 * returns: struct KeyValuePair * on success
 *          NULL otherwise
 */
struct KeyValuePair *kvs_lookup(char *key) 
{
    struct KeyValuePair *kvp = NULL;
    
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return NULL;
    }

    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    IO->key = key;

    ioctl(fd, LOOKUP, (struct InputOutput *)IO);
    kvp = IO->kvp;
    
    close(fd);
    free(key);
    free(IO);
    
    return kvp;
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