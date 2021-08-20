# when KERNELRELEASE is defined...
ifneq ($(KERNELRELEASE),)
	obj-m := hello.o

# or when it is called directly from command
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
