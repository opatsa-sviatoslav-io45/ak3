#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Opatsa Sviatoslav <botnebotua@gmail.com>");
MODULE_DESCRIPTION("Hello1: print_hello() and list of timings");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_event {
	struct list_head list;
	ktime_t t_before;
	ktime_t t_after;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
	struct hello_event *ev = kmalloc(sizeof(*ev), GFP_KERNEL);

	if (!ev) {
		pr_err("hello1: no memory for event\n");
		return;
	}
	ev->t_before = ktime_get();
	pr_info("Hello, world!\n");
	ev->t_after = ktime_get();
	list_add_tail(&ev->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *ev, *tmp;

	list_for_each_entry_safe(ev, tmp, &hello_list, list) {
		pr_info("hello1: print took %lld ns\n",
			ktime_to_ns(ktime_sub(ev->t_after, ev->t_before)));
		list_del(&ev->list);
		kfree(ev);
	}
	pr_info("hello1: unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
