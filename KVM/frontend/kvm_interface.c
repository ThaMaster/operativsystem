// gcc -std=c99 -Wall -o kvm_interface.o kvm_interface.c kvm_interface.h
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../kvm_ioctl.h"
#include "kvm_interface.h"

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
        free(IO);
        close(fd);
        return -1;
    }

    close(fd);
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
        free(IO);
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
    IO->value = calloc(1, sizeof(void *));
    
    if (ioctl(fd, LOOKUP, IO) != 0) {
        free(IO->value);
        free(IO);
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
 * Function: kvs_dump
 * -------------------------
 * Attempts to extract all the data that is currently
 * loaded in the KVS.
 *
 * currently lodade *
 *
 * returns: struct Dump * on success
 *          NULL otherwise
 */
struct Dump *kvs_dump()
{
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return NULL;
    }

    struct Dump *dumped_values = NULL;
    struct Dump *curr_dump = NULL;
    struct Dump *new_dump = NULL;

    struct InputOutput* curr_kvp = calloc(1, sizeof(struct InputOutput));
    curr_kvp->key = calloc(1024, sizeof(char));
    curr_kvp->value = calloc(1024, sizeof(char));

    int status = ioctl(fd, DUMP, curr_kvp);
    while(status == 0) 
    {
        if (dumped_values == NULL) {
            dumped_values = calloc(1, sizeof(struct Dump));
            dumped_values->kvp = curr_kvp;
            curr_dump = dumped_values;
        } else {
            new_dump = calloc(1, sizeof(struct Dump));
            new_dump->kvp = curr_kvp;

            curr_dump->next = new_dump;
            curr_dump = curr_dump->next;
        }
        curr_kvp = calloc(1, sizeof(struct InputOutput));
        curr_kvp->key = calloc(1024, sizeof(char));
        curr_kvp->value = calloc(1024, sizeof(char));
        status = ioctl(fd, DUMP, curr_kvp);
    }
    if (status == -1) {
        free(dumped_values);
        close(fd);
        return NULL;
    }

    close(fd);
    return dumped_values;
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
    // printf("\nOpening Driver\n");
    fd = open(file, O_RDWR);
    if(fd < 0) {
        printf("Error: Cannot open device file...\n");
        return -1;
    }
    return fd;
}