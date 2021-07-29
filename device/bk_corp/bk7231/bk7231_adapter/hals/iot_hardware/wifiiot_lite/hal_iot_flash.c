#include "iot_errno.h"
#include "iot_flash.h"

#include "BkDriverFlash.h"


unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
	liteos_flash_read(flashOffset, ramData,size);
	return IOT_SUCCESS;
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size,
                           const unsigned char *ramData, unsigned char doErase)
{
	liteos_flash_write(flashOffset,ramData,size,doErase);
	return IOT_SUCCESS;
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
	liteos_flash_erase(flashOffset, size);
	return IOT_SUCCESS;
}

unsigned int IoTFlashInit(void)
{
	return IOT_SUCCESS;
}

unsigned int IoTFlashDeinit(void)
{
	return IOT_SUCCESS;
}


