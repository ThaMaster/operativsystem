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
    ioctl(fd, INSERT, (int32_t *)&number);
    
    printf("Looking up entry.\n");
    int32_t value;
    ioctl(fd, LOOKUP, (int32_t *)&value);

    printf("Removing KeyValuePair\n");
    int32_t cringe;
    ioctl(fd, REMOVE, (int32_t *)&cringe);

    printf("Closing Driver\n");
    close(fd);
}