#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/cpufreq.h>
#include <x86/kernel/cpu/cpu.h>
#include <asm/processor.h>
#include "my_procfs.h"

#define DRIVER_NAME "MyDevice"
#define PROC_NAME "MyProcFs"
#define BUF_SIZE 256

static unsigned int major_num;

MODULE_LICENSE("Dual BSD/GPL");

static int mydevice_open(struct inode *inode, struct file *fp) {
    printk("open my device\n");
    return 0;
}

static int mydevice_close(struct inode *inode, struct file *fp) {
    printk("close my device\n");

    return 0;
}

static ssize_t mydevice_read(struct file *fp, char __user *_buf, size_t count, loff_t *f_pos) {
    char buffer[BUF_SIZE];
    int cpu_id = 0;
    struct cpuinfo_x86 *c = &cpu_data(cpu_id);

    unsigned int freq;
    if (cpu_has(c->cpu_index, X86_FEATURE_TSC)) {
        freq = aperfmperf_get_khz(cpu_id);

        if (!freq)
            freq = cpufreq_quick_get(cpu_id);
        if (!freq)
            freq = cpu_khz;
    }

    int size = snprintf(
        buffer, 
        BUF_SIZE, 
        "vender id: %s\n"
        "cpu family: %u\n"
        "model: %s\n"
        "cpu MHz: %u.%03u\n"
        "cache size: %u KB\n"
        "core id: %u\n"
        "cpu cores: %u\n",
        c->x86_vendor_id,
        c->x86,
        c->x86_model_id,
        freq / 1000, (freq % 1000),
        c->x86_cache_size,
        c->cpu_core_id,
        c->x86_max_cores
    );

    printk("read my device\n");

    if (count > BUF_SIZE) count = BUF_SIZE-1;
    copy_to_user(_buf, buffer, size);

    return 0;
}

static ssize_t mydevice_write(struct file *fp, const char __user *_buf, size_t count, loff_t *f_pos) {
    printk("write my device\n");

    /*
    if (count > BUF_SIZE) count = BUF_SIZE-1;
    if (copy_from_user(buffer, _buf, count) != 0) {
        return -EFAULT;
    }
    buffer[count] = '\0';
    */
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
