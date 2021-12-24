/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: 蓝牙SDK提供demo示例代码
 */
#include "hilink_bt_api.h"
#include "hilink_bt_function.h"
#include "hilink_device.h"
#include <stdio.h>

/* 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�? */
typedef struct {
    const char* sn;      /* 设备唯一标识，比如sn号，长度范围�?0,40] */
    const char* prodId;  /* 设备HiLink认证号，长度范围�?0,5] */
    const char* model;   /* 设备型号，长度范围（0,32] */
    const char* dev_t;   /* 设备类型，长度范围（0,4] */
    const char* manu;    /* 设备制�?�商，长度范围（0,4] */
    const char* mac;     /* 设备MAC地址，固�?32字节 */
    const char* hiv;     /* 设备Hilink协议版本，长度范围（0,32] */
    const char* fwv;     /* 设备固件版本，长度范围[0,64] */
    const char* hwv;     /* 设备硬件版本，长度范围[0,64] */
    const char* swv;     /* 设备软件版本，长度范围[0,64] */
    const int prot_t;    /* 设备协议类型，取值范围[1,3] */
} dev_info_t;

/*
 * 设备类型英文名和厂商英文名长度之和不能超�?17字节
 * 如果�?要发送蓝牙广播，设备类型英文名和厂商英文名长度分别不能超�?4字节
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
typedef struct {
    const char* devTypeName; /* 设备类型英文名称 */
    const char* manuName;    /* 厂商英文名称 */
} DevNameEn;

/*
 * 设备基本信息
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
//#define DEVICE_SN "12345678"
#define DEVICE_MAC "AABBCCDDEEFF"
#define HILINK_VERSION "1.0.0"
#define HILINK_ProtType 1
#if 0
#define PRODUCT_ID "2XXX"
#define DEVICE_MODEL "model-X"
#define DEVICE_TYPE "000"
#define MANUAFACTURER "000"

/*
 * 请确保设备类型英文名和厂商英文名长度之和不超�?17字节
 * 如果�?要发送蓝牙广播，设备类型英文名和厂商英文名长度分别不能超�?4字节
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
#define DEVICE_TYPE_NAME "other"
#define MANUAFACTURER_NAME "HW"
#elif 0
#define PRODUCT_ID    "26GU"
#define DEVICE_TYPE   "176"
#define MANUAFACTURER "fhc"
#define DEVICE_MODEL  "bk001"


#define DEVICE_TYPE_NAME   "Smart Panel"
#define MANUAFACTURER_NAME "beken"

#elif 0
#define PRODUCT_ID    "23TJ"
#define DEVICE_TYPE   "01D"
#define MANUAFACTURER "011"
#define DEVICE_MODEL  "SP mini3-HL"


#define DEVICE_TYPE_NAME   "Switch"
#define MANUAFACTURER_NAME "Broadlink"

#elif 0
#define PRODUCT_ID    "273D"
#define DEVICE_TYPE   "195"
#define MANUAFACTURER "fhc"
#define DEVICE_MODEL  "BK001"


#define DEVICE_TYPE_NAME   "SmartBox"
#define MANUAFACTURER_NAME "BEKEN"


#endif




/*
 * 设备信息定义
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
#if 0
dev_info_t dev_info_BT = {
		OHOS_SERIAL,
		PRODUCT_ID,
		PRODUCT_MODEL,
		DEVICE_TYPE_ID,
		MANUAFACTURER_ID,
		DEVICE_MAC,
		"1.0.0",
		FIRMWARE_VER,
		HARDWARE_VER,
		SOFTWARE_VER,
		1
	};

#elif 0
dev_info_t dev_info_BT = {
    DEVICE_SN,
    PRODUCT_ID,
    DEVICE_MODEL,
    DEVICE_TYPE,
    MANUAFACTURER,
    DEVICE_MAC,
    "1.0.0",
    "1.0.0",
    "1.0.0",
    "1.0.0",
    1
};
#endif


/*
 * 设备名称定义, 请确保设备类型英文名和厂商英文名长度之和不超�?17字节
 * 如果�?要发送蓝牙广播，设备类型英文名和厂商英文名长度分别不能超�?4字节
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
#if 0
DevNameEn g_devNameEn_BT = {
		DEVICE_TYPE_NAME,
		MANUAFACTURER_NAME
	};
#endif
/* 蓝牙sdk单独使用的定�? */
//#define SUB_PRODUCT_ID "00"
#define ADV_TX_POWER 0xF8

#define MIN_LEN(a, b) (((a) < (b)) ? (a) : (b))

static HILINK_BT_DevInfo g_btDevInfo;

