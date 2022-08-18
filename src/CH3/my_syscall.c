#include <linux/sort.h>
#include <linux/syscalls.h>

#define	EFAULT 14	/* Bad address */
#define	EINVAL 22	/* Invalid argument */
#define BUFFERSIZE 100

static int compare(const void *l, const void *r) {
    int lhs = *(const int *)(l);
    int rhs = *(const int *)(r);

    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

int do_array_median(int __user *_array, const int size, int __user *_result) {
    int array[BUFFERSIZE] = {0};
    int result = 0;

    printk("syscall array_median start.");
    if (size <= 0) return -EINVAL;
    if (size > BUFFERSIZE) return -EINVAL;
    if (copy_from_user(array, _array, sizeof(int) * size) > 0) return -EFAULT;
    if (copy_from_user(&result, _result, sizeof(int)) > 0) return -EFAULT;

    sort(array, size, sizeof(int), &compare, NULL);

    result = array[size/2];

    if (copy_to_user(_result, &result, sizeof(int)) > 0) return -EFAULT;
    
    printk("syscall array_median done.");
    return 0;
}

// https://elixir.bootlin.com/linux/v5.17.1/source/fs/namei.c#L4097
SYSCALL_DEFINE3(array_median,
        int __user *, _array,
        const int, size,
        int __user *, _result) {

    return do_array_median(_array, size, _result);
}
