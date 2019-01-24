#include "page.h"
#include <linux/init.h>
#include <linux/module.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <linux/thread_info.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/smp.h>
#include <asm-generic/percpu.h>
#include <linux/mm_types.h>
#include <linux/buffer_head.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sun");
struct buffer_head* bh;

static int __init myModule_init(void){

	bh=alloc_buffer_head(GFP_KERNEL);
	printk(KERN_EMERG"one\n");
	BUG_ON(!bh);
	printk(KERN_EMERG"two\n");
	page_address(bh->b_page);
	
	//BUG_ON(!((long int *)(page_address(bh->b_page))));
	printk(KERN_EMERG"three\n");
	//BUG_ON(!(bh->b_data));
	printk(KERN_EMERG"four\n");
	//struct address_space *as=bh->b_assoc_map;
	//BUG_ON(!as);
	#if 1
	printk(KERN_EMERG" size:%ld blocknr:%ld count:%d state:%d data:%d",
		bh->b_size,bh->b_blocknr,bh->b_count.counter,bh->b_state,bh->b_data);
	#endif
	return 0;
}


static void __exit myModule_exit(void)
{
	free_buffer_head(bh);
    printk(KERN_ALERT"myModule exit.\r\n");
	return;
}

module_init(myModule_init);
module_exit(myModule_exit);


