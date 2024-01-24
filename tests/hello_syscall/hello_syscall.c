#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include "hello_syscall.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoffer / Ludvig");
MODULE_DESCRIPTION("Syscall for printing Hello World to standard output\n");
MODULE_VERSION("0.1");

char* hello_world(void)
{
    char* string = kmalloc(255, GFP_KERNEL);

    if(!string) {
        printk(KERN_ERR "Failed to alloc memory\n");
        return "FAILURE TO ALLOC\n";
    }

    printk(KERN_INFO "Allocated memory successfully\n");
    
    string = "Hello World";
    
    printk(KERN_INFO "String content: %s \n", string);
    
    kfree(string);

    printk(KERN_INFO "Freed string!\n");

    return "Return Hello World!\n";
}

int __init start(void)
{
    printk(KERN_INFO "Starting module\n");
    return 0;
}

void __exit end(void)
{
    printk(KERN_INFO "Exiting  module\n");
}

EXPORT_SYMBOL(hello_world);
module_init(start);
module_exit(end);