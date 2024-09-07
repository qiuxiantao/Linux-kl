#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
//关键头文件
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
//这3个头文件在内核源码目录下找到，并放在该项目下
#define DEVICE_NAME "memory_share"
#define CLASS_NAME "memory"
//调用虚拟内存
MODULE_LICENSE("GPL");//协议
MODULE_AUTHOR("xiantaoqiu");//作者
MODULE_DESCRIPTION("Reverse memory module");//模块介绍
MODULE_VERSION("0.1");//模块版本

static int majorNumber;
static struct class* memoryClass = NULL;//返回0
static struct cdev memoryCDev;
static char* sharedMemory;

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MemoryShare: Device has been opened\n");
    return 0;
}
//测试dve通信
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    error_count = copy_to_user(buffer, sharedMemory, 16);

    if (error_count == 0) {
        printk(KERN_INFO "MemoryShare: Sent 16 bytes to the user\n");//发送测试16个字符失败
        return 16;
    } else {
        printk(KERN_INFO "MemoryShare: Failed to send 16 bytes to the user\n");//发生测试16字符成功
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    error_count = copy_from_user(sharedMemory, buffer, len);

    if (error_count == 0) {
        printk(KERN_INFO "MemoryShare: Received %zu bytes from the user\n", len);//成功
        return len;
    } else {
        printk(KERN_INFO "MemoryShare: Failed to receive %zu bytes from the user\n", len);//失败
        return -EFAULT;
    }
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "MemoryShare: Device successfully closed\n");//成功关闭设备
    return 0;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init memory_share_init(void) {
    printk(KERN_INFO "MemoryShare: Initializing the MemoryShare LKM\n");//初始化

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber<0){
        printk(KERN_ALERT "MemoryShare failed to register a major number\n");//无法注册编号
        return majorNumber;
    }
    printk(KERN_INFO "MemoryShare: registered correctly with major number %d\n", majorNumber);

    memoryClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(memoryClass)){
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(memoryClass);
    }
    printk(KERN_INFO "MemoryShare: device class registered correctly\n");

    if (IS_ERR(device_create(memoryClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME))){
        class_destroy(memoryClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(memoryClass);
    }
    printk(KERN_INFO "MemoryShare: device class created correctly\n");
//内存共享
    sharedMemory = kmalloc(16, GFP_KERNEL);
    if (!sharedMemory) {
        device_destroy(memoryClass, MKDEV(majorNumber, 0));
        class_destroy(memoryClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to allocate memory for sharedMemory\n");
        return -ENOMEM;
    }

    cdev_init(&memoryCDev, &fops);
    if (cdev_add(&memoryCDev, MKDEV(majorNumber, 0), 1) < 0) {
        kfree(sharedMemory);
        device_destroy(memoryClass, MKDEV(majorNumber, 0));
        class_destroy(memoryClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "添加dve设备点失败\n");
        return -1;
    }

    return 0;
}

static void __exit memory_share_exit(void) {
    cdev_del(&memoryCDev);
    kfree(sharedMemory);
    device_destroy(memoryClass, MKDEV(majorNumber, 0));
    class_unregister(memoryClass);
    class_destroy(memoryClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "MemoryShare: 卸载模块成功\n");
}

module_init(memory_share_init);
module_exit(memory_share_exit);//释放