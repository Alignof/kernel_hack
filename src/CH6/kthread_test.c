//#include <curl/curl.h>
#include "/usr/include/x86_64-linux-gnu/curl/curl.h"
#include "jansson.h"
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/string.h>
#include <linux/file.h>
#include <linux/fileattr.h>


#define APIKEY "3a3c618b4b1b4733be25043f17ee86a7"

MODULE_LICENSE("Dual BSD/GPL");

struct task_struct *k;

struct MemFile {
    char* data;
    size_t size;
};

struct MemFile* memfopen() {
    struct MemFile* mf = (struct MemFile*) malloc(sizeof(struct MemFile));
    if (mf) {
        mf->data = NULL;
        mf->size = 0;
    }

    return mf;
}

void memfclose(struct MemFile* mf) {
    if (mf->data) free(mf->data);
    free(mf);
}

size_t memfwrite(char* ptr, size_t size, size_t nmemb, void* stream) {
    struct MemFile* mf = (struct MemFile*) stream;
    int block = size * nmemb;
    if (!mf) return block; // through

    if (!mf->data) {
        mf->data = (char*) malloc(block);
    } else {
        mf->data = (char*) realloc(mf->data, mf->size + block);
    }

    if (mf->data) {
        memcpy(mf->data + mf->size, ptr, block);
        mf->size += block;
    }

    return block;
}

char* memfstrdup(struct MemFile* mf) {
    if (mf->size == 0) return NULL;

    char* buf = (char*) malloc(mf->size + 1);
    memcpy(buf, mf->data, mf->size);
    buf[mf->size] = 0;

    return buf;
}

static void kthread_main(void) {
    ssleep(120);
    CURLcode res;

    struct MemFile *mf = memfopen();
    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            "https://api.currencyfreaks.com/latest?apikey="
            APIKEY
            "&symbols=JPY"
        );
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, mf);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memfwrite);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);

    char* js = memfstrdup(mf);
    memfclose(mf);
    json_error_t error;
    json_t *result = json_loads(js, 0, &error);

    if (result == NULL) {
        fputs(error.text, stderr);
        free(js);
        return 0;
    }

    json_t *rates = json_object_get(result, "rates");
    printk(
        "1USD = %sJPY\n",
        json_string_value(json_object_get(rates, "JPY"))
    );
    json_decref(result);

    free(js);
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
