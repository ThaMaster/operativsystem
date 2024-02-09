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
    struct KeyValuePair *kvp;
    int status;

    kvp = kvs_lookup(key);
    if(kvp != NULL) {
        printf("Found value: %s\n", (char *)kvp->value);
    } else {
        printf("No value found!\n");
    }
    printf("%s\n", key);

    // Insert
    char *value = calloc(4, sizeof(char));
    strcpy(value, "Hej");

    status = kvs_insert(key, value);
    printf("Insert returned: %d\n", status);

    
    // Lookup

    kvp = kvs_lookup(key);
    if(kvp != NULL) {
        printf("Found value: %s\n", (char *)kvp->value);
    } else {
        printf("No value found!\n");
    }

    
    // Remove
    char *rkey = calloc(2, sizeof(char));
    strcpy(rkey, "1");
    status = kvs_remove(rkey);
    printf("Remove returned: %d\n", status);
    
    
    // Lookup
    key = calloc(2, sizeof(char));
    strcpy(key, "1");

    kvp = kvs_lookup(key);
    if(kvp != NULL) {
        printf("Found value: %s\n", (char *)kvp->value);
    } else {
        printf("No value found!\n");
    }
    
    return 0;
}