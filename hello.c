#include"hello.h"


MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void){
	printhello("asdfadsdf\n");
	return 0;
}

static void hello_exit(void){
	printhello("byebye\n");

}

module_init(hello_init);
module_exit(hello_exit);
