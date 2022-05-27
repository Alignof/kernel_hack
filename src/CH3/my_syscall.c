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

int do_median(int *array, int size, float *result) {
    quick_sort(array, 0, size-1);
    if (size % 2) {
        *result = (float)array[size/2];
    } else {
        *result = (float)((array[(size-1)/2] + array[size/2]) / 2.0);
    }
    
    printk("syscall median done.");
    return 0;
}

// https://elixir.bootlin.com/linux/v5.17.1/source/fs/namei.c#L4097
SYSCALL_DEFINE2(median, int __user *, array, const int size, float __user *, result) {
    return do_sort_array(array, size, result);
}

