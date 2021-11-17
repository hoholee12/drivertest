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

//never use too much allocation as this takes precedence and will force userspace pages to swap out and crawl to halt.

//kmalloc dynamically allocates memory from kernel space. it is 1:1 mapping to physical:virtual and contiguous.
//GFP_KERNEL: can block until it finds sufficient memory. do not use for interrupt services.
//GFP_ATOMIC: return NULL immediately if memory insufficient. no block.
//GFP_DMA: allocate memory from DMA zone from 32bit machines. same as GFP_KERNEL for 64bit.
void kmalloc_test(void){	//parameter must mention void to confirm that its not prototype.
	char* buf;
	printhello("kmalloc test");
	buf = kmalloc(1024, GFP_KERNEL);
	if(buf> 0){
		printhello("kmalloc small test ok!");
		kfree(buf);
	}
	buf = kmalloc(32*PAGE_SIZE, GFP_KERNEL);
	if(buf> 0){
		printhello("kmalloc big test ok!");
		kfree(buf);
	}
	
}

//similar to kmalloc but vmalloc is slow because:
//allocation from HIGH memory. -> its not 1:1 so its non contiguous, and setting up page table is slower than simple 1:1 mapping of kmalloc. however you can allocate far more memory.
//it will never be swapped out since its still critical kernel part.
void vmalloc_test(void){
	char* buf;
	
	printhello("vmalloc test");
	
	buf = vmalloc(32*PAGE_SIZE);
	if(buf> 0){
		printhello("vmalloc big test ok!");
		vfree(buf);
	}
	
}

//get them directly from buddy system.
void get_free_pages_test(void){
	unsigned long buf; //address
	int order;
	int input = PAGE_SIZE*4;
	printhello("get_free_pages test");
	
	order = get_order(input);
	printhello("input of %d, get_order of: %d", input, order);
	buf = __get_free_pages(GFP_KERNEL, order);
	if(buf>0){
		printhello("get_free_pages test ok");
		free_pages(buf, order);
	}


}

//memory pools are for reserving memory incase of emergency.
//if dealing with large amount of data processing, kernel memory can run out fast.
// so this was introduced.
//this is used alot in block device drivers..
#define MIN_ELEMENT 4
#define TEST_ELEMENT 4
typedef struct{
	int number;
	char string[128];
} TMemElement;
int elementcount = 0;
void *mempool_alloc_test(gfp_t gfp_mask, void *pool_data){
	TMemElement *data;
	printhello("mempool allocated");
	data = kmalloc(sizeof(TMemElement), gfp_mask);
	if(data>0) data->number = elementcount++;
	return data;
}
void mempool_free_test(void *element, void *pool_data){
	printhello("mempool freed");
	if(element>0) kfree(element);
	
}



static int __init hello_init(void){
	struct task_struct *task;


	//print
	//printhello("asdfadsdf\n");
	
	//parameter test
	//printhello("paramtest = %d", paramTest);
	//printhello("paramarray[0] = %d, paramarray[1] = %d, paramarray[2] = %d,", paramArray[0],paramArray[1],paramArray[2]);
	
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
	/*
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
	*/

	//kmalloc_test();
	//vmalloc_test();
	//get_free_pages_test();

	mempool_t *mp;
	TMemElement *element[TEST_ELEMENT];
	int lp;

	printhello("start mempool allocation test");
	memset(element, 0, sizeof(element));
	mp = mempool_create(MIN_ELEMENT, mempool_alloc_test, mempool_free_test, NULL);
	for(lp=0;lp<TEST_ELEMENT;lp++){
		element[lp] = mempool_alloc(mp, GFP_KERNEL);
		if(element[lp]==NULL) printhello("alloc fail");
		else{
			printhello("alloc data %d", element[lp]->number);
		}
	}
	for(lp=0;lp<TEST_ELEMENT;lp++){
		if(element[lp]) mempool_free(element[lp], mp);
	}

	mempool_destroy(mp);

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
