#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "kvm_ioctl.h"

int open_driver_fd(char *file);

int kvs_insert(char *key, void *value) 
{
    int status;

    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return -1;
    }
    
    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    struct KeyValuePair *kvp = calloc(1, sizeof(struct KeyValuePair));
    kvp->key = (char *)key;
    kvp->value = value;
    IO->kvp = kvp;
    
    ioctl(fd, INSERT, (struct InputOutput *)IO);
    status = IO->status;

    close(fd);
    free(IO);

    return status;
}

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
    // TODO: MAKE CLEAN!
    // printf("1\n");
    // free(IO->kvp->value);
    // printf("2\n");
    // free(IO->kvp->key);
    // printf("3\n");
    // free(IO->kvp);
    // printf("4\n");
    // free(IO);
    // printf("5\n");

    return 0;
}

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
    free(IO);
    
    return kvp;
}

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