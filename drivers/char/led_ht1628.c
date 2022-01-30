#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/jiffies.h> 
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sysctl.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/workqueue.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/uaccess.h>

#include "led_ht1628.h"
#define LCD_EXT_DEBUG_INFO
#ifdef LCD_EXT_DEBUG_INFO
#define DBG_PRINT(...)		printk(__VA_ARGS__)
#else
#define DBG_PRINT(...)
#endif
#define DEVICE_NAME  "LED_HT1628"
#define CLR_LCD_CS		gpio_set_value(gpio_spi_cs,0) 
#define SET_LCD_CS		gpio_set_value(gpio_spi_cs,1)
#define CLR_LCD_WR		gpio_set_value(gpio_spi_clk,0)
#define SET_LCD_WR		gpio_set_value(gpio_spi_clk,1)
#define CLR_LCD_DATA	gpio_set_value(gpio_spi_data,0)
#define SET_LCD_DATA	gpio_set_value(gpio_spi_data,1)
#define DELAY_VALUE 10
//�ַ���ʾ
static unsigned char display_num[]={0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};//0,1,2,3,4,5,6,7,8,9
static unsigned char display_letter[]={0x77,0x7C,0x39,0x5E,0x79,0x71,0x7D,0x78,0x38,0x3E}; //a,b,c,d,e,f,g,t,L
static unsigned char HT1628_DIG_REG[]={0xC0,0xC2,0xC4,0xC6,0xC8};
static unsigned char char_position[4]={0};
static unsigned char current_display[4] = {0};
static unsigned char symbol_display = 0;

static int gpio_spi_cs,gpio_spi_clk,gpio_spi_data; //gpio_typec_pd,gpio_typec_sel;

static void LED_DATA_SEND(uint8_t data)
{
	int i;
	for( i=0; i<8; i++ ) {
		udelay(DELAY_VALUE);
		CLR_LCD_WR;
		if(data & 0x1) {
			SET_LCD_DATA;
		}
		else {
			CLR_LCD_DATA;
		}
		data>>=1;
		udelay(DELAY_VALUE);
		SET_LCD_WR;
	}
	udelay(DELAY_VALUE);
}

static void LED_COMM_SEND(uint8_t data)
{
	SET_LCD_CS;
	udelay(DELAY_VALUE);
	CLR_LCD_CS;
	LED_DATA_SEND(data);
}

static void show_gedu(void)
{
	LED_COMM_SEND(HT1628_DIG_REG[0]);
	LED_DATA_SEND(display_letter[6]);		//G
	LED_COMM_SEND(HT1628_DIG_REG[1]);
	LED_DATA_SEND(display_letter[4]);		//E
	LED_COMM_SEND(HT1628_DIG_REG[2]);
	LED_DATA_SEND(display_num[0]);		  //0 as D
	LED_COMM_SEND(HT1628_DIG_REG[3]);
	LED_DATA_SEND(display_letter[9]);		//U
}

static int HT1628_init(void)
{
	int i,ret = 0;
	//��ʼ��
	LED_COMM_SEND(0x03);   // 7λ10��
	LED_COMM_SEND(0x40);   // ��ַ�Զ�����ģʽ
	LED_COMM_SEND(0xC0);
	for (i=0;i<8 ;i++ )
	{
		LED_DATA_SEND(0x0);
		LED_DATA_SEND(0x0);
	}
	LED_COMM_SEND(0x44);   // ��ַ���óɹ̶�ģʽ
	LED_COMM_SEND(0x8f);
	/*power on display boot*/		
	LED_COMM_SEND(0xC8);						//power on
	symbol_display |= 0x04|0x69;
	LED_DATA_SEND(symbol_display);
	
	show_gedu();

	/*LED_COMM_SEND(HT1628_DIG_REG[0]);		
//	LED_DATA_SEND(display_letter[1]);		//b
	LED_DATA_SEND(display_letter[8]);		//L
	
	LED_COMM_SEND(HT1628_DIG_REG[1]);
	LED_DATA_SEND(display_num[0]);			//0
	
	LED_COMM_SEND(HT1628_DIG_REG[2]);
//	LED_DATA_SEND(display_num[0]);			//0
	LED_DATA_SEND(display_letter[0]);			//a
	
	
	LED_COMM_SEND(HT1628_DIG_REG[3]);
//	LED_DATA_SEND(display_letter[7]);		//t
	LED_DATA_SEND(display_letter[3]);*/		//d
	/***************/

	printk("HT1628_init success\n");
	return ret;
}

static int HT1628_open(struct inode *inode, struct file *file) 
{                                                                                                                                   
	int ret;                                                                                                                  
	ret = nonseekable_open(inode, file);                                                                                       
	if(ret < 0)                                                                                                          
		return ret;            
	return 0;                                                                                                     
}

static int HT1628_release(struct inode *inode, struct file *file) 
{
	return 0;
}

