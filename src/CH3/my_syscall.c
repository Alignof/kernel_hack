#include <linux/syscalls.h>

void swap (int *x, int *y) {
    int tmp = *x;

    *x = *y;
    *y = tmp;
}

int partition (int *array, int left, int right) {
    int i = left;
    int j = right + 1;
    int pivot = left;

    do {
        do { i++; } while (array[i] < array[pivot]);
        do { j--; } while (array[pivot] < array[j]);
        if (i < j) { swap(&array[i], &array[j]); }
    } while (i < j);

    swap(&array[pivot], &array[j]);

    return j;
}

void quick_sort (int *array, int left, int right) {
    int pivot;

    if (left < right) {
        pivot = partition(array, left, right);
        quick_sort(array, left, pivot-1);
        quick_sort(array, pivot+1, right);
    }
}

int do_median(int __user *_array, int size, float __user *_result) {
    int *array;
    float *result;

    copy_from_user(array, _array, size * sizeof(int));
    copy_from_user(result, _result, sizeof(float));

    quick_sort(array, 0, size-1);
    if (size % 2) {
        *result = (float)array[size/2];
    } else {
        *result = (float)((array[(size-1)/2] + array[size/2]) / 2.0);
    }

    copy_to_user(_result, result, sizeof(float));
    
    printk("syscall median done.");
    return 0;
}

// https://elixir.bootlin.com/linux/v5.17.1/source/fs/namei.c#L4097
SYSCALL_DEFINE3(median,
        int __user *, _array,
        const int, size,
        float __user *, _result) {

    return do_median(_array, size, _result);
}
