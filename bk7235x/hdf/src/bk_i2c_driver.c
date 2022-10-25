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

#define HDF_LOG_TAG i2c_driver

struct I2cResource {
	uint32_t irqNum;
	uint32_t dataRate;
	uint32_t devAddr;
};

struct I2cService {
	struct IDeviceIoService service;

	struct I2cResource resource;
	int (*test)(void);
};

static struct I2cService g_i2cService;

static int GetI2cDeviceResource(struct I2cService *device, const struct DeviceResourceNode *resourceNode)
{
	struct DeviceResourceIface *dri = NULL;
	struct I2cResource *resource;

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

	if (dri->GetUint32(resourceNode, "irqNum", &resource->irqNum, 0) != HDF_SUCCESS) {
        HDF_LOGE("I2c config read base fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "dataRate", &resource->dataRate, 0) != HDF_SUCCESS) {
        HDF_LOGE("I2c config read irqNum fail");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "devAddr", &resource->devAddr, 0) != HDF_SUCCESS) {
        HDF_LOGE("I2c config read baudrate fail");
        return HDF_FAILURE;
    }

	HDF_LOGI("parse resource : irqNum=%d, dataRate=%d, devAddr=%d",
		resource->irqNum, resource->dataRate, resource->devAddr);

	return HDF_SUCCESS;
}

static int32_t I2cDriverBind(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);

	if (deviceObject == NULL) {
		return HDF_FAILURE;
	}

	deviceObject->service = &g_i2cService.service;
	return HDF_SUCCESS;
}

static int32_t I2cDriverInit(struct HdfDeviceObject *deviceObject)
{
	int ret;
	
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);

	if (deviceObject == NULL) {
		return HDF_FAILURE;
	}

	ret = GetI2cDeviceResource(&g_i2cService, deviceObject->property);
    if (ret != HDF_SUCCESS) {
		HDF_LOGE("I2c parse resource failed");
        return HDF_FAILURE;
    }

    HDF_LOGD("%s:Init success", __func__);
	return HDF_SUCCESS;
}

static void I2cDriverRelease(struct HdfDeviceObject *deviceObject)
{
    HDF_LOGD("%s::enter, deviceObject=%p", __func__, deviceObject);
}

struct HdfDriverEntry g_I2cDriverEntry = {
	.moduleVersion = 1,
	.moduleName = "HDF_PLATFORM_I2C_DRIVER",
	.Bind = I2cDriverBind,
	.Init = I2cDriverInit,
	.Release = I2cDriverRelease,
};

HDF_INIT(g_I2cDriverEntry);
