#ifndef KVM_IOCTL_H
#define KVM_IOCTL_H

#include "kvm.h"
// TODO REMOVE KVP AND USE PURELY VALUE AND KEY.
struct InputOutput
{
    struct KeyValuePair *kvp;
    int status;
};

#endif