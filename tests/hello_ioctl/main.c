#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


#define WR_VALUE _IOW('a','a', int32_t *)
#define RD_VALUE _IOR('a','b', int32_t *)

int main(void) 
{
    int fd;
    printf("\nOpening Driver\n");
    fd = open("/dev/hello_device", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }

    printf("Writing to file\n");
    int32_t number = 255;
    ioctl(fd, WR_VALUE, (int32_t *)&number);
    
    printf("Reading to file\n");
    int32_t value;
    ioctl(fd, RD_VALUE, (int32_t *)&value);

    printf("Closing Driver\n");
    close(fd);
    
    return 0;
}