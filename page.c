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
//#include <asm-generic/page.h>
#include <linux/mm_types.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sun");

//extern pgd_t *pgd_alloc(struct mm_struct *mm);

//extern void *page_address(struct page *page);

struct kmem_cache *cache = NULL;
void *objp=NULL;
char *buf=NULL;
struct kset *test_kset=NULL;

static struct class *myclass;

#if 1/*definition for char device and fops*/
struct reg_dev
{
	char *data;
	unsigned long size;
	
	struct cdev cdev;
};


struct reg_dev *my_devices;


static int chardevnode_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "chardevnode_open is success!\n");
	
	return 0;
}
/*关闭操作*/
static int chardevnode_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "chardevnode_release is success!\n");
	
	return 0;
}
/*IO操作*/
static long chardevnode_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	printk(KERN_EMERG "chardevnode_ioctl is success! cmd is %d,arg is %d \n",cmd,arg);
	
	return 0;
}

ssize_t chardevnode_read(struct file *file, char __user *buf, size_t count, loff_t *f_ops){
	return 0;
}

ssize_t chardevnode_write(struct file *file, const char __user *buf, size_t count, loff_t *f_ops){
	return 0;
}

loff_t chardevnode_llseek(struct file *file, loff_t offset, int ence){
	return 0;
}
struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = chardevnode_open,
	.release = chardevnode_release,
	.unlocked_ioctl = chardevnode_ioctl,
	.read = chardevnode_read,
	.write = chardevnode_write,
	.llseek = chardevnode_llseek,
};
static void reg_init_cdev(struct reg_dev *dev,int index){
	int err;
	int devno = MKDEV(numdev_major,numdev_minor+index);
	
	/*数据初始化*/
	cdev_init(&dev->cdev,&my_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &my_fops;
	
	/*注册到系统*/
	err = cdev_add(&dev->cdev,devno,1);
	if(err){
		printk(KERN_EMERG "cdev_add %d is fail! %d\n",index,err);
	}
	else{
		printk(KERN_EMERG "cdev_add %d is success!\n",numdev_minor+index);
	}
}
#endif
static int add(struct work_struct *work){
	printk(KERN_ALERT"work_data:%ld.\r\n",work->data.counter);
}

static int __init myModule_init(void)
{
	struct thread_info * ti;
	struct mm_struct *mm;

	struct task_struct* ts;
	ti=current_thread_info();
	
	struct pt_regs *regs;
	int i=1;
	dev_t num_dev;
#if 0	/*use wrong*/
	struct work_struct *work;
	atomic_long_t data;
	data.counter=1;
	work->data=data;
	work->func=add;
	schedule_work(work);
#endif

#if 0/*fork, but fail*/
	long id=do_fork(CLONE_VFORK | CLONE_VM | SIGCHLD, regs->sp, regs,0, NULL, NULL);
	printk(KERN_ALERT"myModule init----fork_id:%d.\r\n",id);
#endif

#if 0/*thread*/
	long id=kernel_thread(add, regs,CLONE_VFORK);
	printk(KERN_ALERT"kernel_thread_id:%d.\r\n",id);
#endif

#if 0/*task,nice*/
	int nice=task_nice(ti->task);

    printk(KERN_ALERT"myModule init----pid:%d state:%d cpu:%d prio:%d static_prio:%d nice:%d policy:%d.\r\n",
			ti->task->pid,ti->task->state,ti->cpu,ti->cpu,ti->task->prio,ti->task->static_prio,nice,
			ti->task->policy);
#endif

#if 0 /*page,zone*/
	struct page *page=pfn_to_page(100);
	struct zone *zone=page_zone(page);
	long int * addr=kmap(page);
	long int * maybe_addr=page_address(page);

	printk(KERN_ALERT"count:%d flag:%d zone_spanned_pages:%d present_pages:%d addr:%x maybe_addr:%x\n",
				page->_count.counter,page->flags,zone->spanned_pages,zone->present_pages,&addr,&maybe_addr);
#endif

#if 0/*cache*/
	cache = kmem_cache_create("cache",
				sizeof(struct kmem_cache),
				0, SLAB_RECLAIM_ACCOUNT|SLAB_MEM_SPREAD,
				NULL);

	objp=kmem_cache_alloc(cache, GFP_KERNEL);
	printk(KERN_ALERT"kmem_cache_alloc___ size:%d \n",cache->size);

	buf=vmalloc(0xffff);
	printk(KERN_ALERT"vmalloc addr:%x \n",&buf);
#endif

#if 0/*page alloc, kmap, */
	struct page *alloc_page=alloc_pages_current(__GFP_HIGHMEM,2);
	addr=kmap(alloc_page);
	maybe_addr=page_address(alloc_page);
	printk(KERN_ALERT"alloc_page_counter:%d ,addr:%x,maybe_addr:%x",alloc_page->_count.counter,&addr,&maybe_addr);
#endif	

#if 0/*current_thread,schedule*/
	yield();
	
	ti=current_thread_info();
	printk(KERN_ALERT"after yield----pid:%d state:%d cpu:%d prio:%d static_prio:%d nice:%d policy:%d.\r\n",
			ti->task->pid,ti->task->state,ti->cpu,ti->cpu,ti->task->prio,ti->task->static_prio,nice,
			ti->task->policy);

	schedule();

	
	ti=current_thread_info();
	printk(KERN_ALERT"after schedule----pid:%d state:%d cpu:%d prio:%d static_prio:%d nice:%d policy:%d.\r\n",
			ti->task->pid,ti->task->state,ti->cpu,ti->cpu,ti->task->prio,ti->task->static_prio,nice,
			ti->task->policy);
#endif

#if 0/*mm,vm_area*/
	ti=current_thread_info();
	struct mm_struct *cur_mm=ti->task->mm;
	printk(KERN_ALERT"pgd:%ld hiwater_rss:%ld hiwater_vm:%ld map_count:%d\n",
		cur_mm->pgd->pgd,cur_mm->hiwater_rss,cur_mm->hiwater_vm,cur_mm->map_count);

	struct vm_area_struct *vm_area=find_vma(cur_mm,0x12346578&0x400000);
	printk(KERN_ALERT"start:%ld end:%ld\n",vm_area->vm_start,vm_area->vm_end);
#endif

#if 0/*char device kset class */
	test_kset=kset_create_and_add("test",NULL,NULL);
	struct cdev *hello=cdev_alloc();
	
	register_chrdev_region(1,100,"hello");
	cdev_add(hello, 1, 100);
	
	int ret = alloc_chrdev_region(&num_dev,numdev_minor,DEVICE_MINOR_NUM,DEVICE_NAME);
		/*获得主设备号*/
	numdev_major = MAJOR(num_dev);
	printk(KERN_EMERG "adev_region req %d !\n",numdev_major);
	myclass = class_create(THIS_MODULE,DEVICE_NAME);	
	
	my_devices = kzalloc(DEVICE_MINOR_NUM * sizeof(struct reg_dev),GFP_KERNEL);
	
	for(i=0;i<DEVICE_MINOR_NUM;i++){
		my_devices[i].data = kmalloc(REGDEV_SIZE,GFP_KERNEL);
		memset(my_devices[i].data,0,REGDEV_SIZE);
		/*设备注册到系统*/
		reg_init_cdev(&my_devices[i],i);
		
		/*创建设备节点*/
		device_create(myclass,NULL,MKDEV(numdev_major,numdev_minor+i),NULL,DEVICE_NAME"%d",i);
	}
#endif
return 0;
}

static void __exit myModule_exit(void)
{
#if 0/*if use*/
	kmem_cache_free(cache,objp);
	vfree(buf);
#endif

#if 0
	kset_unregister(test_kset);
	int i;
		printk(KERN_EMERG "scdev_exit!\n");
		
		/*除去字符设备*/
		for(i=0;i<DEVICE_MINOR_NUM;i++){
			cdev_del(&(my_devices[i].cdev));
			/*摧毁设备节点函数d*/
			device_destroy(myclass,MKDEV(numdev_major,numdev_minor+i));
		}
		/*释放设备class*/
		class_destroy(myclass);
		/*释放内存*/
		kfree(my_devices);
		
		unregister_chrdev_region(MKDEV(numdev_major,numdev_minor),DEVICE_MINOR_NUM);
#endif
    printk(KERN_ALERT"myModule exit.\r\n");
}

module_init(myModule_init);
module_exit(myModule_exit);