static int PutServiceOta(const void *svc, const unsigned char *in, unsigned int inLen,
    unsigned char **out, unsigned int *outLen)
{
    (void)inLen;
    (void)out;
    (void)outLen;
    printf("service:%s, payload:%s", (char *)svc, (char *)in);
    return 0;
}

static int GetServiceOta(const void *svc, const unsigned char *in, unsigned int inLen,
    unsigned char **out, unsigned int *outLen)
{
    (void)inLen;
    (void)out;
    (void)outLen;
    printf("service:%s, payload:%s", (char *)svc, (char *)in);
    return 0;
}

/* 服务信息 */
static HILINK_BT_SvcInfo g_svcInfo[] = {
    { 64, "ota", PutServiceOta, GetServiceOta, 0, NULL },
};

/* Profile */
static HILINK_BT_Profile g_profile = {
    .svcNum  = sizeof(g_svcInfo) / sizeof(g_svcInfo[0]),
    .svcInfo = g_svcInfo,
};

/* 蓝牙发�?�数据接口打桩函�?,由厂家实�? */
static int HILINK_BT_FeedBackBtData(const unsigned char *buf, unsigned int len)
{
    (void)buf;
    (void)len;
    return 0;
}
#if 0
/*
 * 功能: 获取设备sn�?
 * 参数[in]: len sn的最大长�?, 39字节
 * 参数[out]: sn 设备sn
 * 注意: sn指针的字符串长度�?0时将使用设备mac地址作为sn
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
void HilinkGetDeviceSn(unsigned int len, char *sn)
{
    if (sn == NULL) {
        return;
    }
    const char *ptr = DEVICE_SN;
    int tmp = MIN_LEN(len ,sizeof(DEVICE_SN));
    for (int i = 0; i < tmp; i++) {
        sn[i] = ptr[i];
    }
    return;
}

/*
 * 获取设备的子型号，长度固定两个字�?
 * subProdId为保存子型号的缓冲区，len为缓冲区的长�?
 * 如果产品定义有子型号，则填入两字节子型号，并�?'\0'结束, 返回0
 * 没有定义子型号，则返�?-1
 * 该接口需设备�?发�?�实�?
 * 与hilink sdk相同定义，双模组模式只需�?份，已提供给第三方厂家，暂不按编程规范整�?
 */
int HILINK_GetSubProdId(char *subProdId, int len)
{
    if (subProdId == NULL) {
        return -1;
    }
    const char *ptr = SUB_PRODUCT_ID;
    int tmp = MIN_LEN((unsigned int)len ,sizeof(SUB_PRODUCT_ID));
    for (int i = 0; i < tmp; i++) {
        subProdId[i] = ptr[i];
    }
    return 0;
}
#endif
/*
 * 获取设备表面的最强点信号发射功率强度，最强点位置的确定以及功率测试方
 * 法，参照hilink认证蓝牙靠近发现功率设置及测试方法指导文档，power为出�?
 * ，单位dbm，返回设备表面的�?强信号强度�?�，如果厂商不想使用蓝牙靠近发现�?
 * 能，接口直接�?-1，如果需要使用蓝牙靠近发现，则接口返�?0，如�?及时生效，需
 * 调用HILINK_BT_StartAdvertise()方法启动广播
 */
int HILINK_BT_GetDevSurfacePower(char *power)
{
    if (power == NULL) {
        return -1;
    }
    *power = ADV_TX_POWER;
    return 0;
}

/* 获取蓝牙SDK设备相关信息 */
HILINK_BT_DevInfo *HILINK_BT_GetDevInfo(void)
{
    g_btDevInfo.manuName = (char *)MANUAFACTURER_NAME;
    g_btDevInfo.devName = (char *)DEVICE_TYPE_NAME;
    g_btDevInfo.productId = (char *)PRODUCT_ID;
    g_btDevInfo.mac = (char *)DEVICE_MAC;
    g_btDevInfo.model = (char *)PRODUCT_MODEL;
    g_btDevInfo.devType = (char *)DEVICE_TYPE_ID;
    g_btDevInfo.hiv = (char *)HILINK_VERSION;
    g_btDevInfo.protType = HILINK_ProtType;
    return &g_btDevInfo;
}

int main(void)
{
    int ret;

    ret = HILINK_BT_Init(&g_profile);
    if (ret != 0) {
        printf("init fail");
        return ret;
    }

    ret = HILINK_BT_Process();
    if (ret != 0) {
        printf("process fail");
        return ret;
    }

    /* 设置蓝牙发�?�接�? */
    ret = HILINK_BT_RegisterBtDataSendCallback(HILINK_BT_FeedBackBtData);
    if (ret != 0) {
        printf("set callback fail");
        return ret;
    }

    return 0;
}

