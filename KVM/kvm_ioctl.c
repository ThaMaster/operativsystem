#include "kvm_ioctl.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/rwlock_types.h>
#include "kvm.h"

static int __init kvm_ioctl_init(void);
static void __exit kvm_exit(void);
static int kvm_open(struct inode *inode, struct file *file);
static int kvm_close(struct inode *inode, struct file *file);
static long kvm_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// SYNCHRONIZATION
static DEFINE_RWLOCK(rw_lock);

dev_t dev = 0;
static struct class *kvm_class;
static struct cdev kvm_cdev;

static struct file_operations fops = 
{
    .owner          = THIS_MODULE,
    .open           = kvm_open,
    .unlocked_ioctl = kvm_ioctl,
    .release        = kvm_close,
};

/**
 * Function: kvm_open
 * -------------------------
 * 
 */
static int kvm_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "KVM opened.\n");
    return 0;
}

/**
 * Function: kvm_close
 * -------------------------
 * 
 */
static int kvm_close(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "KVM closed.\n");
    return 0;
}

/**
 * Function: kvm_ioctl
 * -------------------------
 * 
 */
static long kvm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct InputOutput IO;
    unsigned long flags;
    char *key;
    switch(cmd) {
        case INSERT:
            printk(KERN_INFO "Inserting entry to storage.\n");
            if(copy_from_user(&IO, (struct InputOutput *) arg, sizeof(IO)))
            {
                printk(KERN_ERR "ERROR: Cannot copy FROM user arguments.\n");
                break;
            }
            // printk(KERN_INFO "key: \"%s\"", (char *)IO.kvp->key);
            // printk(KERN_INFO "value: \"%s\"", (char *)IO.kvp->value);
            
            write_lock_irqsave(&rw_lock, flags);
            IO.status = kvm_insert(IO.kvp);

            if(copy_to_user((void *) arg, &IO, sizeof(struct InputOutput)))
            {
                printk(KERN_ERR "ERROR: Successful INSERT, but can not return value to user.\n");
            }

            write_unlock_irqrestore(&rw_lock, flags);
            break;
        case LOOKUP:
            printk(KERN_INFO "Looking for entry in storage.\n");
            if(copy_from_user(&IO, (struct InputOutput *) arg, sizeof(IO)))
            {
                printk(KERN_ERR "ERROR: Cannot copy FROM user arguments.\n");
                break;
            }
            
            read_lock_irqsave(&rw_lock, flags);
            if (IO.kvp->key == NULL) {
                printk(KERN_INFO "ERROR: No value of key.");
                IO.status = -1;
            } else {
                key = IO.kvp->key;

                IO.kvp = kvm_lookup(key);
                if (IO.kvp == NULL) {
                    IO.status = -1;
                } else {
                    IO.status = 0;
                }
            }

            if(copy_to_user((struct InputOutput *) arg, &IO, sizeof(IO)))
            {
                printk(KERN_ERR "ERROR: Successful LOOKUP, but can not return value to user.\n");
            }     

            read_unlock_irqrestore(&rw_lock, flags);            
            break;
		case REMOVE:
			printk(KERN_INFO "Removing entry from storage.");
            if(copy_from_user(&IO, (struct InputOutput *) arg, sizeof(IO)))
            {
                printk(KERN_ERR "ERROR: Cannot copy FROM user arguments.\n");
                break;
            }

            write_lock_irqsave(&rw_lock, flags);
            if (IO.kvp == NULL) {
                printk(KERN_INFO "ERROR: No value of key.");
                IO.status = -1;
            } else {
                key = IO.kvp->key;

                IO.kvp = kvm_remove(key);

                if (IO.kvp == NULL) {
                    IO.status = -1;
                } else {
                    IO.status = 0;
                }
            }
            
            if(copy_to_user((struct InputOutput *) arg, &IO, sizeof(IO)))
            {
                printk(KERN_ERR "ERROR: Successful REMOVE, but can not return value to user.\n");
            }

            write_unlock_irqrestore(&rw_lock, flags);
			break;
        default: 
            printk(KERN_ERR "ERROR: Unknown command.\n");
            IO.status = -1;
            if(copy_to_user((struct InputOutput *) arg, &IO, sizeof(IO)))
            {
                printk(KERN_ERR "ERROR: Was not able to send error to user.\n");
            }
    }
    return 0;
}

/**
 * Function: kvm_ioctl_init
 * -------------------------
 * Initialize the lkm and kvs
 * 
 * returns: 0 on success
 *          -1 on error
 */
static int __init kvm_ioctl_init(void)
{
    // Initialize key value store and receieve.
    kvm_init();

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
    // SEND ALL DATA AND FREE BUCKETS.
	device_destroy(kvm_class, dev);
    class_destroy(kvm_class);
    cdev_del(&kvm_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Exiting module.\n");
}

module_init(kvm_ioctl_init);
module_exit(kvm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoffer Nordlander, Ludvig Larsson");
MODULE_DESCRIPTION("A module that provides a key-value store like functionality.\n");
MODULE_VERSION("0.1");