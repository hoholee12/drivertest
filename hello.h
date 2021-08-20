#pragma once

#include<linux/init.h>
#include<linux/module.h>
#include<linux/string.h>
#include<linux/sched.h>

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
