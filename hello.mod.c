#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xa4b86400, "module_layout" },
	{ 0x979bdcd9, "sysfs_remove_bin_file" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x725a8218, "cdev_del" },
	{ 0x48aac170, "root_device_unregister" },
	{ 0x7d82ab27, "sysfs_create_bin_file" },
	{ 0x298c03ad, "__root_device_register" },
	{ 0xd2198226, "cdev_add" },
	{ 0x8b58c8f8, "device_create" },
	{ 0x2b6d8feb, "cdev_alloc" },
	{ 0xcaed4f8, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x1b44c663, "current_task" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xc5850110, "printk" },
	{ 0x9ea53d7f, "vsnprintf" },
	{ 0xf9c0b663, "strlcat" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "8993770932538030396591F");
