#ifndef KVM_IOCTL_H
#define KVM_IOCTL_H

struct InputOutput
{
    struct KeyValuePair *kvp;
    int status;
};

#endif