/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hal_file.h"
#include "utils_file.h"

#include "easyflash.h"
#include <stdlib.h>

struct file_info {
	const char *file_name;
	int fd;
	char *content;
	unsigned int len;
	unsigned int offset;
};

#define INVALID_FD	-1
#define FILE_ERR_GENERAL -1

#define MAX_FILE_CNT 32

static struct file_info infos[MAX_FILE_CNT] = {
	[0 ... (MAX_FILE_CNT-1)] = {NULL, INVALID_FD, NULL, 0},
};

#define ESC_CHAR1 '\0'
//#define ESC_CHAR2 0xFE
#define ESC_CHAR2 0x41

#define MAX_FILE_NAME_LEN 31
#define MAX_FILE_LENGTH 512

#define MAGIC_CHAR '&'
#define MAGIC_LEN 1

static unsigned int file_encode(const char *buf_in, unsigned int len, char *buf_out)
{
	int i;
	int j;
	
	if (!buf_in || !buf_out || (len == 0))
		return 0;

	for (i = 0, j = 0; i < len; i++) {
		if (buf_in[i] == ESC_CHAR1) {
			buf_out[j++] = ESC_CHAR2;
		} else if (buf_in[i] == ESC_CHAR2) {
			buf_out[j++] = ESC_CHAR2;
			buf_out[j++] = ESC_CHAR2;
		} else {
			buf_out[j++] = buf_in[i];
		}
	}
	
	return j;
}

static unsigned int file_decode(char *buf_in, unsigned int len, char *buf_out)
{
	int i;
	int j;
	
	if (!buf_in || !buf_out || (len == 0))
		return 0;

	for (i = 0, j = 0; i < len; i++) {
		if (buf_in[i] == ESC_CHAR2) {
			if ((i+1 < len) && (buf_in[i+1] == ESC_CHAR2)) {
				buf_out[j++] = ESC_CHAR2;
				i++;	//skip it
			} else {
				buf_out[j++] = ESC_CHAR1;
			}
		} else {
			buf_out[j++] = buf_in[i];
		}
	}
	
	return j;
}

static void init_ef(void)
{
	static int inited = 0;
	int ret;

	if (!inited) {
		inited = 1;
		ret = easyflash_init();
		ret = ret;
		//printf("easyflash_init : ret=%d\n", ret);
	}
}

static int file_exist(const char *key) 
{
	char *value;
	
	init_ef();
	
	value = ef_get_env(key);
	if (value)
		return 1;
	else
		return 0;
}

static int file_load(const char *key, char *value_out, unsigned int *len_out)
{
	char *value;
	unsigned int len;
	
	init_ef();

	value = ef_get_env(key);
	if (!value || value[0] != MAGIC_CHAR) {
		*len_out = 0;
		return FILE_ERR_GENERAL;
	}

	value += MAGIC_LEN;	//skip MAGIC_CHAR
	
	len = file_decode(value, strlen(value), value_out);
	*len_out = len;
	
	return 0;
}

static int file_save(const char *key, const char *value, unsigned int len)
{
	char *buffer;
	char *buffer2;
	unsigned int len2;
	int ret;

	init_ef();

	if (!value) {	//for delete
		ef_set_and_save_env(key, NULL);
		return 0;
	}

	buffer = malloc(len * 2 + 1 + 1);
	if (!buffer)
		return FILE_ERR_GENERAL;

	buffer[0] = MAGIC_CHAR;
	buffer2 = buffer + MAGIC_LEN;
	
	len2 = file_encode(value, len, buffer2);
	buffer2[len2] = '\0';
	
	ret = ef_set_and_save_env(key, buffer);
	free(buffer);

	if (ret != 0)
		return FILE_ERR_GENERAL;
	return 0;
}

static struct file_info* file_get_info(int fd)
{
    int i;
	struct file_info *info;

	if (fd <= 0 || fd > MAX_FILE_CNT)
		return NULL;

	info = &infos[fd -1];
	if (info->fd != fd)
		return NULL;

	return info;
}

