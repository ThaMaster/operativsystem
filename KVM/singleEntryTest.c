// gcc -std=c99 -Wall -o main.o main.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include "kvm.h"
#include "kvm_ioctl.h"

int main(void)
{
    int fd;
    printf("\nOpening Driver\n");
    fd = open("/dev/kvm", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }

    printf("Inserting KeyValuePair.\n");
    struct InputOutput insertIO;
    struct KeyValuePair insertPair;
    insertPair.key = "cringe";
    insertPair.value = calloc(7, sizeof(char));
    insertPair.value = "hejsan";
    insertIO.kvp = &insertPair;

    ioctl(fd, INSERT, (struct InputOutput *)&insertIO);
    
    printf("Looking up entry.\n");

    struct InputOutput lookingIO;
    struct KeyValuePair lookPair;
    lookPair.key = "cringe";
    lookPair.value = calloc(7, sizeof(char));
    lookingIO.kvp = &lookPair;

    ioctl(fd, LOOKUP, (struct InputOutput *)&lookingIO);
    if(lookingIO.status >= 0)
        printf("%s\n", (char *)lookingIO.kvp->value);

    struct InputOutput removeIO;
    struct KeyValuePair removePair;
    removePair.key = "cringe";
    removePair.value = calloc(7, sizeof(char));
    removeIO.kvp = &removePair;

    ioctl(fd, REMOVE, (struct InputOutput *)&removeIO);

    if(removeIO.status >= 0)
        printf("Removing element: %s\n", (char *)removeIO.kvp->value);

    printf("Closing Driver\n");
    close(fd);
}