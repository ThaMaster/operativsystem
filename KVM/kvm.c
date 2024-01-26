#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>

#include "kvm_ioctl.h"
#include "kvm.h"

uint8_t kvm_hash(const char key[]);
int kvm_init(void);
struct KeyValuePair *kvm_lookup(const char[]);
int kvm_remove(const char[]);
int kvm_insert(struct KeyValuePair *);

static struct Bucket **buckets;

/**
 * Function: kvm_init
 * -------------------------
 * Initializes the key value store 
 * 
 * return: 0 on success,
 *         -1 on error, also prints error message to KERN_ERR.
 */
int kvm_init() 
{
    // Init store table with 0-255 buckets each with a size of 2.
    buckets = kcalloc(256, sizeof(struct KeyValuePair *), GFP_KERNEL);
    if (buckets == NULL) {
        printk(KERN_ERR "ERROR: Unable to allocate memory for the bucket.");
        return -1;
    }
    return 0;
}

/**
 * Function: kvm_lookup
 * -------------------------
 * 
 */
struct KeyValuePair *kvm_lookup(const char[])
{
    if (buckets != NULL) {
        printk(KERN_ERR "ERROR: Key value store has not been initialized.");
        return NULL;
    }

    return NULL;
}

/**
 * Function: kvm_remove
 * -------------------------
 * 
 */
int kvm_remove(const char[])
{
    if (buckets != NULL) {
        printk(KERN_ERR "ERROR: Key value store has not been initialized.");
        return -1;
    }
    
    return 0;
}

/**
 * Function: kvm_insert
 * -------------------------
 * 
 */
int kvm_insert(struct KeyValuePair *kvp)
{
    if (buckets != NULL) {
        printk(KERN_ERR "ERROR: Key value store has not been initialized.");
        return -1;
    }

    uint8_t hashed_key = kvm_hash(kvp->key);
    struct Bucket *last_bucket = buckets[hashed_key];

    while (last_bucket->next != NULL) {
        last_bucket = last_bucket->next;
    }

    struct Bucket *new_bucket = kcalloc(1, sizeof(struct Bucket), GFP_KERNEL);
    if (new_bucket == NULL) {
        printk(KERN_ERR "ERROR: Unable to allocate memory for bucket.\n");
    }
    new_bucket->value = kvp;

    last_bucket->next = (struct Bucket *)new_bucket;
    
    printk(KERN_INFO "Inserted a new bucket to the KVM!\n");

    return 0;
}

struct KeyValuePair *get_bucket(uint8_t key)
{
    // Check if bucket is allocated
    if (buckets[key] != NULL) {
        kcalloc(2, sizeof(struct KeyValuePair), GFP_KERNEL);
    }

    return NULL;
}

/**
 * Function: kvm_hash
 * -------------------------
 * Hashes a key of type char list into an unsigned uint8_t.
 * 
 * key: Key to be hashed of type char list.
 * 
 * returns: uint8_t.
 */
uint8_t kvm_hash(const char key[])
{
    uint8_t hash = 0;

    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christoffer Nordlander, Ludvig Larsson");
MODULE_DESCRIPTION("A module that provides a key-value store like functionality.\n");
MODULE_VERSION("0.1");