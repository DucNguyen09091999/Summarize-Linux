/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/io.h>

#define GPIO_LED_BASE   0x44E07000
#define ADDR_SIZE   0x1000
#define GPIO_SETDATAOUT_OFFSET  0x194
#define GPIO_CLEARDATAOUT_OFFSET    0x190
#define GPIO_OE_OFFSET  0x134
#define GPIO_DATAOUT    0x13c
#define LED (0x1 << 30) //GPIO0_30

void __iomem *base_addr = NULL;

int init_module(void)
{
    uint32_t reg_data = 0;
	pr_info("Hello world\n");

	base_addr = ioremap(GPIO_LED_BASE, ADDR_SIZE);
    reg_data = readl_relaxed(base_addr + GPIO_OE_OFFSET);
	reg_data &= (~LED);
	writel_relaxed(reg_data, base_addr + GPIO_OE_OFFSET);

    writel_relaxed(LED, base_addr + GPIO_SETDATAOUT_OFFSET);

	return 0;
}

void cleanup_module(void)
{
	pr_info("Goodbye world\n");
    writel_relaxed(LED, base_addr + GPIO_CLEARDATAOUT_OFFSET);
}

MODULE_AUTHOR("Luu An Phu");	/* Who wrote this module? */
MODULE_DESCRIPTION("hello world kernel module");	/* What does this module do */
MODULE_LICENSE("GPL");