#include "../kvm_ioctl.h"

int kvs_insert(char *key, void *value, int value_size); 
int kvs_remove(char *key); 
struct KeyValuePair *kvs_lookup(char *key);