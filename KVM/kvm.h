#define INSERT _IOW('k', 'i', int32_t *)
#define LOOKUP _IOR('k', 'l', int32_t *)
#define REMOVE _IOW('k', 'r', int32_t *)

/**
 * Storage for the key value store
 */
struct KeyValuePair
{
    char key[32];
    char *value;
};