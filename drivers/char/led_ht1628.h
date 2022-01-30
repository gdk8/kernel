#ifndef __LED_CH455_H__
#define __LED_CH455_H__

#define CH455_IOC_MAGIC 'h'
#define IOCTL_CHAR_DISPLAY 	_IOWR(CH455_IOC_MAGIC, 0x10, unsigned char)
#define IOCTL_DOT_DISPLAY  	_IOWR(CH455_IOC_MAGIC, 0x11, unsigned char)
#define IOCTL_COLON_DISPLAY 	_IOWR(CH455_IOC_MAGIC, 0x12, unsigned char)
#define IOCTL_PWR_DISPLAY	_IOWR(CH455_IOC_MAGIC, 0x13, unsigned char)
#define IOCTL_LAN_DISPLAY	_IOWR(CH455_IOC_MAGIC, 0x14, unsigned char)
#define IOCTL_LAN_OFF	_IOWR(CH455_IOC_MAGIC, 0x15, unsigned char)
#define IOCTL_WIFI_LOW_DISPLAY	_IOWR(CH455_IOC_MAGIC, 0x16, unsigned char)
#define IOCTL_WIFI_FINE_DISPLAY	_IOWR(CH455_IOC_MAGIC, 0x17, unsigned char)
#define IOCTL_WIFI_OFF	_IOWR(CH455_IOC_MAGIC, 0x18, unsigned char)
#define IOCTL_LED_ON		_IOWR(CH455_IOC_MAGIC, 0x19, unsigned char)
#define	IOCTL_LED_OFF		_IOWR(CH455_IOC_MAGIC, 0x1a, unsigned char)

#endif
