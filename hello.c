#include"hello.h"

//anything other than GPL may not compile
MODULE_LICENSE("GPL");

int paramTest;
//parameter name, datatype, permission for that parameter
//S_I is common header, R read, W write, X execute, USR user, GRP group
module_param(paramTest, int, S_IRUSR|S_IWUSR);	//user r/w permission

int paramArray[3];
module_param_array(paramArray, int, NULL, S_IRUSR|S_IWUSR);


static dev_t devnum;	//for device number

static struct cdev c_dev;	//for chardev struct
static struct class *cl;	//for device class

static int my_open(struct inode* i, struct file* f){
	printhello("open()");
	return 0;
}

static int my_close(struct inode* i, struct file* f){
	printhello("close()");
	return 0;
}

static ssize_t my_read(struct file* f, char __user *buf, size_t len, loff_t *off){
	printhello("read()");
	return 0;
}

static ssize_t my_write(struct file* f, const char __user *buf, size_t len, loff_t *off){
	printhello("write()");
	return len;
}

static struct file_operations some_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write
};

static int __init hello_init(void){
	struct task_struct *task;


	//print
	printhello("asdfadsdf\n");
	
	//parameter test
	printhello("paramtest = %d", paramTest);
	printhello("paramarray[0] = %d, paramarray[1] = %d, paramarray[2] = %d,", paramArray[0],paramArray[1],paramArray[2]);
	
	//printhello("the pid for the process %s is = %d", current->comm, current->pid);

	//macro for listing process
	/*
	for_each_process(task){
		printhello("process name: %s, PID: %d, ProcessState: %ld", task->comm, task->pid, task->state);
	}
	*/

	//major minor number test
	/*
	if(alloc_chrdev_region(&devnum, 0, 3, "testcdev") < 0) return -1;
	printhello("chrdev test major:%d minor:%d", MAJOR(devnum), MINOR(devnum));
	*/
	//and then use mknod to create dev file for device... or below..

	//chrdev test
	//associate major minor number
	if(alloc_chrdev_region(&devnum, 0, 3, "chrdev") < 0)return -1;
	//create class(abstraction for that type of device, maybe for /sys?)
	if((cl = class_create(THIS_MODULE, "chrdrv")) == NULL) {
		unregister_chrdev_region(devnum, 3);
		return -1;
	}
	//expose dev file
	if(device_create(cl, NULL, devnum, NULL, "mynull") ==NULL){
		class_destroy(cl);
		unregister_chrdev_region(devnum, 3);
		return -1;
	}
	//create a working character device
	cdev_init(&c_dev, &some_fops);
	if(cdev_add(&c_dev, devnum, 1) == -1){
		device_destroy(cl, devnum);
		class_destroy(cl);
		unregister_chrdev_region(devnum, 3);
		return -1;
	}

	return 0;
}

static void __exit hello_exit(void){
	cdev_del(&c_dev);
	device_destroy(cl, devnum);
	class_destroy(cl);
	unregister_chrdev_region(devnum, 3);
		
	
	printhello("byebye\n");

}

module_init(hello_init);
module_exit(hello_exit);
