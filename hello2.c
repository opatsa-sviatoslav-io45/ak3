#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Opatsa Sviatoslav <botnebotua@gmail.com>");
MODULE_DESCRIPTION("Hello2: calls print_hello() count times");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of times to print \"Hello, world!\" (default 1)");

static int __init hello2_init(void)
{
	uint i;

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello2: count = %u, continuing\n", count);

	if (count > 10) {
		pr_err("hello2: count = %u is greater than 10\n", count);
		return -EINVAL;
	}

	for (i = 0; i < count; i++)
		print_hello();
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
