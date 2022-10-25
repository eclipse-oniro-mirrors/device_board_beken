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

#define HDF_LOG_TAG uart_driver

struct UartResource {
	uint32_t idx;
	uint32_t base;
	uint32_t irqNum;
	uint32_t baudrate;
	uint32_t uartClk;
	const char *sn;
};

struct UartService {
	struct IDeviceIoService service;

	struct UartResource resource;
	int (*test)(void);
};

static struct UartService g_uartService;

static int GetUartDeviceResource(struct UartService *device, const struct DeviceResourceNode *resourceNode)
{
	struct DeviceResourceIface *dri = NULL;
	struct UartResource *resource;

	if (device == NULL || resourceNode == NULL) {
		HDF_LOGE("%s: device is %p, resourceNode is %p", __func__, device, resourceNode);
		return HDF_ERR_INVALID_PARAM;
	}

	resource = &device->resource;

	dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
	if (dri == NULL) {
		HDF_LOGE("DeviceResourceIface is invalid");
        return HDF_ERR_INVALID_OBJECT;
    }

	if (dri->GetUint32(resourceNode, "idx", &resource->idx, 0) != HDF_SUCCESS) {
        HDF_LOGE("Uart config read idx fail");
        return HDF_FAILURE;
    }

	if (dri->GetUint32(resourceNode, "base", &resource->base, 0) != HDF_SUCCESS) {
        HDF_LOGE("Uart config read base fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "irqNum", &resource->irqNum, 0) != HDF_SUCCESS) {
        HDF_LOGE("Uart config read irqNum fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "baudrate", &resource->baudrate, 0) != HDF_SUCCESS) {
        HDF_LOGE("Uart config read baudrate fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "uartClk", &resource->uartClk, 0) != HDF_SUCCESS) {
        HDF_LOGE("Uart config read uartClk fail");
        return HDF_FAILURE;
    }
	
    if (dri->GetString(resourceNode, "sn", &resource->sn, "no-sn") != HDF_SUCCESS) {
        HDF_LOGE("Uart config read sn fail");
        return HDF_FAILURE;
    }

	HDF_LOGI("parse resource : idx=%d, base=%d, irqNum=%d, baudrate=%d, uartClk=%d, sn=%s",
		resource->idx, resource->base, resource->irqNum, resource->baudrate, resource->uartClk, resource->sn);

	return HDF_SUCCESS;
}

static int32_t UartDriverBind(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGI("%s::enter, deviceObject=%p", __func__, deviceObject);

	if (deviceObject == NULL) {
		return HDF_FAILURE;
	}

	deviceObject->service = &g_uartService.service;
	return HDF_SUCCESS;
}

static int32_t UartDriverInit(struct HdfDeviceObject *deviceObject)
{
	int ret;
	
    HDF_LOGI("%s::enter, deviceObject=%p", __func__, deviceObject);

	if (deviceObject == NULL) {
		return HDF_FAILURE;
	}

	ret = GetUartDeviceResource(&g_uartService, deviceObject->property);
    if (ret != HDF_SUCCESS) {
		HDF_LOGE("Uart parse resource failed");
        return HDF_FAILURE;
    }

    HDF_LOGD("%s:Init success", __func__);
	return HDF_SUCCESS;
}

static void UartDriverRelease(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);
}

struct HdfDriverEntry g_UartDriverEntry = {
	.moduleVersion = 1,
	.moduleName = "HDF_PLATFORM_UART_DRIVER",
	.Bind = UartDriverBind,
	.Init = UartDriverInit,
	.Release = UartDriverRelease,
};

HDF_INIT(g_UartDriverEntry);
