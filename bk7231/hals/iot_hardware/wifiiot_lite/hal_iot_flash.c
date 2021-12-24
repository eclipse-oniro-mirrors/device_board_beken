#include "iot_errno.h"
#include "iot_flash.h"

#include "BkDriverFlash.h"

static PROTECT_TYPE protect_flag;
unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
	bk_flash_abs_addr_read(flashOffset, ramData,size);
	return IOT_SUCCESS;
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size,
                           const unsigned char *ramData, unsigned char doErase)
{
	bk_flash_abs_addr_write(flashOffset,ramData,size,doErase);
	return IOT_SUCCESS;
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
	bk_flash_abs_addr_erase(flashOffset, size);
	return IOT_SUCCESS;
}

unsigned int IoTFlashInit(void)
{
	bk_flash_get_security(&protect_flag);
	bk_flash_enable_security(FLASH_PROTECT_NONE);//or FLASH_PROTECT_HALF
	return IOT_SUCCESS;
}

unsigned int IoTFlashDeinit(void)
{
	bk_flash_enable_security(protect_flag);
	return IOT_SUCCESS;
}


