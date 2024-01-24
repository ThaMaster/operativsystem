#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoffer Nordlander");
MODULE_DESCRIPTION("A super basic hello world example");
MODULE_VERSION("0.1");

int init_module(void)
{
    printk(KERN_INFO "Hello world 1. \n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Goodbye world 1.\n");
}