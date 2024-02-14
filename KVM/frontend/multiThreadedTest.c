// gcc -o main_par.o -std=c99 -Wall -fopenmp main_par.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "kvm_interface.h"

int main(void)
{
    int fd;
    // printf("\nOpening Driver\n");
    fd = open("/dev/kvm", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }
    
    #pragma omp parallel for
    for (int i = 0; i < 10; i++) {
        char *key = calloc(1, sizeof(char));
        sprintf(key, "%d", i);
        char *value = calloc(4, sizeof(char));
        strcpy(value, "Hej");
        int value_size = strlen(value)*sizeof(char);

        if (kvs_insert(key, value, value_size) != 0) {
            printf("[%d] Could not instert value!\n", i);
        } else {
            printf("[%d] Insterted value!\n", i);
        }
        
        free(key);
        free(value);
    }

    #pragma omp parallel for
    for (int i = 0; i < 10; i++) {
        char *key = calloc(1, sizeof(char));
        sprintf(key, "%d", 0);

        if (kvs_remove(key) != 0) {
            printf("[%d] Could not remove value!\n", i);
        } else {
            printf("[%d] Removed value!\n", i);
        }
        
        free(key);
    }

    close(fd);
}