static long HT1628_ioctl(struct file *file, unsigned int cmd, unsigned long args) 
{
	int ret = 0 , i=0,pos_temp=0;
	unsigned char dot_position = 0;
	void __user *argp = (void __user *)args;
	unsigned char display_arg[4]={0};
	switch (cmd){
		case IOCTL_CHAR_DISPLAY:
			if (args) {  
				ret = copy_from_user(display_arg, argp,sizeof(display_arg)/sizeof(display_arg[0]));
			}
			for(i=0;i<4;i++)
			{
				if(display_arg[i] <= '9' && display_arg[i] >= '0')
				{
					char_position[i] = display_arg[i] - '0';
					pos_temp = char_position[i];  
					LED_COMM_SEND(HT1628_DIG_REG[i]);
					LED_DATA_SEND(display_num[pos_temp]);
					current_display[i] = display_num[pos_temp];
				}else if(display_arg[i] <= 'z' && display_arg[i] >= 'a'){	
					char_position[i] = display_arg[i] - 'a';
					pos_temp = char_position[i];  
					LED_COMM_SEND(HT1628_DIG_REG[i]);
					LED_DATA_SEND(display_letter[pos_temp]);
					current_display[i] = display_letter[pos_temp];
				}else if(display_arg[i] <= 'Z' && display_arg[i] >= 'A'){
					char_position[i] = display_arg[i] - 'A';
					pos_temp = char_position[i];  
					LED_COMM_SEND(HT1628_DIG_REG[i]);
					LED_DATA_SEND(display_letter[pos_temp]);
					current_display[i] = display_letter[pos_temp];
				}
			}
			break;
		case IOCTL_DOT_DISPLAY:
			if (args) {  
				ret = copy_from_user(&dot_position, argp,1);
			}
			//LED_COMM_SEND(HT1628_DIG_REG[dot_position-1]);
			//LED_DATA_SEND(current_display[dot_position]|0x80);
			break;
		case IOCTL_COLON_DISPLAY:
			LED_COMM_SEND(0xC8);
			symbol_display |= 0x90;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_PWR_DISPLAY:
			LED_COMM_SEND(0xC8);
			symbol_display |= 0x04;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_LAN_DISPLAY:
			LED_COMM_SEND(0xC8);
			symbol_display |= 0x09;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_LAN_OFF:
			LED_COMM_SEND(0xC8);
			symbol_display &= 0xf7;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_WIFI_LOW_DISPLAY:
			LED_COMM_SEND(0xC8);
			symbol_display &= 0xdf;
			symbol_display |= 0x40;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_WIFI_FINE_DISPLAY:
			LED_COMM_SEND(0xC8);
			symbol_display |= 0x60;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_WIFI_OFF:
			LED_COMM_SEND(0xC8);
			symbol_display &= 0x9f;
			LED_DATA_SEND(symbol_display);
			break;
		case IOCTL_LED_ON:
			LED_COMM_SEND(0x8f);
			break;
		case IOCTL_LED_OFF:
			LED_COMM_SEND(0x87);
			break;
		default:
			printk("ERROR: IOCTL CMD NOT FOUND!!!\n");
			break;
	}
	return 0;
}
static struct file_operations HT1628_fops ={
	.owner =THIS_MODULE,
	.open  =HT1628_open,
	.release =HT1628_release,
	.unlocked_ioctl =HT1628_ioctl,
};



static int HT1628_probe(struct platform_device *pdev)
{
	static struct class * scull_class;
	struct device_node *node = pdev->dev.of_node;
	enum of_gpio_flags flags;
	int ret; 

	ret =register_chrdev(0,DEVICE_NAME,&HT1628_fops);
	if(ret<0){
		printk("can't register device lcd_HT1628.\n");
		return ret;
	}
	printk("register device lcd_HT1628 success.\n");

	scull_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(scull_class))
	{
		printk(KERN_ALERT "Err:faile in scull_class!\n");
		return -1;
	}
	device_create(scull_class, NULL, MKDEV(ret,0), NULL, DEVICE_NAME);


	//---------------------------
	gpio_spi_cs = of_get_named_gpio_flags(node, "spi_cs", 0, &flags);
	if (gpio_is_valid(gpio_spi_cs)){
		if (gpio_request(gpio_spi_cs, "spi_cs_gpio")<0) {
			printk("%s: failed to get gpio_spi_cs.\n", __func__);
			return -1;
		}
		gpio_direction_output(gpio_spi_cs, 1);
		printk("%s: get property: gpio,spi_cs = %d\n", __func__, gpio_spi_cs);
	}else{
		printk("get property gpio,spics failed \n");
		return -1;
	}

	gpio_spi_clk = of_get_named_gpio_flags(node, "spi_clk", 0, &flags);
	
	if (gpio_is_valid(gpio_spi_clk)){
		if (gpio_request(gpio_spi_clk, "spi_clk_gpio")<0) {
			printk("%s: failed to get gpio_spi_clk.\n", __func__);
			return -1;
		}
		gpio_direction_output(gpio_spi_clk, 1);
		printk("%s: get property: gpio,spi_clk = %d\n", __func__, gpio_spi_clk);
	}else{
		printk("get property gpio,spiclk failed \n");
		return -1;
	} 

	gpio_spi_data = of_get_named_gpio_flags(node, "spi_data", 0, &flags);
	if (gpio_is_valid(gpio_spi_data)){
		if (gpio_request(gpio_spi_data, "spi_data_gpio")<0) {
			printk("%s: failed to get gpio_spi_data.\n", __func__);
			return -1;
		}
		gpio_direction_output(gpio_spi_data, 1);
		printk("%s: get property: gpio,spi_data = %d\n", __func__, gpio_spi_data);
	}else{
		printk("get property gpio,spidata failed \n");
		return -1;
	} 
	
	//		printk("gpio_spi_cs====%d\n", gpio_spi_cs);
	//		printk("gpio_spi_clk====%d\n", gpio_spi_clk);
		//	printk("gpio_spi_data====%d\n", gpio_spi_data);
