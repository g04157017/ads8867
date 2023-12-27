#ifndef __LED_H_
#define __LED_H_

#define SYSFS_GPIO_EXPORT           "/sys/class/gpio/export"  
#define SYSFS_GPIO_UNEXPORT			"/sys/class/gpio/unexport"

//led2引脚控制
#define SYSFS_BUSY_GPIO5_IO6_PIN	"134"
#define SYSFS_BUSY_GPIO5_IO6_PIN_DIR	"/sys/class/gpio/gpio134/direction"
#define SYSFS_BUSY_GPIO5_IO6_PIN_VAL	"/sys/class/gpio/gpio134/value"
#define SYSFS_BUSY_GPIO5_IO6_PIN_DIR_VAL	"out"
#define SYSFS_RST_GPIO5_IO6_PIN_H		"1"
#define SYSFS_RST_GPIO5_IO6_PIN_L		"0"

//led3引脚控制
#define SYSFS_RST_GPIO5_IO8_PIN	"136"
#define SYSFS_RST_GPIO5_IO8_PIN_DIR	"/sys/class/gpio/gpio136/direction"
#define SYSFS_RST_GPIO5_IO8_PIN_VAL	"/sys/class/gpio/gpio136/value"
#define SYSFS_RST_GPIO5_IO8_PIN_DIR_VAL	"out"
#define SYSFS_RST_GPIO5_IO8_PIN_H		"1"
#define SYSFS_RST_GPIO5_IO8_PIN_L		"0"

//led4引脚控制
#define SYSFS_RST_GPIO4_IO16_PIN	"112"
#define SYSFS_RST_GPIO4_IO16_PIN_DIR	"/sys/class/gpio/gpio112/direction"
#define SYSFS_RST_GPIO4_IO16_PIN_VAL	"/sys/class/gpio/gpio112/value"
#define SYSFS_RST_GPIO4_IO16_PIN_DIR_VAL	"out"
#define SYSFS_RST_GPIO4_IO16_PIN_H		"1"
#define SYSFS_RST_GPIO4_IO16_PIN_L		"0"

//led5引脚控制
#define SYSFS_RST_GPIO5_IO4_PIN	"132"
#define SYSFS_RST_GPIO5_IO4_PIN_DIR	"/sys/class/gpio/gpio132/direction"
#define SYSFS_RST_GPIO5_IO4_PIN_VAL	"/sys/class/gpio/gpio132/value"
#define SYSFS_RST_GPIO5_IO4_PIN_DIR_VAL	"out"
#define SYSFS_RST_GPIO5_IO4_PIN_H		"1"
#define SYSFS_RST_GPIO5_IO4_PIN_L		"0"

void led_pin_init();//初始化led引脚配置
void led2_open();
void led2_close();
void led3_open();
void led3_close();
void led4_open();
void led4_close();
void led5_open();
void led5_close();

#endif