// gcc -std=c99 -Wall -o singleEntryTest.o singleEntryTest.c
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "kvm_interface.h"


int main(void)
{
    char *key = calloc(1, sizeof(char));
    key = "1";
    char *value = calloc(4, sizeof(char));
    value = "Hej";
    struct KeyValuePair *kvp;
    int status;

    kvp = kvs_lookup(key);
    if(kvp != NULL) {
        printf("Found value: %s\n", (char *)kvp->value);
    } else {
        printf("No value found!\n");
    }

    status = kvs_insert(key, value);
    printf("Insert returned: %d\n", status);
    
    kvp = kvs_lookup(key);
    if(kvp != NULL) {
        printf("Found value: %s\n", (char *)kvp->value);
    } else {
        printf("No value found!\n");
    }
    
    char *rkey = calloc(1, sizeof(char));
    rkey = "1";
    status = kvs_remove(rkey);
    printf("Remove returned: %d\n", status);
    
    kvp = kvs_lookup(key);
    if(kvp != NULL) {
        printf("Found value: %s\n", (char *)kvp->value);
    } else {
        printf("No value found!\n");
    }
    return 0;
}