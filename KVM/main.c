#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "kvm.h"

int main(void)
{
    int fd;
    printf("\nOpening Driver\n");
    fd = open("/dev/kvm", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }

    printf("Inserting KeyValuePair.\n");
    int32_t number = 255;
    ioctl(fd, INSERT, (struct InputOutput *)&number);
    
    printf("Looking up entry.\n");

    struct InputOutput lookingIO;
    struct KeyValuePair keyVal;
    keyVal.key = "cringe";
    lookingIO.kvp = keyVal;
    ioctl(fd, LOOKUP, (struct InputOutput *)&lookingIO);

    printf("Removing KeyValuePair\n");
    int32_t cringe;
    ioctl(fd, REMOVE, (struct InputOutput *)&cringe);

    printf("Closing Driver\n");
    close(fd);
}