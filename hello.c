#include"hello.h"

#include<linux/init.h>
#include<linux/module.h>
#include<linux/string.h>
#include<linux/sched.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<linux/kernel.h>

//number of devices this driver supports
#define NUM_MINOR_DEVS 1U



void printhello(const char* str, ...){
	va_list arg_ptr;
	char a[100];
	char b[100];
	strcpy(a, "\0011hello: ");
	// \001 is KERN_SOH
	// 1 is KERN_ALERT
	strcat(a, str);

	va_start(arg_ptr, str);
	vsnprintf(b, 100, a, arg_ptr);
	va_end(arg_ptr);
	printk(b);
}

//driver's context
//devnum - major/minor num
//pdev - cdev struct
//battr - attributes for sysfs file
//sysfsdev - sysfs entry struct
struct class *my_class = NULL;

struct cdev_ctx{
	dev_t devnum;
	struct cdev *mycdev;
	struct bin_attribute battr;
	struct device *sysfsdev;

};
struct cdev_ctx my_ctx = {
	.devnum = 0,
	.mycdev = NULL,
	.sysfsdev = NULL,
};


//for dev file
static const char test_read_msg[] = "YEAH BOIII";
static size_t test_read_msg_len = 11U;

//for dev file
static ssize_t hello_read(struct file *file, char __user *buf, size_t length, loff_t *offset){
	ssize_t bytes_read = 0;
	
	if(*offset < test_read_msg_len){
		bytes_read = test_read_msg_len - *offset;
		
		//				to, 		from,				length
		if(copy_to_user(buf, test_read_msg + *offset, bytes_read)){
			//it must return 0 bytes left, otherwise err
			return -EFAULT;
		}
		*offset += bytes_read;
	}
	printhello("reading %zu bytes from dev file\n", bytes_read);
	return bytes_read;
}

//for sysfs entry file
static const char test_data_block[] = "WHATS UP BITCHES";
static size_t test_data_block_len = 17U;

//for sysfs entry file
static ssize_t hello_sysfs_read(struct file *file_ptr, struct kobject *kobj, struct bin_attribute *battr, char *data, loff_t offset, size_t size){
	printhello("reading %zu bytes from sysfs\n", size);
	if(size < test_data_block_len) return -EINVAL; //write to file only when app is reading 22 bytes??
	//		to, 	from,			length
	memcpy(data, test_data_block, test_data_block_len);

	return test_data_block_len;
}

const struct file_operations fops = {
	.read = hello_read,
};


static int __init hello_init(void){
	int result;
	printhello("asdfadsdf\n");
	printhello("the pid for the process %s is = %d", current->comm, current->pid);

	//allocate cdev major num
	result = alloc_chrdev_region(&my_ctx.devnum, 0, NUM_MINOR_DEVS, "hello_cdev");
	if(result) goto alloc_fail;
	printhello("dev major: %u\n", MAJOR(my_ctx.devnum));
	
	//create class
	my_class = class_create(THIS_MODULE, "temporary_class");


	//allocate cdev minor num
	my_ctx.mycdev = cdev_alloc();
	if(!my_ctx.mycdev){
		result = -ENOMEM;
		printhello("cdev_alloc failed");
		goto alloc_fail;
	}
	
	//create device file
	device_create(my_class, NULL, my_ctx.devnum, NULL, "hello%d", 0);

	//allocate custom file operations for my cdev
	my_ctx.mycdev->ops = &fops;
	my_ctx.mycdev->owner = THIS_MODULE;
	result = cdev_add(my_ctx.mycdev, my_ctx.devnum, NUM_MINOR_DEVS);
	if(result){
		printhello("cdev_add failed");
		goto add_fail;
	}
	
	//create sysfs "/sys/devices/hello"
	my_ctx.sysfsdev = root_device_register("hello");
	if(IS_ERR(my_ctx.sysfsdev)){
		result = PTR_ERR(my_ctx.sysfsdev);
		printhello("sysfs failed");
		goto add_fail;
	}

	//create sysfs entry on "/sys/devices/hello/read_here"
	sysfs_bin_attr_init(&my_ctx.battr);
	my_ctx.battr.attr.name = "read_here";
	my_ctx.battr.attr.mode = 0644;
	//read operation FOR the sysfs entry...
	my_ctx.battr.read = hello_sysfs_read;
	my_ctx.battr.write = NULL;
	my_ctx.battr.size = test_data_block_len;
	result = sysfs_create_bin_file(&my_ctx.sysfsdev->kobj, &my_ctx.battr);
	if(result) goto sysfs_file_fail;
	
	//end
	return 0;

//reverse
sysfs_file_fail:
	root_device_unregister(my_ctx.sysfsdev);
add_fail:
	cdev_del(my_ctx.mycdev);
alloc_fail:
	unregister_chrdev_region(my_ctx.devnum, NUM_MINOR_DEVS);
	
	return result;
}

static void __exit hello_exit(void){
	printhello("byebye\n");

	//delete sysfs entry file
	sysfs_remove_bin_file(&my_ctx.sysfsdev->kobj, &my_ctx.battr);
	//rest below is same thing as error quit
	root_device_unregister(my_ctx.sysfsdev);
	cdev_del(my_ctx.mycdev);
	unregister_chrdev_region(my_ctx.devnum, NUM_MINOR_DEVS);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fuck you");
MODULE_DESCRIPTION("fuck you");
