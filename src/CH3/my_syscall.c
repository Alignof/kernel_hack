#include <linux/syscalls.h>

#define	EFAULT 14	/* Bad address */
#define	EINVAL 22	/* Invalid argument */
#define BUFFERSIZE 100

void myswap (int *x, int *y) {
    int tmp;

    tmp = *x;
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
        if (i < j) { myswap(&array[i], &array[j]); }
    } while (i < j);

    myswap(&array[pivot], &array[j]);

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

int do_median(int __user *_array, const int size, int __user *_result) {
    int array[BUFFERSIZE] = {0};
    int result = 0;

    printk("syscall median start.");
    if (size <= 0) return EINVAL;
    if (size > BUFFERSIZE) return EINVAL;
    if (copy_from_user(array, _array, sizeof(int) * size) > 0) return EFAULT;
    if (copy_from_user(&result, _result, sizeof(int)) > 0) return EFAULT;

    quick_sort(array, 0, size-1);

    result = array[size/2];

    if (copy_to_user(_result, &result, sizeof(int)) > 0) return EFAULT;
    
    printk("syscall median done.");
    return 0;
}

// https://elixir.bootlin.com/linux/v5.17.1/source/fs/namei.c#L4097
SYSCALL_DEFINE3(median,
        int __user *, _array,
        const int, size,
        int __user *, _result) {

    return do_median(_array, size, _result);
}
