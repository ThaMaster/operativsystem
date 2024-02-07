// gcc -o main_par.o -std=c99 -Wall -fopenmp main_par.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <omp.h>
#include "kvm.h"
#include "kvm_ioctl.h"
#include <string.h>

struct InputOutput *createIO(char* key, int kSize, void* value, int vSize);

int main(void)
{
    int fd;
    printf("\nOpening Driver\n");
    fd = open("/dev/kvm", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }

    #pragma omp parallel for
    for (int i = 0; i < 3; i++) {
        char *n = calloc(1, sizeof(char));
        n = "1";
        struct InputOutput *insertIO = createIO(n, 1, "eat my shorts", strlen("eat my shorts"));

        printf("Inserting key: \"%s\"\n", insertIO->kvp->key);
        ioctl(fd, INSERT, (struct InputOutput *)insertIO);
        printf("(%d) Insert returned: %d.\n", i, insertIO->status);

        char *l = "1";
        struct InputOutput *lookIO = createIO(l, 1, "bruh", strlen("eat my shorts"));
        ioctl(fd, LOOKUP, (struct InputOutput *)lookIO);
        printf("(%d) Lookup returned: %d.\n", i, lookIO->status);
        if(lookIO->status >= 0)
        {
            printf("Found value!\n");
            printf("Value:  %s.\n", (char *)lookIO->kvp->value);
        } else {
            printf("No value!\n");
        }
    }

    #pragma omp parallel for 
    for (int i = 0; i < 3; i++) {
        char *l = "1";
        char *r = "1";
        struct InputOutput *removeIO = createIO(r, 1, "eat my shorts", strlen("eat my shorts"));
        ioctl(fd, REMOVE, (struct InputOutput *)removeIO);
        printf("(%d) Remove returned: %d.\n", i, removeIO->status);

        struct InputOutput *lookIO2 = createIO(l, 1, "bruh", strlen("eat my shorts"));
        ioctl(fd, LOOKUP, (struct InputOutput *)lookIO2);
        printf("(%d) Lookup returned: %d.\n", i, lookIO2->status);
        if(lookIO2->status >= 0)
        {
            printf("Found value!\n");
            printf("Found value:  %s.\n", (char *)lookIO2->kvp->value);
        } else {
            printf("No value!\n");
        }

    }
    close(fd);
}

struct InputOutput *createIO(char* key, int kSize, void* value, int vSize)
{
    struct InputOutput *insertIO = calloc(1, sizeof(struct InputOutput));
    struct KeyValuePair *insertPair = calloc(1, sizeof(struct KeyValuePair));
    insertPair->key = calloc(kSize, sizeof(char));
    insertPair->key = (char *)key;

    insertPair->value = calloc(vSize, sizeof(char));
    insertPair->value = value;
    insertIO->kvp = insertPair;

    return insertIO;
}