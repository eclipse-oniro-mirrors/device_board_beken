// Copyright (C) 2022 BEKEN .
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "hdf_device_desc.h"
#include "hdf_log.h"
#include "device_resource_if.h"
#include "gpio_core.h"
#include "osal_mem.h"

#include <driver/gpio.h>

#define HDF_LOG_TAG gpio_driver

struct GpioResource {
	uint32_t pinCnt;
	
	uint32_t pin;
	uint32_t realPin;
	uint32_t config;
};

struct GpioConfig {
	uint32_t pin;
	uint32_t config;
};

struct GpioService {
	struct IDeviceIoService service;

	struct GpioResource resource;
	int (*test)(void);
};

#define INVALID_PIN 0xFFF0

#define MAX_GPIO_CNT 10
static struct GpioConfig g_gpioPinMap[MAX_GPIO_CNT];
static int g_gpioCnt;

static struct GpioService g_GpioService;

static struct GpioCntlr *g_gpioCntlr = NULL;

//static uint16_t g_simValues[MAX_GPIO_CNT] = {0};

static uint32_t GpioGetHwPin(uint16_t gpio)
{
	if (gpio >= g_gpioCnt)
		return INVALID_PIN;

	return g_gpioPinMap[gpio].pin;
}

static uint32_t GpioGetPinIdx(uint32_t pin) 
{
	uint32_t i;
	for (i = 0; i < g_gpioCnt; i++) {
		if (g_gpioPinMap[i].pin == pin)
			return i;
	}

	return INVALID_PIN;
}

static int32_t GpioDevWrite(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t val)
{
	HDF_LOGI("call %s %d", __func__, gpio);
	if (gpio >= g_gpioCnt || g_gpioPinMap[gpio].pin == INVALID_PIN) {
		HDF_LOGE("invalid gpio %d\n", gpio);
		return HDF_FAILURE;
	}
	//g_simValues[gpio] = val;
	if (val)
		bk_gpio_set_output_high(GpioGetHwPin(gpio));
	else
		bk_gpio_set_output_low(GpioGetHwPin(gpio));
	return HDF_SUCCESS;
}
static int32_t GpioDevRead(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t *val)
{
	HDF_LOGI("call %s %d", __func__, gpio);
	//*val = g_simValues[gpio];
	*val = bk_gpio_get_input(GpioGetHwPin(gpio));
	return HDF_SUCCESS;
}
static int32_t GpioDevSetDir(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t dir)
{
	HDF_LOGI("call %s", __func__);
	if (dir == GPIO_DIR_IN) {
		bk_gpio_disable_output(GpioGetHwPin(gpio));
		bk_gpio_enable_input(GpioGetHwPin(gpio));
	}
	else {
		bk_gpio_disable_input(GpioGetHwPin(gpio));
		bk_gpio_enable_output(GpioGetHwPin(gpio));
	}
	
	return HDF_SUCCESS;
}
static int32_t GpioDevGetDir(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t *dir)
{
	HDF_LOGI("call %s", __func__);
	// TODO
	*dir = GPIO_DIR_IN;
	return HDF_SUCCESS;
}

static void GpioIsr(gpio_id_t id)
{
	uint32_t local;
	
	local = GpioGetPinIdx(id);
	if (local != INVALID_PIN) {
		GpioCntlrIrqCallback(g_gpioCntlr, local);
	}
}

static int32_t GpioDevSetIrq(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t mode, GpioIrqFunc func, void *arg)
{
	gpio_config_t cfg;
	gpio_int_type_t int_type = 0;
	
	HDF_LOGI("call %s", __func__);

	cfg.io_mode = GPIO_INPUT_ENABLE;
	cfg.func_mode = GPIO_SECOND_FUNC_DISABLE;
	if (mode & GPIO_IRQ_TRIGGER_RISING) {
		int_type = GPIO_INT_TYPE_RISING_EDGE;
		cfg.pull_mode = GPIO_PULL_DOWN_EN;
	} else if (mode & GPIO_IRQ_TRIGGER_FALLING) {
		int_type = GPIO_INT_TYPE_FALLING_EDGE;
		cfg.pull_mode = GPIO_PULL_UP_EN;
	} else if (mode & GPIO_IRQ_TRIGGER_HIGH) {
		int_type = GPIO_INT_TYPE_HIGH_LEVEL;
		cfg.pull_mode = GPIO_PULL_DOWN_EN;
	} else if (mode & GPIO_IRQ_TRIGGER_LOW) {
		int_type = GPIO_INT_TYPE_LOW_LEVEL;
		cfg.pull_mode = GPIO_PULL_UP_EN;
	} else {
		return HDF_FAILURE;
	}

	bk_gpio_set_config(GpioGetHwPin(gpio), &cfg);
	bk_gpio_set_interrupt_type(GpioGetHwPin(gpio), int_type);
	return HDF_SUCCESS;
}
static int32_t GpioDevUnSetIrq(struct GpioCntlr *cntlr, uint16_t gpio)
{
	HDF_LOGI("call %s", __func__);
	return HDF_SUCCESS;
}
static int32_t GpioDevEnableIrq(struct GpioCntlr *cntlr, uint16_t gpio)
{
	HDF_LOGI("call %s", __func__);
	bk_gpio_register_isr(GpioGetHwPin(gpio), GpioIsr);
	bk_gpio_enable_interrupt(GpioGetHwPin(gpio));
	return HDF_SUCCESS;
}
static int32_t GpioDevDisableIrq(struct GpioCntlr *cntlr, uint16_t gpio)
{
	HDF_LOGI("call %s", __func__);
	bk_gpio_disable_interrupt(GpioGetHwPin(gpio));
	bk_gpio_register_isr(GpioGetHwPin(gpio), NULL);
	return HDF_SUCCESS;
}

