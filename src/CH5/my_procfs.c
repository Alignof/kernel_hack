#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/cpufreq.h>
#include <linux/utsname.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <linux/time_namespace.h>
#include <asm/page_types.h>
#include <asm/processor.h>
#include <asm/cpufeature.h>

#include "my_procfs.h"

#define pagenum_to_KB(x) ((x) << (PAGE_SHIFT - 10))
#define DRIVER_NAME "MyDevice"
#define PROC_NAME "MyProcFs"
#define BUF_SIZE 512

static char buffer[BUF_SIZE];
static unsigned int major_num;

MODULE_LICENSE("Dual BSD/GPL");


static int mydevice_open(struct inode *inode, struct file *fp) {
    printk("open my device\n");
    int *buf_pos = kmalloc(sizeof(int), GFP_KERNEL);
    *buf_pos = BUF_SIZE;
    fp->private_data = buf_pos;
    return 0;
}

static int mydevice_close(struct inode *inode, struct file *fp) {
    printk("close my device\n");
    return 0;
}

static ssize_t mydevice_read(struct file *fp, char __user *_buf, size_t count, loff_t *f_pos) {
    const int cpu_id = 0;
    int *buf_pos = fp->private_data;
    struct cpuinfo_x86 *c = &cpu_data(cpu_id);

    if (*buf_pos == BUF_SIZE) {
        unsigned int freq = 0;
        typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
        kallsyms_lookup_name_t kallsyms_lookup_name;
        static struct kprobe kp = {
           .symbol_name = "kallsyms_lookup_name"
        };
        register_kprobe(&kp);
        kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
        unregister_kprobe(&kp);
        unsigned int (*aperfmperf_get_khz)(int) = kallsyms_lookup_name("aperfmperf_get_khz");
     
        if (cpu_has(c, X86_FEATURE_TSC)) {
            if (aperfmperf_get_khz != 0) freq = aperfmperf_get_khz(cpu_id);

            if (!freq) freq = cpufreq_quick_get(cpu_id);
            if (!freq) freq = cpu_khz;
        }

        struct sysinfo mem_info;
        si_meminfo(&mem_info);

        struct timespec64 uptime;
        ktime_get_boottime_ts64(&uptime);
        timens_add_boottime(&uptime);

        *buf_pos = snprintf(
            buffer, 
            sizeof(buffer), 
            "========= system =========\n"
            "kernel version: %s %s %s\n"
            "uptime: %lu.%02lu\n"
            "========== cpu ==========\n"
            "vender id: %s\n"
            "cpu family: %u\n"
            "model: %s\n"
            "cpu MHz: %u.%03u\n"
            "cache size: %u KB\n"
            "core id: %u\n"
            "cpu cores: %u\n"
            "========= memory =========\n"
            "MemTotal: %lu\n"
            "MemFree: %lu\n"
            "MemUsed: %lu\n"
            "",
            utsname()->sysname,
            utsname()->release,
            utsname()->version,
            (unsigned long) uptime.tv_sec,
            (uptime.tv_nsec / (NSEC_PER_SEC / 100)),
            c->x86_vendor_id,
            c->x86,
            c->x86_model_id,
            freq / 1000, (freq % 1000),
            c->x86_cache_size,
            c->cpu_core_id,
            c->x86_max_cores,
            pagenum_to_KB(mem_info.totalram),
            pagenum_to_KB(mem_info.freeram),
            pagenum_to_KB(mem_info.totalram - mem_info.freeram)
        ) + 1;
        printk("buf_pos = %d", *buf_pos);
    }

    int copy_size = count;
    if (count > *buf_pos) copy_size = *buf_pos;
    copy_to_user(_buf, buffer, copy_size);

    *f_pos += copy_size;
    int i;
    for (i = copy_size; i < *buf_pos; i++) {
        buffer[i - copy_size] = buffer[i];
    }
    *buf_pos -= copy_size;
    printk( KERN_INFO "%s : buf_pos = %d\n", buffer, *buf_pos );

    printk("read my device\n");

    printk("buf_pos = %d", *buf_pos);
    printk("count = %d", count);
    printk("copy_size = %d", copy_size);

    return copy_size;
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
