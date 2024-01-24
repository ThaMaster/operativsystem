#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/err.h>

#define WR_VALUE _IOW('a', 'a', int32_t *)
#define RD_VALUE _IOR('a', 'b', int32_t *)

dev_t dev = 0;
static struct class *kvm_class;
static struct cdev kvm_cdev;


/**
 * Function prototypes
 */
static int __init start(void);
static void __exit end(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops = 
{
    .owner          = THIS_MODULE,
    .read           = etx_read,
    .write          = etx_write,
    .open           = etx_open,
    .unlocked_ioctl = etx_ioctl,
    .release        = etx_release,
};


static int etx_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Divice file opened!\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Divice file closed.\n");
    return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read function.\n");
    return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write function.\n");
    return 0;
}


static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case WR_VALUE:
            printk(KERN_INFO "Writing!\n");
            break;
        case RD_VALUE:
            printk(KERN_INFO "Reading!\n");
            break;
        default:
            printk(KERN_INFO "Default\n");
    }
    return 0;
}

static int __init start(void)
{
    printk(KERN_INFO "Starting module\n");
    if ((alloc_chrdev_region(&dev, 0, 1, "hello_Dev")) < 0) {
        printk(KERN_ERR "Cannot allocate major number.\n");
        return -1;
    }

    printk(KERN_INFO "Major = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&kvm_cdev, &fops);

    if ((cdev_add(&kvm_cdev, dev, 1)) < 0) {
        printk(KERN_ERR "Cannot add the device to the system.\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    if (IS_ERR(kvm_class = class_create("hello_class"))) {
        printk(KERN_ERR "Cannot create the struct class.\n");
        class_destroy(kvm_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    if (IS_ERR(device_create(kvm_class, NULL, dev, NULL, "hello_device"))) {
        printk(KERN_ERR "Cannot create the device 1.\n");
        class_destroy(kvm_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    printk(KERN_INFO "Divice driver inserted!\n");
    return 0;
}

static void __exit end(void)
{
    device_destroy(kvm_class, dev);
    class_destroy(kvm_class);
    cdev_del(&kvm_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Exiting  module\n");
}

module_init(start);
module_exit(end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoffer / Ludvig");
MODULE_DESCRIPTION("A ioctl module for handling communication between kernel and user level.\n");
MODULE_VERSION("0.1");