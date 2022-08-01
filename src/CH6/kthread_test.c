#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/file.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <linux/tls.h>
#include <net/sock.h>
#include <linux/init.h>
#define BUFFER_SIZE 1024

MODULE_LICENSE("Dual BSD/GPL");

struct task_struct *k;

static int kthread_main(void) {
    struct socket *sock;
    struct sockaddr_in s_addr;
    unsigned short port_num = 80;
    int ret = 0;
    char *send_buf = NULL;
    char *recv_buf = NULL;
    struct kvec send_vec, recv_vec;
    struct msghdr send_msg, recv_msg;

    /* kmalloc a send buffer*/
    send_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (send_buf == NULL) {
        printk("client: send_buf kmalloc error!\n");
        return -1;
    }

    /* kmalloc a receive buffer*/
    recv_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if(recv_buf == NULL){
        printk("client: recv_buf kmalloc error!\n");
        return -1;
    }
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port_num);
    s_addr.sin_addr.s_addr = in_aton("52.73.174.144");
    sock = (struct socket *)kmalloc(sizeof(struct socket), GFP_KERNEL);

    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        printk("client:socket create error!\n");
        return ret;
    } printk("client: socket create ok!\n");

    ret = sock->ops->connect(sock, (struct sockaddr *)&s_addr, sizeof(s_addr), 0);
    if (ret != 0) {
        printk("client: connect error!\n");
        return ret;
    }
    printk("client: connect ok!\n");
    memset(send_buf, 0, BUFFER_SIZE);
    snprintf(
        send_buf,
        BUFFER_SIZE,
        "GET /api/latest.json?app_id=9d0f0bc518774cdb861e50a742964ad5&symbols=JPY HTTP/1.1\r\n"
        "Host: 52.73.174.144\r\n"
        "\r\n"
    );
    memset(&send_msg, 0, sizeof(send_msg));
    memset(&send_vec, 0, sizeof(send_vec));

    send_vec.iov_base = send_buf;
    send_vec.iov_len = BUFFER_SIZE;

    ret = kernel_sendmsg(sock, &send_msg, &send_vec, 1, BUFFER_SIZE);
    if (ret < 0) {
        printk("client: kernel_sendmsg error!\n");
        return ret;
    } else if(ret != BUFFER_SIZE){
        printk("client: ret!=BUFFER_SIZE");
    }
    printk("client: send ok!\n");

    memset(recv_buf, 0, BUFFER_SIZE);
    memset(&recv_vec, 0, sizeof(recv_vec));
    memset(&recv_msg, 0, sizeof(recv_msg));
    recv_vec.iov_base = recv_buf;
    recv_vec.iov_len = BUFFER_SIZE;

    ret = kernel_recvmsg(sock, &recv_msg, &recv_vec, 1, BUFFER_SIZE, 0);
    char *jpy = NULL;
    if (jpy = strstr(recv_buf, "\"JPY\": ")) {
        jpy += sizeof("\"JPY\": ")-1;
    }
    printk("client: received message:\n%.*s\n", 8, jpy);

    kernel_sock_shutdown(sock, SHUT_RDWR);
    sock_release(sock);
    return 0;
}

static int kthread_function(void* arg) {
    printk(KERN_INFO "[%s] start kthread\n", k->comm);

    while (!kthread_should_stop()) {
        kthread_main();
        ssleep(300);
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
