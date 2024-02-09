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
    strcpy(value, "Hej");
    int value_size = strlen(value)*sizeof(char);

    status = kvs_insert(key, value, value_size);
    printf("Insert returned: %d\n", status);
    
    // Lookup
    char *lkey = calloc(2, sizeof(char));
    strcpy(lkey, "1");
    printf("before lookup!\n");
    struct KeyValuePair *kvp = calloc(1, sizeof(struct KeyValuePair));
    kvp = kvs_lookup(lkey);
    printf("after lookup!\n");

    if (kvp != NULL) {
        // TODO: SEG FAULT WHEN TRYING TO CHECK NULLILLITY.
        // MAY BE BECAUSE OF KERNEL SPACE PERMISSOIN????????
        if (kvp->key != NULL) {
            printf("Key: %s\n", kvp->key);
            fflush(stdout);
            if (kvp->value != NULL) {
                
            }
        }
    }

    return 0;
}