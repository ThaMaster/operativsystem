#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/err.h>
#include "kvm.h"

dev_t dev = 0;
static struct class *kvm_class;
static struct cdev kvm_cdev;

static int __init kvm_init(void);
static void __exit kvm_exit(void);
static int kvm_open(struct inode *inode, struct file *file);
static int kvm_close(struct inode *inode, struct file *file);
static long kvm_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static uint8_t kvm_hash(const char key[]);

/**
 * Defines the operations that can be ran on the device file.
 */
static struct file_operations fops = 
{
    .owner          = THIS_MODULE,
    .open           = kvm_open,
    .unlocked_ioctl = kvm_ioctl,
    .release        = kvm_close,
};

static struct KeyValuePair **kvs;

/**
 * Function: kvm_hash
 * -------------------------
 * Hashes a key of type char list into an unsigned uint8_t.
 * 
 * key: Key to be hashed of type char list.
 * 
 * returns: uint8_t.
 */
static uint8_t kvm_hash(const char key[])
{
    uint8_t hash = 0;

    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash;
}

static int kvm_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "KVM opened.\n");
    return 0;
}

static int kvm_close(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "KVM closed.\n");
    return 0;
}

static long kvm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case INSERT:
            printk(KERN_INFO "Inserting entry to storage.\n");
            break;
        case LOOKUP:
            printk(KERN_INFO "Looking for entry in storage.\n");
            break;
		case REMOVE:
			printk(KERN_INFO "Removing entry from storage.");
			break;
        default: 
            printk(KERN_INFO "Default.\n");
    }
    return 0;
}

/**
 * Function: kvm_init
 * -------------------------
 * Initialize the lkm and kvs
 * 
 * returns: 0 on success
 *          -1 on error
 */
static int __init kvm_init(void)
{
    // Allocate memory for key value store
    kvs = kcalloc(256, sizeof(struct KeyValuePair *), GFP_KERNEL);
    if (kvs == NULL) {
        printk(KERN_ERR "ERROR: Unable to allocate memory for the bucket.");
        return -1;
    }

    // Allocate major number
    if ((alloc_chrdev_region(&dev, 0, 1, "kvm")) < 0) {
        printk(KERN_ERR "ERROR: Unable to allocate major number.\n");
        return -1;
    }

    cdev_init(&kvm_cdev, &fops);

    // Create char device
    if ((cdev_add(&kvm_cdev, dev, 1)) < 0) {
        printk(KERN_ERR "ERROR: Unable to add the device to the system.\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    // Create class
    if (IS_ERR(kvm_class = class_create("kvm"))) {
        printk(KERN_ERR "ERROR: Unable to create the class.\n");
        class_destroy(kvm_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    // Create device 
    if (IS_ERR(device_create(kvm_class, NULL, dev, NULL, "kvm"))) {
        printk(KERN_ERR "ERROR: Unable to create the device.\n");
        class_destroy(kvm_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    printk(KERN_INFO "SUCCESS: Device driver inserted!\n");
    return 0;
}

/**
 * Function: kvm_exit
 * -------------------------
 * Removes the device and class.
 */
static void __exit kvm_exit(void)
{
	device_destroy(kvm_class, dev);
    class_destroy(kvm_class);
    cdev_del(&kvm_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Exiting module.\n");
}

module_init(kvm_init);
module_exit(kvm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoffer Nordlander, Ludvig Larsson");
MODULE_DESCRIPTION("A module that provides a key-value store like functionality.\n");
MODULE_VERSION("0.1");