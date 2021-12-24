#include "hal_hota_board.h"
#include <stdio.h>

int HotaHalInit(void)
{
    return OHOS_SUCCESS;
}

int HotaHalGetUpdateIndex(unsigned int *index)
{
	if (!index)
		return OHOS_FAILURE;
	
	*index = 1;	//or 2
    return OHOS_SUCCESS;
}

int HotaHalDeInit(void)
{
    return  OHOS_FAILURE;//OHOS_SUCCESS;
}

int HotaHalRead(HotaPartition partition, unsigned int offset, unsigned int bufLen, unsigned char *buffer)
{
    if ((buffer == NULL) || (bufLen == 0)) {
        return OHOS_FAILURE;
    }

	return OHOS_FAILURE;//OHOS_SUCCESS
}

int HotaHalWrite(HotaPartition partition, unsigned char *buffer, unsigned int offset, unsigned int bufLen)
{
    if ((buffer == NULL) || (bufLen == 0)) {
        return OHOS_FAILURE;
    }

    return OHOS_SUCCESS;
}

int HotaHalRestart(void)
{
    return OHOS_SUCCESS;
}

int HotaHalSetBootSettings(void)
{
    return OHOS_SUCCESS;
}

int HotaHalRollback(void)
{
    return OHOS_SUCCESS;
}
