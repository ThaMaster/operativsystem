#ifndef KVM_IOCTL_H
#define KVM_IOCTL_H

#include "kvm.h"
// TODO REMOVE KVP AND USE PURELY VALUE AND KEY.
struct InputOutput
{
    char *key;
    void *value;
    int key_size;
    int value_size;
};

#endif