// gcc -std=c99 -Wall -o kvm_interface.o kvm_interface.c kvm_interface.h
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include "../kvm_ioctl.h"
#include "kvm_interface.h"

int init_module();
int open_driver_fd(char *file);
int clear_file(char *file);
int write_dump_to_file(struct Dump *dumped_values);
void run_persistance(void);
void main_process(pid_t p, int fp);

/**
 * Functoin: init_module()
 * -------------------------
 * Initializes the module and gives the process ids to
 * the persistance storage daemon if it currently does
 * not have one.
 *
 * returns: 0 on success
 *          -1 on error
 */
int init_module()
{
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return -1;
    }
    pid_t p;
    pid_t mypid = getpid();
    printf("main pid: %d\n", mypid);
    int status = ioctl(fd, HASSPID, 0);
    if(status == 0) {
        p = fork();
        if(p < 0) {
            perror("ERROR: Failed to fork a new process...\n");
            close(fd);
            return -1;
        } else if(p == 0) {
            close(fd);
            run_persistance();
        } else {
            main_process(p, fd);
            close(fd);
        }
    } else if(status < 0) {
        perror("ERROR: Failed to get persistance daemon status from module...\n");
        return -1;
    }
    return 0;
}

/**
 * Function: main_process
 * -------------------------
 * Function in which the main process should run
 * when the module initializes the module daemons.
 */
void main_process(pid_t p, int fd)
{
    int status = ioctl(fd, SETSPID, &p);
    printf("Status: %d\n", status);
    if(status < 0) {
        perror("ERROR: Failed to set the pid of persitance process to kvm module");
    }
}


void handle_signal() {
    FILE *file = fopen("/dev/kmsg", "a");
    char *buff = calloc(1024, sizeof(char));
    sprintf(buff, "Signal caught!\n");
    fputs(buff, file);
    int status = kvs_dump();

    sprintf(buff, "Dumper returned: %d\n", status);
    fputs(buff, file);



    free(buff);
    fclose(file);
}

/**
 * Function: run_persistance
 * Function that should be run by the child
 * process to initializes 
 */
void run_persistance()
{
    // Setup signal handling
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_signal;
    sigaction(SIGIO, &action, NULL);

    while(1)
        sleep(1);
}

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
 * returns: 0 on success
 *          -1 otherwise
 */
int kvs_dump()
{
    int fd = open_driver_fd("/dev/kvm");
    if (fd == -1) {
        return -1;
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
        return -1;
    }

    close(fd);


    return write_dump_to_file(dumped_values);
}

int clear_file(char *file) {
    FILE *f = fopen(file, "w");
    if (f == NULL) {
        printf("Error: Could not create/clear file...\n");
        return -1;
    }
    fclose(f);
    return 0;
}

int write_dump_to_file(struct Dump *dumped_values)
{
    struct stat st = {0};
    if (stat("/dev/kvm_data", &st) == -1) {
        mkdir("/dev/kvm_data", 0700);
    }

    if (clear_file("/dev/kvm_data/data.csv") < 0) {
        return -1;
    }
    
    FILE *file = fopen("/dev/kvm_data/data.csv", "a");
    if (!file) {
        printf("Error: Cannot open perpetual storage file...\n");
        return -1;
    }

    struct Dump *current_dump = dumped_values;
    while (current_dump != NULL) {
        char *buff = calloc(2048, sizeof(char));
        sprintf(buff, "%s,%s\n", current_dump->kvp->key, (char *)current_dump->kvp->value);
        fputs(buff, file);
        free(buff);
        free(current_dump->kvp->key);
        free(current_dump->kvp->value);
        free(current_dump->kvp);
        struct Dump *dumpToFree = current_dump;
        current_dump = current_dump->next;
        free(dumpToFree);
    }

    return 0;
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