/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/delay.h>

#define GPIO_LED_BASE   0x44E07000
#define ADDR_SIZE   0x1000
#define GPIO_SETDATAOUT_OFFSET  0x194
#define GPIO_CLEARDATAOUT_OFFSET    0x190
#define GPIO_OE_OFFSET  0x134
#define GPIO_DATAOUT    0x13c
#define LED (0x1 << 30) //GPIO0_30

void __iomem *base_addr = NULL;
struct timer_list my_timer;
unsigned int count = 0;

void my_timer_callback(struct timer_list *data)
{

	count++;
	// pr_info("Timer Callback function Called [%d]\n",count++);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

	if (count % 2 == 0) {
		// pr_info("Turn on led\n");
    	writel_relaxed(LED, base_addr + GPIO_SETDATAOUT_OFFSET);
	}
	else {
		// pr_info("Turn off led\n");
    	writel_relaxed(LED, base_addr + GPIO_CLEARDATAOUT_OFFSET);
	}
}

int init_module(void)
{
	int ret;
    uint32_t reg_data = 0;
	pr_info("Hello world\n");

	base_addr = ioremap(GPIO_LED_BASE, ADDR_SIZE);
    reg_data = readl_relaxed(base_addr + GPIO_OE_OFFSET);
	reg_data &= (~LED);
	writel_relaxed(reg_data, base_addr + GPIO_OE_OFFSET);

    writel_relaxed(LED, base_addr + GPIO_SETDATAOUT_OFFSET);
	pr_info("timer module installing\n");

	timer_setup(&my_timer, my_timer_callback, 0);
	printk("Starting timer to fire in 1000ms (%ld)\n", jiffies);
	ret = mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
	if (ret) 
		printk("Error in mod_timer\n");

	return 0;
}

void cleanup_module(void)
{
	int ret;
	
	ret = del_timer(&my_timer);
	if (ret)
		printk("The timer is still in use...\n");
	printk("Timer module uninstalling\n");
	pr_info("Goodbye world\n");
    writel_relaxed(LED, base_addr + GPIO_CLEARDATAOUT_OFFSET);
	
}

MODULE_AUTHOR("Luu An Phu");	/* Who wrote this module? */
MODULE_DESCRIPTION("hello world kernel module");	/* What does this module do */
MODULE_LICENSE("GPL");