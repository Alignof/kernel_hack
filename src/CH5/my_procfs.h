#pragma once
#include <linux/ioctl.h>

struct parameter {
    int value1;
    int value2;
};

unsigned int aperfmperf_get_khz(int cpu);

#define MYDEVICE_IOC_TYPE 'X'
#define MYDEVICE_SET_VALUES _IOW(MYDEVICE_IOC_TYPE, 1, struct parameter)
#define MYDEVICE_GET_VALUES _IOR(MYDEVICE_IOC_TYPE, 2, struct parameter)
#define MYDEVICE_SWAP_VALUES _IOR(MYDEVICE_IOC_TYPE, 3, struct parameter)
