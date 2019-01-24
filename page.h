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
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/buffer_head.h>


#define waitname "waitforsome"

extern int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);
extern int task_nice(const struct task_struct *p);
extern void set_user_nice(struct task_struct *p, long nice);
extern asmlinkage void __sched schedule(void);
extern void __sched yield(void);
extern void *kmap(struct page *page);
extern struct page *alloc_pages_current(gfp_t gfp, unsigned order);
extern void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags);
extern struct kmem_cache *kmem_cache_create (const char *name, size_t size, size_t align, unsigned long flags, void (*ctor)(void *));
extern void *vmalloc(unsigned long size);
extern struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr);
extern struct kset *kset_create_and_add(const char *name,
				 const struct kset_uevent_ops *uevent_ops,
				 struct kobject *parent_kobj);
extern struct cdev *cdev_alloc(void);
extern int cdev_add(struct cdev *p, dev_t dev, unsigned count);
extern int register_chrdev_region(dev_t from, unsigned count, const char *name);
extern void cdev_init(struct cdev *cdev, const struct file_operations *fops);
extern struct buffer_head *alloc_buffer_head(gfp_t gfp_flags);
extern void free_buffer_head(struct buffer_head * bh);

#define DEVICE_NAME "chardevnode"
#define DEVICE_MINOR_NUM 2
#define DEV_MAJOR 0
#define DEV_MINOR 0
#define REGDEV_SIZE 3000

int numdev_major = DEV_MAJOR;
int numdev_minor = DEV_MINOR;



