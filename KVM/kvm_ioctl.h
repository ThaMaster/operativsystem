#ifndef KVM_IOCTL_H
#define KVM_IOCTL_H

#include "kvm.h"

struct InputOutput
{
    struct KeyValuePair *kvp;
    int status;
    char *key;
};

#endif