int HalFileOpen(const char* path, int oflag, int mode)
{
    int i;
	int exist;
	struct file_info *info;

	for (i = 0; i < MAX_FILE_CNT; i++) {
		if (infos[i].fd == INVALID_FD)
			break;
	}

	if (i >= MAX_FILE_CNT)
		return FILE_ERR_GENERAL;

	if (strlen(path) > MAX_FILE_NAME_LEN)
		return FILE_ERR_GENERAL;

	exist = file_exist(path);
	
	if (!(oflag & O_CREAT_FS) && !exist) {
		return FILE_ERR_GENERAL;
	}
	
	if ((oflag & O_EXCL_FS) && exist) {
		return FILE_ERR_GENERAL;
	}

	info = &infos[i];

	info->content = malloc(MAX_FILE_LENGTH);
	if (!info->content)
		return FILE_ERR_GENERAL;

	info->file_name = malloc(strlen(path) + 1);
	if (!info->file_name) {
		free(info->content);
		return FILE_ERR_GENERAL;
	}
	strcpy(info->file_name, path);
	
	if (oflag & O_TRUNC_FS) {
		info->len = 0;
	} else {
		file_load(path, info->content, &info->len);
	}

	if (oflag & O_APPEND_FS) {
		info->offset = info->len;		
	} else {
		info->offset = 0;		
	}

	info->fd = i + 1;

	return info->fd;
}

int HalFileClose(int fd)
{
    struct file_info *info;
	int ret;

	info = file_get_info(fd);
	if (!info)
		return FILE_ERR_GENERAL;

	ret = file_save(info->file_name, info->content, info->len);

	free(info->file_name);
	free(info->content);
	info->fd = INVALID_FD;
	
	return 0;	
}

int HalFileRead(int fd, char *buf, unsigned int len)
{
    struct file_info *info;
    int ret;

	info = file_get_info(fd);
	if (!info)
		return FILE_ERR_GENERAL;

	if (!buf)
		return FILE_ERR_GENERAL;
		
	if (info->offset + len > info->len)
		len = info->len - info->offset;

	if (len) {
		memcpy(buf, info->content + info->offset, len);
		info->offset += len;
	}

	return len;
}

int HalFileWrite(int fd, const char *buf, unsigned int len)
{
    struct file_info *info;
    int ret;

	info = file_get_info(fd);
	if (!info)
		return FILE_ERR_GENERAL;

	if (!buf)
		return FILE_ERR_GENERAL;
		
	if (info->offset + len > MAX_FILE_LENGTH)
		len = MAX_FILE_LENGTH - info->offset;

	if (len) {
		memcpy(info->content + info->offset, buf, len);
		info->offset += len;
		if (info->len < info->offset)
			info->len = info->offset;
	}

	return len;
}

int HalFileDelete(const char *path)
{
	int exist;

	exist = file_exist(path);
	if (!exist)
		return FILE_ERR_GENERAL;

	file_save(path, NULL, 0);	//value==NULL means to delete
    return 0;
}

int HalFileStat(const char *path, unsigned int *fileSize)
{
	int fd;
    struct file_info *info;

	if (!fileSize)
		return FILE_ERR_GENERAL;
	
	fd = HalFileOpen(path, O_RDONLY_FS, 0);
	if (fd <= 0)
		return FILE_ERR_GENERAL;

	info = file_get_info(fd);
	if (!info) {
		HalFileClose(fd);
		return FILE_ERR_GENERAL;
	}

	*fileSize = info->len;
	HalFileClose(fd);
	
	return 0;
}

int HalFileSeek(int fd, int offset, unsigned int whence)
{
    struct file_info *info;
    //int ret;
    int pos;

	info = file_get_info(fd);
	if (!info)
		return FILE_ERR_GENERAL;

	if (whence == SEEK_SET_FS)
		pos = 0;
	else if (whence == SEEK_CUR_FS)
		pos = info->offset;
	else if (whence == SEEK_END_FS)
		pos = info->len;
	else
		return FILE_ERR_GENERAL;

	if (pos + offset < 0)
		return FILE_ERR_GENERAL;

	if (pos + offset > info->len)
		return FILE_ERR_GENERAL;

	info->offset = pos + offset;

	return info->offset;
}
