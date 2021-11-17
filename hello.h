#pragma once

#include<linux/init.h>
#include<linux/module.h>	//THIS_MODULE struct module* owner
#include<linux/string.h>
#include<linux/sched.h>

#include<linux/sched/signal.h>	//for_each_process macro moved here..

// major is device type?
//minor is specific devices
// for major, 234-254	char	RESERVED FOR DYNAMIC ASSIGNMENT https://www.kernel.org/doc/Documentation/admin-guide/devices.txt

#include<linux/device.h>	//for device related ops
#include<linux/cdev.h>	//to create chardev
#include<linux/fs.h>	//file related ops
#include<linux/slab.h>	//kmalloc()
#include<linux/errno.h>	//error code macros
#include<linux/types.h> //size_t, dev_t, etc...
#include<linux/fcntl.h>	//o_accmode <- mask for file access flag

#include<linux/uaccess.h> 	//copy_to_user/copy_from_user()

#include<linux/vmalloc.h>

#include<linux/mempool.h>	//mempool for reserving memory incase of emergency


void printhello(const char* str, ...){
	va_list arg_ptr;
	char a[100];
	char b[100];
	strcpy(a, KERN_ALERT "hello: ");
	strcat(a, str);

	va_start(arg_ptr, str);
	vsnprintf(b, 100, a, arg_ptr);
	//int vsnprintf(char *target-string, size_t n, const char *format, va_list arg_ptr);
	//a is without args, b is with args. print b
	va_end(arg_ptr);
	printk(b);
}
