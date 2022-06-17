#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include "tutorial_kernel_module.h"
#define DRIVER_NAME "MyDevice"
#define BUF_SIZE 256

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM  = 3;
static unsigned int major_num;
static struct cdev my_cdev;

MODULE_LICENSE("Dual BSD/GPL");

struct _mydevice_file_data {
    unsigned char buffer[BUF_SIZE];
    struct parameter param;
};

static long mydevice_ioctl(struct file *fp, unsigned int cmd, unsigned long arg) {
    printk("called ioctl\n");

    int tmp = 0;
    switch (cmd) {
        case MYDEVICE_SET_VALUES:
            if (copy_from_user(&(((struct _mydevice_file_data *)fp->private_data)->param), (struct parameter __user *)arg, sizeof(struct parameter))) {
                return -EFAULT;
            }
            break;
        case MYDEVICE_GET_VALUES:
            if (copy_to_user((struct parameter __user *)arg, &(((struct _mydevice_file_data *)fp->private_data)->param), sizeof(struct parameter))) {
                return -EFAULT;
            }
            break;
        case MYDEVICE_SWAP_VALUES:
            tmp = ((struct _mydevice_file_data *)fp->private_data)->param.value1;
            ((struct _mydevice_file_data *)fp->private_data)->param.value1 = ((struct _mydevice_file_data *)fp->private_data)->param.value2;
            ((struct _mydevice_file_data *)fp->private_data)->param.value2 = tmp;
            break;
        default:
            printk(KERN_WARNING "unsupported command %d\n", cmd);
            return -EFAULT;
    }

    return 0;
}

static int mydevice_open(struct inode *inode, struct file *fp) {
    printk("open my device\n");

    struct _mydevice_file_data *p = kmalloc(sizeof(struct _mydevice_file_data), GFP_KERNEL);
    if (p == NULL) {
        printk(KERN_ERR  "kmalloc\n");
        return -ENOMEM;
    }

    strlcat(p->buffer, "init", 4);
    p->param.value1 = 0;
    p->param.value2 = 0;
    fp->private_data = p;

    return 0;
}

static int mydevice_close(struct inode *inode, struct file *fp) {
    printk("close my device\n");

    if (fp->private_data) {
        kfree(fp->private_data);
    }

    return 0;
}

static ssize_t mydevice_read(struct file *fp, char __user *_buf, size_t count, loff_t *f_pos) {
    printk("read my device\n");

    if (count > BUF_SIZE) count = BUF_SIZE;

    struct _mydevice_file_data *p = fp->private_data;
    if (copy_to_user(_buf, p->buffer, count) != 0) {
        return -EFAULT;
    }
    return count;
}

static ssize_t mydevice_write(struct file *fp, const char __user *_buf, size_t count, loff_t *f_pos) {
    printk("write my device\n");

    struct _mydevice_file_data *p = fp->private_data;
    if (copy_from_user(p->buffer, _buf, count) != 0) {
        return -EFAULT;
    }
    return count;
}

struct file_operations s_mydevice_fops = {
    .open    = mydevice_open,
    .release = mydevice_close,
    .read    = mydevice_read,
    .write   = mydevice_write,
    .unlocked_ioctl = mydevice_ioctl,
};

static int mydevice_init(void) {
    printk("init my device\n");

    dev_t dev;
    if (alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME) != 0) {
        printk(KERN_ERR  "alloc_chrdev_region = %d\n", major_num);
        return -1;
    }

    major_num = MAJOR(dev);
    dev = MKDEV(major_num, MINOR_BASE);

    cdev_init(&my_cdev,&s_mydevice_fops);
    my_cdev.owner = THIS_MODULE;
    int cdev_err = cdev_add(&my_cdev, dev, MINOR_NUM);
    if (cdev_err != 0) {
        printk(KERN_ERR  "cdev_add = %d\n", cdev_err);
        unregister_chrdev_region(dev, MINOR_NUM);
        return -1;
    }
    return 0;
}

static void mydevice_exit(void) {
    printk("exit my device\n");

    dev_t dev = MKDEV(major_num, MINOR_BASE);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
