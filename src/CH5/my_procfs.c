#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include "my_procfs.h"

#define DRIVER_NAME "MyDevice"
#define PROC_NAME "MyProcFs"
#define BUF_SIZE 128

static unsigned int major_num;
static unsigned int read_ok = 1;
static char buffer[BUF_SIZE];

MODULE_LICENSE("Dual BSD/GPL");

static int mydevice_open(struct inode *inode, struct file *fp) {
    printk("open my device\n");

    read_ok = 1;

    return 0;
}

static int mydevice_close(struct inode *inode, struct file *fp) {
    printk("close my device\n");

    return 0;
}

static ssize_t mydevice_read(struct file *fp, char __user *_buf, size_t count, loff_t *f_pos) {
    if (read_ok) {
        printk("read my device\n");

        if (count > BUF_SIZE) count = BUF_SIZE-1;
        sprintf(_buf, "%s\n", buffer);

        read_ok = 0;
    } else {
        return 0;
    }

    return count;
}

static ssize_t mydevice_write(struct file *fp, const char __user *_buf, size_t count, loff_t *f_pos) {
    printk("write my device\n");

    if (count > BUF_SIZE) count = BUF_SIZE-1;
    if (copy_from_user(buffer, _buf, count) != 0) {
        return -EFAULT;
    }
    buffer[count] = '\0';
    return count;
}

static struct proc_ops s_mydevice_fops = {
    .proc_open    = mydevice_open,
    .proc_release = mydevice_close,
    .proc_read    = mydevice_read,
    .proc_write   = mydevice_write,
};

static int mydevice_init(void) {
    printk("init my device\n");

    struct proc_dir_entry *entry;
    entry = proc_create(PROC_NAME, S_IRUGO | S_IWUGO, NULL, &s_mydevice_fops);
    if (entry == NULL) {
        printk(KERN_ERR, "proc_create\n");
        return -ENOMEM;
    }

    return 0;
}

static void mydevice_exit(void) {
    printk("exit my device\n");

    remove_proc_entry(PROC_NAME, NULL);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
