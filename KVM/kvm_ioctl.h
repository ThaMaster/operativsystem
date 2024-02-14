#ifndef KVM_IOCTL_H
#define KVM_IOCTL_H

#include "kvm.h"

struct InputOutput
{
    // Add rw_lock to each bucket
    char *key;
    void *value;
    int key_size;
    int value_size;
};

#endif