//========for test typeC=================	
#if 0
	gpio_typec_pd = of_get_named_gpio_flags(node, "tpyec_pd", 0, &flags);
	if (gpio_is_valid(gpio_typec_pd)){
		gpio_direction_output(gpio_typec_pd, 0);
		printk("%s: get property: gpio,spi_data = %d\n", __func__, gpio_typec_pd);
	}else{
		printk("get property gpio,spidata failed \n");
		return -1;
	} 
	
	gpio_typec_sel = of_get_named_gpio_flags(node, "tpyec_sel", 0, &flags);
	if (gpio_is_valid(gpio_typec_sel)){
		gpio_direction_output(gpio_typec_sel, 0);
		printk("%s: get property: gpio,spi_data = %d\n", __func__, gpio_typec_sel);
	}else{
		printk("get property gpio,spidata failed \n");
		return -1;
	} 
#endif
	
	printk("==========%s probe ok========\n", DEVICE_NAME);

	ret = HT1628_init();	
	if(ret < 0)
		return -1;


	return 0;
}

static int gpio_led_suspend(struct device *dev)
{
	LED_COMM_SEND(HT1628_DIG_REG[0]);
	LED_DATA_SEND(display_num[7]);			//0
	LED_COMM_SEND(HT1628_DIG_REG[1]);
	LED_DATA_SEND(display_num[7]);			//0
	LED_COMM_SEND(HT1628_DIG_REG[2]);
	LED_DATA_SEND(display_num[7]);			//0
	LED_COMM_SEND(HT1628_DIG_REG[3]);
	LED_DATA_SEND(display_num[7]);			//0
	printk("==============%s: display 8.\n", __func__);
	return 0;
	//return led_mode_request(to_platform_device(dev), LED_M_SUSPEND);
}

static int gpio_led_resume(struct device *dev)
{
	LED_COMM_SEND(HT1628_DIG_REG[0]);
	LED_DATA_SEND(display_num[7]);			//0
	LED_COMM_SEND(HT1628_DIG_REG[1]);
	LED_DATA_SEND(display_num[7]);			//0
	LED_COMM_SEND(HT1628_DIG_REG[2]);
	LED_DATA_SEND(display_num[7]);			//0
	LED_COMM_SEND(HT1628_DIG_REG[3]);
	LED_DATA_SEND(display_num[7]);			//0
	
	printk("==============%s: display 8.\n", __func__);
	return 0;
	//return led_mode_request(to_platform_device(dev), LED_M_RESUME);
}

static SIMPLE_DEV_PM_OPS(gpio_led_pm_ops, gpio_led_suspend, gpio_led_resume);
//#define GPIO_LED_PM_OPS (&gpio_led_pm_ops)

static int HT1628_remove(struct platform_device *pdev)
{
	unregister_chrdev(0,DEVICE_NAME);
	return 0;
}


static void HT1628_shutdown (struct platform_device *pdev)
{
		LED_COMM_SEND(0x87);  //close led
}

#ifdef CONFIG_OF
static const struct of_device_id HT1628_dt_match[]={
	{ .compatible = "skykirin-ht1628",},
	{}
};
MODULE_DEVICE_TABLE(of, HT1628_dt_match);
#endif

static struct platform_driver HT1628_driver = {
	.probe  = HT1628_probe,
	.remove = HT1628_remove,
  .shutdown     = HT1628_shutdown,
	.driver = {
		.name  = DEVICE_NAME,
		.owner = THIS_MODULE,
		.pm     = &gpio_led_pm_ops,
#ifdef CONFIG_OF
		.of_match_table = of_match_ptr(HT1628_dt_match),
#endif
	},
};


static int __init led_HT1628_init(void)
{
	int ret;
	DBG_PRINT("%s\n=============================================\n", __FUNCTION__);
	ret = platform_driver_register(&HT1628_driver);
	if (ret) {
		printk("[error] %s failed to register HT1628 driver module\n", __FUNCTION__);
		return -ENODEV;
	}
	return ret;
}

static void __exit led_HT1628_exit(void)
{
	platform_driver_unregister(&HT1628_driver);
}

module_init(led_HT1628_init);
module_exit(led_HT1628_exit);

MODULE_AUTHOR("Skykirin");
MODULE_DESCRIPTION("LCD Extern driver for LED_HT1628");
MODULE_LICENSE("GPL");
