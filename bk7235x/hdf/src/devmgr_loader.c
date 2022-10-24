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
