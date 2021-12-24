/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: OTA适配实现 (需设备厂商实现)
 */
#include "hilink_open_ota_adapter.h"
#include <stdio.h>
#include <unistd.h>
#include "securec.h"


#define DOWNLOAD_PATITION_ADDR 0x200000   //2048KB
#define DOWNLOAD_PATITION_LEN 0x180000
static unsigned int write_length = 0;
#define HILINK_OK 0
#define HILINK_ERROR -1

/*
 * Flash初始化
 * 返回值是true时，表示初始化正常
 * 返回值是false时，表示初始化异常
 */
bool HILINK_OtaAdapterFlashInit(void)
{
	printf("HILINK_OtaAdapterFlashInit start\r\n");
	IoTFlashInit();
	write_length = 0;
	return true;
}


/*
 * 判断需要升级的分区
 * 返回值是UPGRADE_FW_BIN1时，表示升级固件到分区1
 * 返回值是UPGRADE_FW_BIN2时，表示升级固件到分区2
 */
unsigned int HILINK_OtaAdapterGetUpdateIndex(void)
{
	printf("HILINK_OtaAdapterGetUpdateIndex start\r\n");
    return UPGRADE_FW_BIN1;
}

/*
 * 擦除需要升级的分区
 * size表示需要擦除的分区大小
 * 返回值是ILINK_OK时，表示擦除成功
 * 返回值是HILINK_ERROR时，表示擦除失败
 */
int HILINK_OtaAdapterFlashErase(unsigned int size)
{
	printf("HILINK_OtaAdapterFlashErase start size=%d\r\n",size);
	return IoTFlashErase(DOWNLOAD_PATITION_ADDR,DOWNLOAD_PATITION_LEN);
}

/*
 * 升级数据写入升级的分区
 * buf表示待写入数据
 * bufLen表示待写入数据的长度
 * 返回值是HILINK_OK时，表示写入成功
 * 返回值是HILINK_ERROR时，表示写入失败
 */
int HILINK_OtaAdapterFlashWrite(const unsigned char *buf, unsigned int bufLen)
{
	//printf("HILINK_OtaAdapterFlashWrite start\r\n");
	#if 1
	int ret;
	if (bufLen == 0) 
	{
		return HILINK_OK;
	}
	if (buf == NULL)
	{
		return HILINK_ERROR;
	}
	
	ret = IoTFlashWrite(DOWNLOAD_PATITION_ADDR + write_length, bufLen, buf, 0);

	write_length += bufLen;
	if(ret == 0)
		return HILINK_OK;
	else
		return HILINK_ERROR;
	#else
	return HILINK_OK;
	#endif
}


/*
 * 读取升级分区数据
 * offset表示读写偏移
 * buf表示输出数据的内存地址
 * bufLen表示输出数据的内存长度
 * 返回值是HILINK_OK时，表示读取成功
 * 返回值是HILINK_ERROR时，表示读取失败
 */
int HILINK_OtaAdapterFlashRead(unsigned int offset, unsigned char *buf, unsigned int bufLen)
{
	//printf("HILINK_OtaAdapterFlashRead start\r\n");
	#if 1
    int ret;
	if ( (buf == NULL) || (bufLen == 0)) 
	{
		return HILINK_ERROR;
	}
  ret = IoTFlashRead(DOWNLOAD_PATITION_ADDR + offset, bufLen, buf);

	if(ret == 0)
		return HILINK_OK;
	else
		return HILINK_ERROR;
	#else
	return HILINK_OK;
	#endif
}

/*
 * 分区升级结束
 * 返回值是true时，表示结束正常
 * 返回值是false时，表示结束异常
 */
bool HILINK_OtaAdapterFlashFinish(void)
{
	printf("HILINK_OtaAdapterFlashFinish start\r\n");
	IoTFlashDeinit();
    return true;
}

/* 获取升级区间最大长度 */
unsigned int HILINK_OtaAdapterFlashMaxSize(void)
{
	printf("HILINK_OtaAdapterFlashMaxSize start\r\n");
	
	return DOWNLOAD_PATITION_LEN;
}

/*
 * 根据标志重启模组
 * flag表示重启标志
 * 当flag是RESTART_FLAG_NOW时，表示只有MCU升级时立即重启
 * 当flag是RESTART_FLAG_LATER时，表示有模组时切换分区后再重启
 */
void HILINK_OtaAdapterRestart(int flag)
{
	printf("HILINK_OtaAdapterRestart start\r\n");
 reboot();
    return;
}

int HILINK_OtaStartProcess(int type)
{
	printf("HILINK_OtaStartProcess start type =%d\r\n",type);
    return 0;
}

int HILINK_OtaEndProcess(int status)
{
	printf("HILINK_OtaEndProcess start status=%d\r\n",status);

	
	
	return 0;
}

int HILINK_GetRebootFlag()
{
	printf("HILINK_GetRebootFlag start\r\n");
	return MODULE_CAN_REBOOT;
}

/* 组合升级模式写入数据 */
int HILINK_GroupOtaDataWrite(const unsigned char *buf, unsigned int bufLen,
    unsigned int type, unsigned int curOffset)
{
	printf("HILINK_GroupOtaDataWrite start\r\n");
    return 0;
}

/* 组合升级模式读取数据 */
int HILINK_GroupOtaDataRead(const unsigned char *buf, unsigned int bufLen,
    unsigned int type, unsigned int curOffset)
{
	printf("HILINK_GroupOtaDataRead start\r\n");
    return 0;
}
