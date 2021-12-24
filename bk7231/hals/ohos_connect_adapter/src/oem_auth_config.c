/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: implement of oem_auth_config
 * Author: Kit Framework group
 * Create: 2020-12-26
 */

#include "oem_auth_config.h"

#include <stdio.h>
#include <stdlib.h>
#include "securec.h"

#include "cJSON.h"

#define HCS_HILINKKIT
#define KIT_ID_LEN 40

typedef const char* (*KitGetVerFunc)();

typedef struct KitGetVerInfo {
    char kitId[KIT_ID_LEN + 1];
    KitGetVerFunc kitGetVerFunc;
} KitGetVerInfo;

#ifdef HCS_DVKIT
extern const char* DvKit_GetVersion();
#endif

#ifdef HCS_HILINKKIT
//extern const char* HiLinkKit_GetVersion();
const char* HiLinkKit_GetVersion()
{
	return "1.0.0";
}
#endif



KitGetVerInfo g_kitGetVerInfo[] = {
#ifdef HCS_DVKIT
    {"DvKit", DvKit_GetVersion},
#endif
#ifdef HCS_HILINKKIT
    {"HiLinkKit", HiLinkKit_GetVersion},
#endif
    {0, NULL}
};

uint32_t OEMGetTotalTimeout()
{
    return TOTAL_TIMEOUT;
}

int32_t OEMReadAuthServerInfo(char* buff, uint32_t len)
{
    if (buff == NULL || len == 0) {
        return -1;
    }
    return -1;
}

char* OEMLoadKitInfos(void)
{
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        return NULL;
    }
    cJSON* infos = cJSON_CreateArray();
    if (infos == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    const uint32_t kitCount = sizeof(g_kitGetVerInfo) / sizeof(KitGetVerInfo);
    for (uint32_t i = 0; i < kitCount; i++) {
        cJSON* obj = cJSON_CreateObject();
        if (obj == NULL) {
            printf("OEM Adapter: Create json object failed\n");
            continue;
        }
        const char* key = g_kitGetVerInfo[i].kitId;
        KitGetVerFunc kitGetVerFunc = g_kitGetVerInfo[i].kitGetVerFunc;
        if (key == NULL || strlen(key) == 0 || kitGetVerFunc == NULL) {
            cJSON_Delete(obj);
            continue;
        }
        const char* value = (kitGetVerFunc)();
        if (value == NULL || strlen(value) == 0 || cJSON_AddStringToObject(obj, key, value) == NULL) {
            printf("OEM Adapter: Add kit info %s to json object failed\n", key);
            cJSON_Delete(obj);
            continue;
        }
        if (!cJSON_AddItemToArray(infos, obj)) {
            cJSON_Delete(obj);
            printf("OEM Adapter: Add kit info %s to json array failed\n", key);
        }
    }

    if (!cJSON_AddItemToObject(root, KIT_INFO_JSON_KEY, infos)) {
        printf("OEM Adapter: Add kit infos to json object failed\n");
        cJSON_Delete(infos);
        cJSON_Delete(root);
        return NULL;
    }
    char* result = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return result;
}
