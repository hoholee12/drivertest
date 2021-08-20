#pragma once

#include<linux/init.h>
#include<linux/module.h>
#include<linux/string.h>

void printhello(const char* str){
	char a[100];
	strcpy(a, "\0011hello: ");
	// \001 is KERN_SOH
	// 1 is KERN_ALERT
	strcat(a, str);
	printk(a);
}
