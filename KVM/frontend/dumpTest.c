// gcc -std=c99 -Wall -o singleEntryTest.o singleEntryTest.c
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kvm_interface.h"

int main(void)
{    
    init_module();
    char *key = calloc(2, sizeof(char));
    strcpy(key, "1");
    int status;

    // Insert1
    char *value = calloc(4, sizeof(char));
    strcpy(value, "hej");
    int value_size = strlen(value)*sizeof(char);
    status = kvs_insert(key, value, value_size);
    printf("Insert returned: %d\n", status);

    // Insert2
    strcpy(key, "2");
    strcpy(value, "jag");
    value_size = strlen(value)*sizeof(char);
    status = kvs_insert(key, value, value_size);
    printf("Insert returned: %d\n", status); 

    // Insert3
    strcpy(key, "3");
    value = calloc(6, sizeof(char));
    strcpy(value, "skits");
    value_size = strlen(value)*sizeof(char);
    status = kvs_insert(key, value, value_size);
    printf("Insert returned: %d\n", status);

    // status = kvs_dump();
    // printf("Dump returned: %d\n", status);
    
    // while(dumped_entries)
    // {
    //     printf("Dump returned w value: %s, key: %s\n", (char*)dumped_entries->kvp->value, dumped_entries->kvp->key);
    //     dumped_entries = dumped_entries->next;
    // }

    
    return 0;
}