struct GpioMethod g_gpioMethod = {
    .request = NULL,
    .release = NULL,
    .write = GpioDevWrite,
    .read = GpioDevRead,
    .setDir = GpioDevSetDir,
    .getDir = GpioDevGetDir,
    .toIrq = NULL,
    .setIrq = GpioDevSetIrq,
    .unsetIrq = GpioDevUnSetIrq,
    .enableIrq = GpioDevEnableIrq,
    .disableIrq = GpioDevDisableIrq,
};

static void init_pin_conifgs(void)
{
	int i;

	for (i = 0; i < MAX_GPIO_CNT; i++) {
		g_gpioPinMap[i].pin = INVALID_PIN;
	}
}

static int config_pin(struct GpioConfig *config)
{
	if (config->config == 0) {
		bk_gpio_disable_pull(config->pin);
	} else if (config->config == 1) {
		bk_gpio_pull_up(config->pin);
	} else if (config->config == 2) {
		bk_gpio_pull_down(config->pin);
	} else {
		return HDF_FAILURE;
	}
	return HDF_SUCCESS;
}

static int GetGpioDeviceResource(struct GpioService *device, const struct DeviceResourceNode *resourceNode)
{
	struct DeviceResourceIface *dri = NULL;
	struct GpioResource *resource;
	int ret;

	if (device == NULL || resourceNode == NULL) {
		HDF_LOGE("%s: device is %p, resourceNode is %p", __func__, device, resourceNode);
		return HDF_ERR_INVALID_PARAM;
	}

	init_pin_conifgs();

	resource = &device->resource;

	dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
	if (dri == NULL) {
		HDF_LOGE("DeviceResourceIface is invalid");
        return HDF_ERR_INVALID_OBJECT;
    }

	if (dri->GetUint32(resourceNode, "pinCnt", &resource->pinCnt, 0) != HDF_SUCCESS) {
        HDF_LOGE("Gpio config read pinCnt fail");
        return HDF_FAILURE;
    }

	g_gpioCnt = resource->pinCnt;
	if (g_gpioCnt > MAX_GPIO_CNT) {
		HDF_LOGE("Gpio config pin count %d exceeds %d\n", g_gpioCnt, MAX_GPIO_CNT);
		return HDF_FAILURE;
	}
		

    for (size_t i = 0; i < g_gpioCnt; i++) {
        if (dri->GetUint32ArrayElem(resourceNode, "pin", i, &resource->pin, 0) != HDF_SUCCESS) {
			HDF_LOGE("Gpio config read pin fail");
            return HDF_FAILURE;
        }
        if (dri->GetUint32ArrayElem(resourceNode, "realPin", i, &resource->realPin, 0) != HDF_SUCCESS) {
			HDF_LOGE("Gpio config read realPin fail");
            return HDF_FAILURE;
        }
        if (dri->GetUint32ArrayElem(resourceNode, "config", i, &resource->config, 0) != HDF_SUCCESS) {
			HDF_LOGE("Gpio config read config fail");
            return HDF_FAILURE;
        }
		g_gpioPinMap[resource->pin].pin = resource->realPin;
		g_gpioPinMap[resource->pin].config = resource->config;
		HDF_LOGI("parse gpio %d : pin=%d, realPin=%d, config=%d",
			i, resource->pin, resource->realPin, resource->config);

		ret = config_pin(&g_gpioPinMap[resource->pin]);
		if (ret) {
			HDF_LOGE("Gpio config %d failed\n", i);
			return HDF_FAILURE;
		}
	}

	return HDF_SUCCESS;
}

static int32_t GpioDriverBind(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);

	if (deviceObject == NULL) {
		return HDF_FAILURE;
	}

	deviceObject->service = &g_GpioService.service;
	return HDF_SUCCESS;
}

static int32_t GpioDriverInit(struct HdfDeviceObject *deviceObject)
{
	int ret;
	struct GpioCntlr *gpioCntlr = NULL;
	
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);

	if (deviceObject == NULL) {
		return HDF_FAILURE;
	}

	ret = GetGpioDeviceResource(&g_GpioService, deviceObject->property);
    if (ret != HDF_SUCCESS) {
		HDF_LOGE("gpio parse resourcefail");
        return HDF_FAILURE;
    }

    gpioCntlr = (struct GpioCntlr *)OsalMemCalloc(sizeof(*gpioCntlr));
    if (gpioCntlr == NULL) {
        HDF_LOGE("%s: malloc cntlr fail!", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

	gpioCntlr->device.hdfDev = deviceObject;
	gpioCntlr->start = 20;
	
	gpioCntlr->ops = &g_gpioMethod;
	gpioCntlr->count = g_gpioCnt;
    ret = GpioCntlrAdd(gpioCntlr);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("GpioCntlrAdd fail ret=%d", ret);
        return HDF_FAILURE;
    }

	g_gpioCntlr = gpioCntlr;
    HDF_LOGD("%s:Init success", __func__);
	return HDF_SUCCESS;
}

static void GpioDriverRelease(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);
}

struct HdfDriverEntry g_GpioDriverEntry = {
	.moduleVersion = 1,
	.moduleName = "HDF_PLATFORM_GPIO_DRIVER",
	.Bind = GpioDriverBind,
	.Init = GpioDriverInit,
	.Release = GpioDriverRelease,
};

HDF_INIT(g_GpioDriverEntry);

