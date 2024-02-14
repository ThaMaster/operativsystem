#include "../kvm_ioctl.h"

struct Dump {
    struct InputOutput* kvp;
    struct Dump* next;
};

int kvs_insert(char *key, void *value, int value_size); 
int kvs_remove(char *key); 
struct Dump *kvs_dump(); 
struct InputOutput *kvs_lookup(char *key);