/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/gpio.h> 		/* For Legacy integer based GPIO */
#include <linux/interrupt.h>	/* For IRQ */
#include <linux/delay.h>	

#define GPIO_LED_BASE   0x44E07000
#define ADDR_SIZE   0x1000
#define GPIO_SETDATAOUT_OFFSET  0x194
#define GPIO_CLEARDATAOUT_OFFSET    0x190
#define GPIO_OE_OFFSET  0x134
#define GPIO_DATAOUT    0x13c
#define LED (0x1 << 30) //GPIO0_30
#define LED_PIN 30
#define BUTTON_PIN 31
void __iomem *base_addr = NULL;
struct timer_list my_timer;
unsigned int count = 0;
int button_irq = 0;
static int irq;
volatile int32_t state;




static irq_handler_t m_btn_irq_handle(unsigned int irq, void *dev_id, struct pt_regs *regs){
	
  	state = gpio_get_value(LED_PIN);
	if (state == 0) {		
		writel_relaxed(LED, base_addr + GPIO_SETDATAOUT_OFFSET);
		state = 1;
	} else {
		writel_relaxed(LED, base_addr + GPIO_CLEARDATAOUT_OFFSET);
		state = 0;
	}
	return (irq_handler_t) IRQ_HANDLED;
}

int init_module(void)
{

    uint32_t reg_data = 0;
	uint8_t ret_val;
	pr_info("Hello world\n");

	base_addr = ioremap(GPIO_LED_BASE, ADDR_SIZE);
    reg_data = readl_relaxed(base_addr + GPIO_OE_OFFSET);
	reg_data &= (~LED);
	writel_relaxed(reg_data, base_addr + GPIO_OE_OFFSET);

    writel_relaxed(LED, base_addr + GPIO_SETDATAOUT_OFFSET);
	

	gpio_direction_input(BUTTON_PIN);        // Set the button GPIO to be an input
	gpio_set_debounce(BUTTON_PIN, 200); // Debounce the button with a delay of 200ms
	gpio_export(BUTTON_PIN, false);

	irq = gpio_to_irq(BUTTON_PIN);	// The interrupt number requested 
	ret_val = request_irq(irq,(irq_handler_t) m_btn_irq_handle,IRQF_TRIGGER_FALLING,"m-button",NULL);                             
	pr_info("On/Off led using button program loaded\n");
	return 0;

	return 0;
}

void cleanup_module(void)
{
	int ret;
	free_irq(irq, NULL);
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