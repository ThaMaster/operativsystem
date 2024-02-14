// gcc -std=c99 -Wall -o singleEntryTest.o singleEntryTest.c
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kvm_interface.h"


int main(void)
{
    char *key = calloc(2, sizeof(char));
    strcpy(key, "1");
    int status;

    // Insert
    char *value = calloc(4, sizeof(char));
    strcpy(value, "put");
    int value_size = strlen(value)*sizeof(char);

    status = kvs_insert(key, value, value_size);
    printf("Insert returned: %d\n", status);
    
    // Lookup
    char *lkey = calloc(2, sizeof(char));
    strcpy(lkey, "1");
    struct InputOutput *IO = calloc(1, sizeof(struct InputOutput));
    IO = kvs_lookup(lkey);
    printf("Lookup returned: %d\n", IO == NULL ? -1 : 0);

    // // Remove
    // char *rkey = calloc(2, sizeof(char));
    // strcpy(rkey, "1");
    // status = kvs_remove(rkey);
    // printf("Remove returned: %d\n", status);
    
    
    // // Lookup 2
    // char *lkey2 = calloc(2, sizeof(char));
    // strcpy(lkey2, "1");
    // IO = kvs_lookup(lkey2);
    // printf("Lookup returned: %d\n", IO->status);
    
    
    return 0;
}