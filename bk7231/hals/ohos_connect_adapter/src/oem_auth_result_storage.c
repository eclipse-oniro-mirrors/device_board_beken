/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: implement of oem_auth_result_storage
 * Author: Kit Framework group
 * Create: 2020-12-26
 */

#include "oem_auth_result_storage.h"

#include <stdio.h>
#include "securec.h"

#include "utils_file.h"

#define RESET_FILE_NAME "auth_reset"
#define TICKET_FILE_NAME "auth_ticket"
#define AUTH_STATS_FILE_NAME "auth_authstats"

#define WRITE_FLASH_MAX_TEMPERATURE 80
static const uint32_t MAX_FILE_BYTES_LIMIT = 5120;

bool OEMIsOverTemperatureLimit(void)//???ии?D?ж╠?D??
{
    return false;
}

static bool IsFileExist(const char* path)
{
	printf("IsFileExist start path=%s\r\n",path);
	if(path == NULL)
		return false;
	int32_t fd = UtilsFileOpen(path, O_RDONLY_FS, 0);
	if(fd < 0)
	{
		return false;
	}
	(void)UtilsFileClose(fd);
	return true;
}

static int32_t CreateFile(const char* path)
{
	printf("CreateFile start path=%s\r\n",path);
	if(path == NULL)
		return false;
	int32_t fd = UtilsFileOpen(path, O_CREAT_FS, 0);
	if(fd < 0)
	{
		return -1;
	}
	(void)UtilsFileClose(fd);
	return 0;
}


static int32_t WriteFile(const char* path,const char* data,uint32_t datalen,int flag)
{
	printf("WriteFile start path=%s\r\n",path);
	if(path == NULL || data == NULL || datalen == 0)
	{
		printf("WriteFile data invalid\r\n");
		return -1;
	}
	if(datalen > MAX_FILE_BYTES_LIMIT)
	{
		printf("WriteFile data over limit\r\n");
		return -1;
	}

	if(OEMIsOverTemperatureLimit())
	{
		return -1;
	}
	int32_t fd = UtilsFileOpen(path, flag, 0);
	if(fd < 0)
	{
		printf("WriteFile open failed\r\n");
		return -1;
	}

	int32_t ret = 0;
	if(UtilsFileWrite(fd, data, datalen) != datalen)
	{
		printf("WriteFile write failed\r\n");
		return -1;
	}

	(void)UtilsFileClose(fd);
	return ret;
}

static int32_t ReadFile(const char* path,const char* buffer,uint32_t bufferlen)
{
	printf("ReadFile start path=%s\r\n",path);
	if(path == NULL || buffer == NULL || bufferlen == 0)
	{
		printf("ReadFile data invalid\r\n");
		return -1;
	}
	if(!IsFileExist(path))
	{
		printf("ReadFile file not exsit\r\n");
		return -1;
	}

	int32_t filesize = 0;
	if(UtilsFileStat(path,&filesize) != 0)
	{
		printf("ReadFile stat file failed\r\n");
		return -1;
	}


	if(filesize > bufferlen)
	{
		printf("ReadFile read data over buffer lenth\r\n");
		return -1;
	}
	int32_t fd = UtilsFileOpen(path, O_RDONLY_FS, 0);
	if(fd < 0)
	{
		printf("ReadFile open failed\r\n");
		return -1;
	}

	int32_t ret = 0;
	if(UtilsFileRead(fd, buffer, filesize) != filesize)
	{
		printf("ReadFile read data failed\r\n");
		return -1;
	}

	(void)UtilsFileClose(fd);
	return ret;
}

static int32_t DeleteFile(const char* path)
{
	UtilsFileDelete(path);
}

bool OEMIsResetFlagExist(void)
{
     return IsFileExist(RESET_FILE_NAME);
}

int32_t OEMCreateResetFlag(void)
{
    return CreateFile(RESET_FILE_NAME);
}

int32_t OEMDeleteResetFlag(void)
{
    return DeleteFile(RESET_FILE_NAME);
}

bool OEMIsAuthStatusExist(void)
{
      return IsFileExist(AUTH_STATS_FILE_NAME);
}

int32_t OEMWriteAuthStatus(const char* data, uint32_t len)
{
   if(data == NULL || len == 0)
		return -1;
		
    return WriteFile(AUTH_STATS_FILE_NAME,data,len,O_CREAT_FS | O_TRUNC_FS | O_RDWR_FS);
}

int32_t OEMReadAuthStatus(char* buffer, uint32_t bufferLen)
{
    if(buffer == NULL || bufferLen == 0)
		return -1;
		
    return ReadFile(AUTH_STATS_FILE_NAME,buffer,bufferLen);
}

int32_t OEMDeleteAuthStatus(void)
{
    return DeleteFile(AUTH_STATS_FILE_NAME);
}

int32_t OEMGetAuthStatusFileSize(uint32_t* len)
{
	printf("OEMGetAuthStatusFileSize len=%p\r\n",len);
	if(len == NULL)
		return -1;
	
    return UtilsFileStat(AUTH_STATS_FILE_NAME,len);
}

bool OEMIsTicketExist(void)
{
    return IsFileExist(TICKET_FILE_NAME);
}

int32_t OEMWriteTicket(const char* data, uint32_t len)
{
	printf("OEMWriteTicket data=%s -- len=%d\r\n",data,len);
    if(data == NULL || len == 0)
		return -1;
		
    return WriteFile(TICKET_FILE_NAME,data,len,O_CREAT_FS | O_TRUNC_FS | O_RDWR_FS);
}

int32_t OEMReadTicket(char* buffer, uint32_t bufferLen)
{
	printf("OEMReadTicket buffer=%s -- bufferLen=%d\r\n",buffer,bufferLen);
	if(buffer == NULL || bufferLen == 0)
		return -1;
		
    return ReadFile(TICKET_FILE_NAME,buffer,bufferLen);
}

int32_t OEMDeleteTicket(void)
{
     return DeleteFile(TICKET_FILE_NAME);
}

int32_t OEMGetTicketFileSize(uint32_t* len)
{
    return UtilsFileStat(TICKET_FILE_NAME,len);
}
