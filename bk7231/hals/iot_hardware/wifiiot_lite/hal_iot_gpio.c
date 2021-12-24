#include "iot_errno.h"
#include "iot_gpio.h"
#include "gpio_pub.h"

#define GPIO_PIN_INIT    1
#define GPIO_PIN_UNINIT  0

static unsigned char g_gpioMap[GPIONUM] = {0};
static unsigned char g_gpioInitNum = 0;

#define GPIO_CHECK(gpio) do { if ((gpio) >= GPIONUM) return IOT_FAILURE; } while(0)

unsigned int IoTGpioInit(unsigned int id)
{
    GPIO_CHECK(id);

    if (g_gpioInitNum == 0) {
        //(void)hi_gpio_init();
    }

    if (g_gpioMap[id] == GPIO_PIN_INIT) {
        return IOT_FAILURE;
    } else {
        g_gpioMap[id] = GPIO_PIN_INIT;
        g_gpioInitNum++;
    }

    return IOT_SUCCESS;
}

unsigned int IoTGpioDeinit(unsigned int id)
{
    GPIO_CHECK(id);

    if (g_gpioMap[id] == GPIO_PIN_INIT) {
        g_gpioInitNum--;
        g_gpioMap[id] = GPIO_PIN_UNINIT;
    } else {
        return IOT_FAILURE;
    }

    if (g_gpioInitNum == 0) {
        //return hi_gpio_deinit();
        return IOT_SUCCESS;
    } else {
        return IOT_SUCCESS;
    }
}

unsigned int IoTGpioSetDir(unsigned int id, IotGpioDir dir)
{
	unsigned int ret;
	
    GPIO_CHECK(id);

	switch (dir) {
		case IOT_GPIO_DIR_IN : 
			bk_gpio_config_input(id);
			ret = IOT_SUCCESS;
			break;
			
		case IOT_GPIO_DIR_OUT : 
			bk_gpio_config_output(id);
			ret = IOT_SUCCESS;
			break;
			
		default : 
			ret = IOT_FAILURE;
			break;
	}
	return ret;
}

unsigned int IoTGpioGetDir(unsigned int id, IotGpioDir *dir)
{
	unsigned int value = 0;

    GPIO_CHECK(id);

	if (!dir)
		return IOT_FAILURE;

	gpio_get_dir(id, &value);	//new
	*dir = value ? IOT_GPIO_DIR_OUT : IOT_GPIO_DIR_IN;
	return IOT_SUCCESS;
}

unsigned int IoTGpioSetOutputVal(unsigned int id, IotGpioValue val)
{
    GPIO_CHECK(id);

	gpio_output(id, val);
	return IOT_SUCCESS;
}

unsigned int IoTGpioGetOutputVal(unsigned int id, IotGpioValue *val)
{
	unsigned int value = 0;
	
    GPIO_CHECK(id);

	if (!val)
		return IOT_FAILURE;

	gpio_get_output(id, &value);	//new
	*val = value ? IOT_GPIO_VALUE1 : IOT_GPIO_VALUE0;
	return IOT_SUCCESS;
}

unsigned int IoTGpioGetInputVal(unsigned int id, IotGpioValue *val)
{
	unsigned int value;
	
    GPIO_CHECK(id);

	if (!val)
		return IOT_FAILURE;

	value = gpio_input(id);
	*val = value ? IOT_GPIO_VALUE1 : IOT_GPIO_VALUE0;
	return IOT_SUCCESS;
}

struct gpio_isr_adapter {
	GpioIsrCallbackFunc func;
	char * arg;
};

static struct gpio_isr_adapter gpio_isr_adapters[GPIONUM] = {
	[ 0 ... (GPIONUM - 1) ] = {NULL, NULL}
};

static void gpio_default_handler(unsigned char gpio_id)
{
	if (gpio_id >= GPIONUM)
		return;
	
	if (gpio_isr_adapters[gpio_id].func)
		gpio_isr_adapters[gpio_id].func(gpio_isr_adapters[gpio_id].arg);
}

static unsigned int combine_isr_mode(IotGpioIntType intType, IotGpioIntPolarity intPolarity)
{
	unsigned int mode;
	
	if (intType == IOT_INT_TYPE_LEVEL) {
		mode = intPolarity ? 1 : 0;
	} else {
		mode = intPolarity ? 2 : 3;
	}

	return mode;
}

unsigned int IoTGpioRegisterIsrFunc(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity,
                                    GpioIsrCallbackFunc func, char *arg)
{
	unsigned int mode;

    GPIO_CHECK(id);

	mode = combine_isr_mode(intType, intPolarity);
	gpio_isr_adapters[id].func = func;
	gpio_isr_adapters[id].arg = arg;
	gpio_int_enable(id, mode, gpio_default_handler);
	return IOT_SUCCESS;
}

unsigned int IoTGpioUnregisterIsrFunc(unsigned int id)
{
    GPIO_CHECK(id);

	gpio_int_disable(id);
	return IOT_SUCCESS;
}

unsigned int IoTGpioSetIsrMask(unsigned int id, unsigned char mask)
{
    GPIO_CHECK(id);

	gpio_int_mask(id, mask);	//new
	return IOT_SUCCESS;
}

unsigned int IoTGpioSetIsrMode(unsigned int id, IotGpioIntType intType, IotGpioIntPolarity intPolarity)
{
    GPIO_CHECK(id);

	if (gpio_isr_adapters[id].func)
		return IoTGpioRegisterIsrFunc(id, intType, intPolarity, gpio_isr_adapters[id].func, gpio_isr_adapters[id].arg);
	else
		return IOT_FAILURE;
}

