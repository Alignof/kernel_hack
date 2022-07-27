#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>

MODULE_LICENSE("Dual BSD/GPL");

struct task_struct *k;

static void kthread_main(void) {
    ssleep(30);
    printk("kthread wake up");
}

static int kthread_function(void* arg) {
    printk(KERN_INFO "[%s] start kthread\n", k->comm);

    while (!kthread_should_stop()) {
        kthread_main();
    }

    printk(KERN_INFO "[%s] stop kthread\n", k->comm);

    return 0;
}

static int __init testmod_init(void) {
    printk(KERN_INFO "driver loaded\n");

    k = kthread_run(kthread_function, NULL, "testmod kthread");

    return 0;
}

static void __exit testmod_exit(void) {
    kthread_stop(k);
    printk(KERN_INFO "driver unloaded\n");
}

module_init(testmod_init);
module_exit(testmod_exit);
