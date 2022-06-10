#include <linux/module.h>
#define DRIVER_NAME "MyDevice_NAME"
#define DRIVER_MAJOR 63

MODULE_LICENSE("Dual BSD/GPL");

struct file_operations s_mydevice_fops = {
    .open    = mydevice_open,
    .release = mydevice_close,
    .read    = mydevice_read,
    .write   = mydevice_write,
};

static int mydevice_open(struct inode *inode, struct file *fp) {
    printk("open my device\n");
    return 0;
}

static int mydevice_close(struct inode *inode, struct file *fp) {
    printk("close my device\n");
    return 0;
}

static int mydevice_read(struct file *fp, char __user *_buf, size_t count, loff_t *f_pos) {
    printk("read my device\n");

    char *buf = kmalloc(sizeof(char) * count, GFP_KERNEL);
    if (copy_from_user(buf, _buf, sizeof(char) * count) > 0) return EFAULT;
    buf[0] = 'A';
    if (copy_to_user(_buf, buf, sizeof(char) * count) > 0) return EFAULT;

    return 1;
}

static int mydevice_write(struct file *fp, char __user *_buf, size_t count, loff_t *f_pos) {
    printk("write my device\n");
    return 1;
}

static int mydevice_init(void) {
    printk("init my device\n");
    register_chrdev(DRIVER_MAJOR, DRIVER_NAME, &s_myDevice_fops);
    return 0;
}

static void mydevice_exit(void) {
    printk("exit my device\n");
    unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
