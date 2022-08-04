#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "my_chdev.h"

int main() {
    int fd;
    struct parameter values_set;
    struct parameter values_get;
    values_set.value1 = 1;
    values_set.value2 = 2;

    if ((fd = open("/dev/mydevice0", O_RDWR)) < 0) perror("open");

    if (ioctl(fd, MYDEVICE_SET_VALUES, &values_set) < 0) perror("ioctl_set");
    if (ioctl(fd, MYDEVICE_GET_VALUES, &values_get) < 0) perror("ioctl_get");
    if (ioctl(fd, MYDEVICE_SWAP_VALUES, &values_set) < 0) perror("ioctl_get");
    if (ioctl(fd, MYDEVICE_GET_VALUES, &values_get) < 0) perror("ioctl_get");

    // expected val1 = 2, val2 = 1
    printf("value1 = %d, value2 = %d\n", values_get.value1, values_get.value2);

    if (close(fd) != 0) perror("close");
    return 0;
}
