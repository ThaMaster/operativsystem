// gcc -o -main_par.o -std=c99 -Wall -fopenmp main_par.c
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
    for (int i = 0; i < 30; i++) {
        // printf("Inserting KeyValuePair.\n");
        struct InputOutput insertIO;
        struct KeyValuePair insertPair;
        insertPair.key = calloc(1, sizeof(char));
        insertPair.key = (char *)&i;
        // insertPair.key = "cringe%d", i;
        insertPair.value = calloc(7, sizeof(char));
        insertPair.value = "hejsan";
        insertIO.kvp = &insertPair;

        ioctl(fd, INSERT, (struct InputOutput *)&insertIO);
        printf("(%d) Returned: %d.\n", i, insertIO.status);
    }

    
    close(fd);
}