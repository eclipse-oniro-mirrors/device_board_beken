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
#include "ohos_init.h"
#include "devmgr_service_start.h"
#include "hdf_log.h"

static void DeviceManagerInit(void)
{
	int ret;

    HDF_LOGD("%s enter", __func__);
    ret = DeviceManagerStart();
    if (ret < 0) {
        HDF_LOGE("%s start failed %d", __func__, ret);
    } else {
        HDF_LOGD("%s start success", __func__);
    }
}

CORE_INIT(DeviceManagerInit);
