#include <stdio.h>
#include "call_median.h"

#define LARGE_SIZE 1000

int main(void) {
    int ok_1[7] = {6, 2, 7, 1, 3, 5, 4};
    int ok_2[6] = {6, 2, 1, 3, 5, 4};
    int test[7] = {6, 2, 7, 1, 3, 5, 4};
    int test_large[LARGE_SIZE] = {0};
    int *test_null = NULL;
    int result = 0;

    int ret_ok_1 = syscall(__NR_median, ok_1, 7, &result);
    if (ret_ok_1 == 0) printf("syscall_median(ok_1, 7, &result) result -> %d\n", result);

    int ret_ok_2 = syscall(__NR_median, ok_2, 6, &result);
    if (ret_ok_2 == 0) printf("syscall_median(ok_2, 6, &result) result -> %d\n", result);

    int ret_err_1 = syscall(__NR_median, test, 0, &result);
    if (ret_err_1) printf("syscall_median(test, 0, &result) returned %d\n", ret_err_1);

    int ret_err_2 = syscall(__NR_median, test, -9, &result);
    if (ret_err_2) printf("syscall_median(test, -9, &result) returned %d\n", ret_err_2);

    int ret_err_3 = syscall(__NR_median, test_large, LARGE_SIZE, &result);
    if (ret_err_3) printf("syscall_median(test_large, LARGE_SIZE, &result) returned %d\n", ret_err_3);

    int ret_err_4 = syscall(__NR_median, test_null, 5, &result);
    if (ret_err_4) printf("syscall_median(test_null, 5, &result) returned %d\n", ret_err_4);

    return 0;
}
