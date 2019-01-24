#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/mm.h>
#include<linux/sched.h>
#include<asm/io.h>
#include<asm/uaccess.h>
#include<asm/system.h>

#include<linux/device.h>

dev_t devno;
struct class * simple_class;
static struct cdev cdev;

wait_queue_head_t simple_queue;

char test_data[255];
int len;

ssize_t simple_read(struct file * pfile,
    char __user * buf, size_t size, loff_t * ppos)
{
    /* 定义一个等待项，添加到等待队列，并设置进程状态后放弃执行权 */
    DECLARE_WAITQUEUE (simple_item, current);
    add_wait_queue(&simple_queue, &simple_item);
    current->state = TASK_INTERRUPTIBLE;
    schedule();
    /* 被唤醒以后，从队列中移除 */
    remove_wait_queue(&simple_queue, &simple_item);

    if (copy_to_user(buf, test_data, len))
        return -EFAULT;
    else
        return len;
}

ssize_t simple_write(struct file * pfile, const char __user * buf, size_t count, loff_t * ppos)
{
    if (count > 255)
    {
        return -EFAULT;
    }

    if (!copy_from_user(test_data, buf, count))
    {
        len = count;
        /* 唤醒等待队列中所有进程 */
        wake_up(&simple_queue);
    }
    return len;
}

int simple_open(struct inode * pnode, struct file * pfile)
{
    printk(KERN_INFO "open simple\n");
    return 0;
}

int simple_release(struct inode * pnode, struct file * pfile)
{
    printk(KERN_INFO "close simple\n");
    return 0;
}

static struct file_operations simple_op =
{
    .owner = THIS_MODULE,
    .read = simple_read,
    .open = simple_open,
    .release = simple_release,
    .write = simple_write,
};

static int __init initialization(void)
{
    int result;

    result = alloc_chrdev_region(&devno, 0, 1, "simple");
    if (result < 0)
        return result;

    cdev_init(&cdev, &simple_op);
    result = cdev_add(&cdev, devno, 1);

    simple_class = class_create(THIS_MODULE, "simple");
    device_create(simple_class, NULL, devno, NULL, "simple");

    printk(KERN_INFO " init simple\n");

    init_waitqueue_head(&simple_queue);

    return result;
}

static void __exit cleanup(void)
{
    device_destroy(simple_class, devno);
    class_destroy(simple_class);

    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
    printk(KERN_INFO " cleanup simple\n");
}

module_init(initialization);
module_exit(cleanup);

MODULE_AUTHOR("alloc cppbreak@gmail.com");
MODULE_DESCRIPTION("A simple linux kernel module");
MODULE_VERSION("V0.2");
MODULE_LICENSE("Dual BSD/GPL");

