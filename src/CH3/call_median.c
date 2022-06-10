#include <stdio.h>
#include "call_median.h"

int main(void) {
    int array[7] = {6, 2, 7, 1, 3, 5, 4};
    int result = 0;

    syscall(__NR_median, array, 7, &result);
    printf("syscall_median(array, 7, &result) -> %d\n", result);
    return 0;
}
