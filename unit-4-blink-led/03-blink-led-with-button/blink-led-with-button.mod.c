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
	{ 0xb71b8d35, "module_layout" },
	{ 0x2b68bd2f, "del_timer" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x6f975db7, "gpiod_to_irq" },
	{ 0x7dce8c7c, "gpiod_export" },
	{ 0x6f6600db, "gpiod_set_debounce" },
	{ 0x19a53ea5, "gpiod_direction_input" },
	{ 0xe97c4103, "ioremap" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xb3c5cdbd, "gpiod_get_raw_value" },
	{ 0x231eb567, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

