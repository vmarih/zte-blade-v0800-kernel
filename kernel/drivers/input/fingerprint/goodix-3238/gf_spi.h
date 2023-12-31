#ifndef __GF_SPI_H
#define __GF_SPI_H

#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/wakelock.h> //Alan, add wakelock
/**********************************************************/
enum FP_MODE{
	GF_IMAGE_MODE = 0,
	GF_KEY_MODE,
	GF_SLEEP_MODE,
	GF_FF_MODE,
	GF_DEBUG_MODE = 0x56
};

typedef enum gf_key_event {
    GF_KEY_NONE = 0,
    GF_KEY_HOME,
    GF_KEY_POWER,
    GF_KEY_MENU,
    GF_KEY_BACK,
    GF_KEY_CAPTURE,
    GF_KEY_UP,
    GF_KEY_DOWN,
    GF_KEY_RIGHT,
    GF_KEY_LEFT,
    GF_KEY_TAP,
    GF_KEY_HEAVY,
    GF_KEY_LONG_PRESS,
    GF_KEY_DOUBLE_TAP
} gf_key_event_t;

struct gf_key {
    enum gf_key_event key;
    uint32_t value; /* key down = 1, key up = 0 */
};

struct gf_key_map {
    char *name;
    unsigned short val;
};

struct gf_ioc_chip_info {
    unsigned char vendor_id;
    unsigned char mode;
    unsigned char operation;
    unsigned char reserved[5];
};

#define GF_IOC_MAGIC    'g'     //define magic number
#define GF_IOC_INIT             _IOR(GF_IOC_MAGIC, 0, uint8_t)
#define GF_IOC_EXIT             _IO(GF_IOC_MAGIC, 1)
#define GF_IOC_RESET            _IO(GF_IOC_MAGIC, 2)
#define GF_IOC_ENABLE_IRQ       _IO(GF_IOC_MAGIC, 3)
#define GF_IOC_DISABLE_IRQ      _IO(GF_IOC_MAGIC, 4)
#define GF_IOC_ENABLE_SPI_CLK   _IO(GF_IOC_MAGIC, 5)
#define GF_IOC_DISABLE_SPI_CLK  _IO(GF_IOC_MAGIC, 6)
#define GF_IOC_ENABLE_POWER     _IO(GF_IOC_MAGIC, 7)
#define GF_IOC_DISABLE_POWER    _IO(GF_IOC_MAGIC, 8)
#define GF_IOC_INPUT_KEY_EVENT  _IOW(GF_IOC_MAGIC, 9, struct gf_key)
#define GF_IOC_ENTER_SLEEP_MODE _IO(GF_IOC_MAGIC, 10)
#define GF_IOC_GET_FW_INFO      _IOR(GF_IOC_MAGIC, 11, uint8_t)
#define GF_IOC_REMOVE           _IO(GF_IOC_MAGIC, 12)
#define GF_IOC_CHIP_INFO        _IOR(GF_IOC_MAGIC, 13, struct gf_ioc_chip_info)

#define  GF_IOC_MAXNR    14  /* THIS MACRO IS NOT USED NOW... */

//#define AP_CONTROL_CLK       1
#define  USE_PLATFORM_BUS     1
//#define  USE_SPI_BUS	1
//#define GF_FASYNC   1	/*If support fasync mechanism.*/
#define GF_NETLINK_ENABLE 1
#define GF_NET_EVENT_IRQ 1
#define GF_NET_EVENT_FB_BLACK 2
#define GF_NET_EVENT_FB_UNBLACK 3
#define NETLINK_TEST 25

struct gf_dev {
	dev_t devt;
	struct list_head device_entry;
#if defined(USE_SPI_BUS)
	struct spi_device *spi;
#elif defined(USE_PLATFORM_BUS)
	struct platform_device *spi;
#endif
	struct clk *core_clk;
	struct clk *iface_clk;

	struct input_dev *input;
	/* buffer is NULL unless this device is open (users > 0) */
	unsigned users;
	signed irq_gpio;
	signed reset_gpio;
	signed pwr3v3_gpio;//added by chenhui for 3v3 pwr
	signed pwrvdd_gpio;//added by chenhui for vdd pwr
	int irq;
	int irq_enabled;
	int clk_enabled;
#ifdef GF_FASYNC
	struct fasync_struct *async;
#endif
	struct notifier_block notifier;
	char device_available;
	char fb_black;
	struct wake_lock ttw_wl; //Alan, add wakelock
};

int gf_parse_dts(struct gf_dev* gf_dev);
void gf_cleanup(struct gf_dev *gf_dev);

int gf_power_on(struct gf_dev *gf_dev);
int gf_power_off(struct gf_dev *gf_dev);

int gf_hw_reset(struct gf_dev *gf_dev, unsigned int delay_ms);
int gf_irq_num(struct gf_dev *gf_dev);

void sendnlmsg(char *message);
int netlink_init(void);
void netlink_exit(void);
#endif /*__GF_SPI_H*/
