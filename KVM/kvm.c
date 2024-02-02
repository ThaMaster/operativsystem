#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>

#include "kvm_ioctl.h"
#include "kvm.h"

uint8_t kvm_hash(const char key[]);
int kvm_init(void);
struct KeyValuePair *kvm_lookup(const char[]);
struct KeyValuePair *kvm_remove(const char[]);
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
struct KeyValuePair *kvm_lookup(const char key[])
{
    if (buckets == NULL) {
        printk(KERN_ERR "ERROR: Key value store has not been initialized.");
        return NULL;
    }

    printk(KERN_INFO "kvm_lookup reached.");

    uint8_t hashed_key = kvm_hash(key);
    struct Bucket *bucket = buckets[hashed_key];
    if (bucket != NULL) {
        while (strcmp(bucket->value->key, key)) {
            if (bucket->next != NULL) {
                bucket = bucket->next;
            } else {
                printk(KERN_INFO "Unable to find KeyValuePair in bucket.");
                return NULL;
            }
        }
        printk(KERN_INFO "SUCCESS: Returning value from lookup");
        return bucket->value;
    }

    printk(KERN_INFO "Could not find bucket with key: \"%s\".\n", key);
    return NULL;
}

/**
 * Function: kvm_remove
 * -------------------------
 * 
 */
struct KeyValuePair *kvm_remove(const char key[])
{
    if (buckets == NULL) {
        printk(KERN_ERR "ERROR: Key value store has not been initialized.");
        return NULL;
    }

    uint8_t hashed_key = kvm_hash(key);

    struct Bucket *prev_bucket;
    struct Bucket *curr_bucket = buckets[hashed_key];
    if(curr_bucket != NULL) {
        while(strcmp(curr_bucket->value->key, key)) {
            if(curr_bucket->next != NULL) {
                prev_bucket = curr_bucket;
                curr_bucket = curr_bucket->next;
            } else {
                printk(KERN_INFO "Unable to find the element to remove.");
                return NULL;
            }
        }
        
        if (prev_bucket != NULL) {
            prev_bucket->next = curr_bucket->next;
        } else {
            buckets[hashed_key] = NULL;
        }
        struct KeyValuePair *res = curr_bucket->value;
        kfree(curr_bucket);
        printk(KERN_INFO "SUCCESS: Removed bucket successfully!");

        return res;
    }

    printk(KERN_INFO "Unable to find the element to remove.");
    return NULL;
}

/**
 * Function: kvm_insert
 * -------------------------
 * 
 */
int kvm_insert(struct KeyValuePair *kvp)
{
    if (buckets == NULL) {
        printk(KERN_ERR "ERROR: Key value store has not been initialized.");
        return -1;
    }
    printk(KERN_INFO "kvm_insert reached.");

    uint8_t hashed_key = kvm_hash(kvp->key);
    struct Bucket *last_bucket = buckets[hashed_key];
    struct Bucket *new_bucket;

    if (last_bucket == NULL) {

        new_bucket = kcalloc(1, sizeof(struct Bucket), GFP_KERNEL);
        if (new_bucket == NULL) {
            printk(KERN_ERR "ERROR: Unable to allocate memory for bucket.\n");
            return -1;
        }

        new_bucket->value = kvp;
        buckets[hashed_key] = new_bucket;
    } else {
        // Loop to get to the last element in bucket list
        while (last_bucket->next != NULL) {
            last_bucket = last_bucket->next;
        }

        new_bucket = kcalloc(1, sizeof(struct Bucket), GFP_KERNEL);
        if (new_bucket == NULL) {
            printk(KERN_ERR "ERROR: Unable to allocate memory for bucket.\n");
            return -1;
        }

        new_bucket->value = kvp;
        last_bucket->next = new_bucket;
    }

    printk(KERN_INFO "SUCCESS: Inserted entry successfully!");
    return 0;
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