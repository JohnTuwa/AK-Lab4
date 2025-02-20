// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Tyshniuk Ivan");
MODULE_DESCRIPTION("Lab 4");
MODULE_LICENSE("Dual BSD/GPL");

int i;
static unsigned int hello_ammount = 1; // Default meaning
module_param(hello_ammount, uint, 0444);
MODULE_PARM_DESC(hello_ammount, "Ammount of 'Hello, World!' that will be printed");

struct event_list {
struct list_head list;
ktime_t event_time;
};

static LIST_HEAD(event_list_head);

static void new_event(void);

static int __init hello_init(void)
{
if (hello_ammount == 0 || (hello_ammount >= 5 && hello_ammount <= 10))
	pr_warn("Parameter is either 0 or in range 5-10\n");

if (hello_ammount > 10) {
	pr_err("Parameter can`t be larger then 10\n");
	return -EINVAL;
	}

for (i = 0; i < hello_ammount; i++) {
	pr_info("Hello, World!\n");
	new_event();
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct event_list *md, *tmp;

list_for_each_entry_safe(md, tmp, &event_list_head, list) {
	pr_info("Time: %lld\n", md->event_time);
	list_del(&md->list);
	kfree(md);
	}
}

void new_event(void)
{
struct event_list *element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
	if (!element)
		return;
	element->event_time = ktime_get();
	list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);

