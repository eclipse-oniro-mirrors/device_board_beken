/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: HiLink��Ʒ����ʵ��Դ�ļ�
 */
#include "hilink_device.h"
#include <stdlib.h>
#include "hilink.h"
#include "securec.h"
#include "cJSON.h"
#include <stdbool.h>
#include "wifi_device.h"
#include "utils_file.h"

#include <samgr_lite.h>
#include "ble_cfg_net_api.h"
#include "wlan_cli_pub.h"
#include "uart_pub.h"


#define PRD_Lamp_21TW 1 /* DALEN product only wifi */
#define PRD_Smart_Box_273D 2 /* BEKEN smart box. wifi+ble */
#define PRD_Smart_Panel_26GU 3 /* BEKEN smart panel. wifi+ble */
#define PRD_Smart_Pen_2DDZ 4 /* wyt pen. wifi+ble */

#define USING_PRODUCT_ID PRD_Smart_Pen_2DDZ

#if(USING_PRODUCT_ID == PRD_Lamp_21TW)
 const char OHOS_MARKET_NAME[] = {"BEKEN-SMT-BOX"};//���Ʒ����һ��
 /* �豸��ƷID */
 const char *PRODUCT_ID = "21TW";
 /* �豸��Ʒ���ͺ�ID */
 const char *SUB_PRODUCT_ID = "";
 /* �豸��Ʒkey */
 const char *PRODUCT_KEY = "7d9d27122c644ce9b8481dfb6390118b";
 const char OHOS_BRAND[] = {"BEKEN"};			 //kitfwk��֤�ؼ����Ʒ��Ӣ����һ��
 const char OHOS_PRODUCT_SERIES[] = {"BEKEN-AUDIO-WIFI"};		 //������һ��
 /* �豸�ͺ� */
 const char *PRODUCT_MODEL = "DL-01W";
 /* �豸����Ӣ������ */
 const char *DEVICE_TYPE_NAME = "Table Lamp";
 /* �豸����ID */
 const char *DEVICE_TYPE_ID = "046";
 /* �豸������Ӣ������ */
 const char *MANUAFACTURER_NAME = "DALEN";
 /* �豸������ID */
 const char *MANUAFACTURER_ID = "17C";

  /* AC���� */
  unsigned char Ackey[48] = {
	 0x24, 0x3D, 0x4C, 0x77, 0x3B, 0x37, 0x6C, 0x70, 
	 0x66, 0x60, 0x2F, 0x4C, 0x74, 0x29, 0x2F, 0x24,
	 0xD8, 0x01, 0x68, 0xB9, 0xF1, 0x7E, 0x6D, 0xC9,
	 0x07, 0xAD, 0x38, 0xF1, 0x28, 0x1A, 0x6E, 0x67,
	 0x15, 0x3C, 0xAB, 0xCC, 0x26, 0xBA, 0xC1, 0xA7,
	 0x2C, 0x63, 0x7E, 0x9F, 0x4E, 0xF1, 0x7F, 0xBD
 };

 /* �豸�̼��汾�� */
 const char *FIRMWARE_VER = "1.0.0";
 /* �豸����汾�� */
 const char *SOFTWARE_VER = "bk_sw_1.0.0";  //��������ͺ�

 /* �豸Ӳ���汾�� */
 const char *HARDWARE_VER = "bk_hw_1.0.0";  //����Ӳ���ͺ�

 const char *OHOS_SERIAL = "123456789";		 // //kitfwk��֤�ؼ�������ƷΨһ��ʶ����Ӧ���ɺ궨�壬Ӧ���ɳ��Ҳ�����¼Ԥ�õ�flash��ͨ���޸�char* HalGetSerial(void)����������flash�в���Ԥ�õ����Ψһ��ʶ������ʱ�����ʱʹ�ô˺꣬ע�Ᵽ֤��ͬ�ͺŴ˱��Ψһ������ظ���ͬ�ͺ�ֻ�����һ̨�豸��

 const char OHOS_PRODUCT_TYPE[] = {"linkiot"};	 //�̶�
 const char OHOS_HARDWARE_PROFILE[] = {"{RAM:256,FLASH:256,WIFI:true,BLE:true}"}; //����оƬȷ��
 const char OHOS_BOOTLOADER_VERSION[] = {"bootloader_bk72xx_uart2_v1.0.8"};	 //������Ӱ�죬��֤ǰȷ��
 const char OHOS_SECURITY_PATCH_TAG[] = {"2021-07-15"};	  //������Ӱ�죬��֤ǰȷ��
 const char OHOS_ABI_LIST[] = {"riscv-liteos"};		 // �̶�ֵ


#elif(USING_PRODUCT_ID == PRD_Smart_Box_273D)
 const char OHOS_MARKET_NAME[] = {"BEKEN-SMT-BOX"};//���Ʒ����һ��
 /* �豸��ƷID */
 const char *PRODUCT_ID = "273D";
 /* �豸��Ʒ���ͺ�ID */
 const char *SUB_PRODUCT_ID = "";
 /* �豸��Ʒkey */
 const char *PRODUCT_KEY = "7d9d27122c644ce9b8481dfb6390118b";
 const char OHOS_BRAND[] = {"BEKEN"};			 //kitfwk��֤�ؼ����Ʒ��Ӣ����һ��
 const char OHOS_PRODUCT_SERIES[] = {"BEKEN-AUDIO-WIFI"};		 //������һ��
 /* �豸�ͺ� */
 const char *PRODUCT_MODEL = "BK001";
 /* �豸����Ӣ������ */
 const char *DEVICE_TYPE_NAME = "SmartBox";
 /* �豸����ID */
 const char *DEVICE_TYPE_ID = "195";
 /* �豸������Ӣ������ */
 const char *MANUAFACTURER_NAME = "BEKEN";
 /* �豸������ID */
 const char *MANUAFACTURER_ID = "fhc";

  /* AC���� */
  unsigned char Ackey[48] = {
	 0x24, 0x3D, 0x4C, 0x77, 0x3B, 0x37, 0x6C, 0x70, 
	 0x66, 0x60, 0x2F, 0x4C, 0x74, 0x29, 0x2F, 0x24,
	 0xD8, 0x01, 0x68, 0xB9, 0xF1, 0x7E, 0x6D, 0xC9,
	 0x07, 0xAD, 0x38, 0xF1, 0x28, 0x1A, 0x6E, 0x67,
	 0x15, 0x3C, 0xAB, 0xCC, 0x26, 0xBA, 0xC1, 0xA7,
	 0x2C, 0x63, 0x7E, 0x9F, 0x4E, 0xF1, 0x7F, 0xBD
 };

 /* �豸�̼��汾�� */
 const char *FIRMWARE_VER = "1.0.0";
 /* �豸����汾�� */
 const char *SOFTWARE_VER = "bk_sw_1.0.0";  //��������ͺ�

 /* �豸Ӳ���汾�� */
 const char *HARDWARE_VER = "bk_hw_1.0.0";  //����Ӳ���ͺ�

 const char *OHOS_SERIAL = "123456789";		 // //kitfwk��֤�ؼ�������ƷΨһ��ʶ����Ӧ���ɺ궨�壬Ӧ���ɳ��Ҳ�����¼Ԥ�õ�flash��ͨ���޸�char* HalGetSerial(void)����������flash�в���Ԥ�õ����Ψһ��ʶ������ʱ�����ʱʹ�ô˺꣬ע�Ᵽ֤��ͬ�ͺŴ˱��Ψһ������ظ���ͬ�ͺ�ֻ�����һ̨�豸��

 const char OHOS_PRODUCT_TYPE[] = {"linkiot"};	 //�̶�
 const char OHOS_HARDWARE_PROFILE[] = {"{RAM:256,FLASH:256,WIFI:true,BLE:true}"}; //����оƬȷ��
 const char OHOS_BOOTLOADER_VERSION[] = {"bootloader_bk72xx_uart2_v1.0.8"};	 //������Ӱ�죬��֤ǰȷ��
 const char OHOS_SECURITY_PATCH_TAG[] = {"2021-07-15"};	  //������Ӱ�죬��֤ǰȷ��
 const char OHOS_ABI_LIST[] = {"riscv-liteos"};		 // �̶�ֵ

#elif(USING_PRODUCT_ID == PRD_Smart_Panel_26GU)
const char OHOS_MARKET_NAME[] = {"bk7231n-demo"};//���Ʒ����һ��
/* �豸��ƷID */
const char *PRODUCT_ID = "26GU";
/* �豸��Ʒ���ͺ�ID */
const char *SUB_PRODUCT_ID = "";
/* �豸��Ʒkey */
const char *PRODUCT_KEY = "41351e3d91d14067beb0cd086c9723b1";
const char OHOS_BRAND[] = {"beken"};			//kitfwk��֤�ؼ����Ʒ��Ӣ����һ��
const char OHOS_PRODUCT_SERIES[] = {"bk"};		//������һ��
/* �豸�ͺ� */
const char *PRODUCT_MODEL = "bk001";
/* �豸����Ӣ������ */
const char *DEVICE_TYPE_NAME = "Smart Panel";
/* �豸����ID */
const char *DEVICE_TYPE_ID = "176";
/* �豸������Ӣ������ */
const char *MANUAFACTURER_NAME = "BEKEN";
/* �豸������ID */
const char *MANUAFACTURER_ID = "fhc";

 /* AC���� */
 unsigned char Ackey[48] = {
	0x24, 0x3D, 0x4C, 0x77, 0x3B, 0x37, 0x6C, 0x70, 
	0x66, 0x60, 0x2F, 0x4C, 0x74, 0x29, 0x2F, 0x24,
	0xD8, 0x01, 0x68, 0xB9, 0xF1, 0x7E, 0x6D, 0xC9,
	0x07, 0xAD, 0x38, 0xF1, 0x28, 0x1A, 0x6E, 0x67,
	0x15, 0x3C, 0xAB, 0xCC, 0x26, 0xBA, 0xC1, 0xA7,
	0x2C, 0x63, 0x7E, 0x9F, 0x4E, 0xF1, 0x7F, 0xBD
};

/* �豸�̼��汾�� */
const char *FIRMWARE_VER = "1.0.0";
/* �豸����汾�� */
const char *SOFTWARE_VER = "bk_sw_1.0.0";  //��������ͺ�

/* �豸Ӳ���汾�� */
const char *HARDWARE_VER = "bk_hw_1.0.0";  //����Ӳ���ͺ�

const char *OHOS_SERIAL = "123456789";		// //kitfwk��֤�ؼ�������ƷΨһ��ʶ����Ӧ���ɺ궨�壬Ӧ���ɳ��Ҳ�����¼Ԥ�õ�flash��ͨ���޸�char* HalGetSerial(void)����������flash�в���Ԥ�õ����Ψһ��ʶ������ʱ�����ʱʹ�ô˺꣬ע�Ᵽ֤��ͬ�ͺŴ˱��Ψһ������ظ���ͬ�ͺ�ֻ�����һ̨�豸��

const char OHOS_PRODUCT_TYPE[] = {"linkiot"};	//�̶�
const char OHOS_HARDWARE_PROFILE[] = {"{RAM:256,FLASH:256,WIFI:true,BLE:true}"}; //����оƬȷ��
const char OHOS_BOOTLOADER_VERSION[] = {"bootloader_bk72xx_uart2_v1.0.8"};	//������Ӱ�죬��֤ǰȷ��
const char OHOS_SECURITY_PATCH_TAG[] = {"2021-07-15"};	 //������Ӱ�죬��֤ǰȷ��
const char OHOS_ABI_LIST[] = {"riscv-liteos"};		// �̶�ֵ

#elif(USING_PRODUCT_ID == PRD_Smart_Pen_2DDZ)

const char OHOS_MARKET_NAME[] = {"BEKEN-SMT-BOX"};//���Ʒ����һ��
/* �豸��ƷID */
const char *PRODUCT_ID = "2DDZ";
/* �豸��Ʒ���ͺ�ID */
const char *SUB_PRODUCT_ID = "";
/* �豸��Ʒkey */
const char *PRODUCT_KEY = "7d9d27122c644ce9b8481dfb6390118b";
const char OHOS_BRAND[] = {"BEKEN"};			//kitfwk��֤�ؼ����Ʒ��Ӣ����һ��
const char OHOS_PRODUCT_SERIES[] = {"BEKEN-AUDIO-WIFI"};		//������һ��
/* �豸�ͺ� */
const char *PRODUCT_MODEL = "BK001";
/* �豸����Ӣ������ */
const char *DEVICE_TYPE_NAME = "SmartBox";
/* �豸����ID */
const char *DEVICE_TYPE_ID = "195";
/* �豸������Ӣ������ */
const char *MANUAFACTURER_NAME = "BEKEN";
/* �豸������ID */
const char *MANUAFACTURER_ID = "fhc";

 /* AC���� */
 unsigned char Ackey[48] = {
	0x24, 0x3D, 0x4C, 0x77, 0x3B, 0x37, 0x6C, 0x70, 
	0x66, 0x60, 0x2F, 0x4C, 0x74, 0x29, 0x2F, 0x24,
	0xD8, 0x01, 0x68, 0xB9, 0xF1, 0x7E, 0x6D, 0xC9,
	0x07, 0xAD, 0x38, 0xF1, 0x28, 0x1A, 0x6E, 0x67,
	0x15, 0x3C, 0xAB, 0xCC, 0x26, 0xBA, 0xC1, 0xA7,
	0x2C, 0x63, 0x7E, 0x9F, 0x4E, 0xF1, 0x7F, 0xBD
};

/* �豸�̼��汾�� */
const char *FIRMWARE_VER = "1.0.0";
/* �豸����汾�� */
const char *SOFTWARE_VER = "bk_sw_1.0.0";  //��������ͺ�

/* �豸Ӳ���汾�� */
const char *HARDWARE_VER = "bk_hw_1.0.0";  //����Ӳ���ͺ�

const char *OHOS_SERIAL = "123456789";		// //kitfwk��֤�ؼ�������ƷΨһ��ʶ����Ӧ���ɺ궨�壬Ӧ���ɳ��Ҳ�����¼Ԥ�õ�flash��ͨ���޸�char* HalGetSerial(void)����������flash�в���Ԥ�õ����Ψһ��ʶ������ʱ�����ʱʹ�ô˺꣬ע�Ᵽ֤��ͬ�ͺŴ˱��Ψһ������ظ���ͬ�ͺ�ֻ�����һ̨�豸��

const char OHOS_PRODUCT_TYPE[] = {"linkiot"};	//�̶�
const char OHOS_HARDWARE_PROFILE[] = {"{RAM:256,FLASH:256,WIFI:true,BLE:true}"}; //����оƬȷ��
const char OHOS_BOOTLOADER_VERSION[] = {"bootloader_bk72xx_uart2_v1.0.8"};	//������Ӱ�죬��֤ǰȷ��
const char OHOS_SECURITY_PATCH_TAG[] = {"2021-07-15"};	 //������Ӱ�죬��֤ǰȷ��
const char OHOS_ABI_LIST[] = {"riscv-liteos"};		// �̶�ֵ


#elif (USING_PRODUCT_ID == 5)
 /* ��ȷ���豸����Ӣ�����ͳ���Ӣ��������֮�Ͳ�����17�ֽ� */
const char OHOS_MARKET_NAME[] = {"BEKEN-SMT-BOX"};//���Ʒ����һ��
/* �豸��ƷID */
const char *PRODUCT_ID = "1011";
/* �豸��Ʒ���ͺ�ID */
const char *SUB_PRODUCT_ID = "";
/* �豸��Ʒkey */
const char *PRODUCT_KEY = "7d9d27122c644ce9b8481dfb6390118b";
const char OHOS_BRAND[] = {"BEKEN"};			//kitfwk��֤�ؼ����Ʒ��Ӣ����һ��
const char OHOS_PRODUCT_SERIES[] = {"BEKEN-AUDIO-WIFI"};		//������һ��
/* �豸�ͺ� */
const char *PRODUCT_MODEL = "SP mini3-HL";
/* �豸����Ӣ������ */
const char *DEVICE_TYPE_NAME = "Switch";
/* �豸����ID */
const char *DEVICE_TYPE_ID = "01D";
/* �豸������Ӣ������ */
const char *MANUAFACTURER_NAME = "Broadlink";
/* �豸������ID */
const char *MANUAFACTURER_ID = "011";

 /* AC���� */
 unsigned char Ackey[48] = {
	0x24, 0x3D, 0x4C, 0x77, 0x3B, 0x37, 0x6C, 0x70, 
	0x66, 0x60, 0x2F, 0x4C, 0x74, 0x29, 0x2F, 0x24,
	0xD8, 0x01, 0x68, 0xB9, 0xF1, 0x7E, 0x6D, 0xC9,
	0x07, 0xAD, 0x38, 0xF1, 0x28, 0x1A, 0x6E, 0x67,
	0x15, 0x3C, 0xAB, 0xCC, 0x26, 0xBA, 0xC1, 0xA7,
	0x2C, 0x63, 0x7E, 0x9F, 0x4E, 0xF1, 0x7F, 0xBD
};

/* �豸�̼��汾�� */
const char *FIRMWARE_VER = "1.0.0";
/* �豸����汾�� */
const char *SOFTWARE_VER = "bk_sw_1.0.0";  //��������ͺ�

/* �豸Ӳ���汾�� */
const char *HARDWARE_VER = "bk_hw_1.0.0";  //����Ӳ���ͺ�

const char *OHOS_SERIAL = "123456789";		// //kitfwk��֤�ؼ�������ƷΨһ��ʶ����Ӧ���ɺ궨�壬Ӧ���ɳ��Ҳ�����¼Ԥ�õ�flash��ͨ���޸�char* HalGetSerial(void)����������flash�в���Ԥ�õ����Ψһ��ʶ������ʱ�����ʱʹ�ô˺꣬ע�Ᵽ֤��ͬ�ͺŴ˱��Ψһ������ظ���ͬ�ͺ�ֻ�����һ̨�豸��

const char OHOS_PRODUCT_TYPE[] = {"linkiot"};	//�̶�
const char OHOS_HARDWARE_PROFILE[] = {"{RAM:256,FLASH:256,WIFI:true,BLE:true}"}; //����оƬȷ��
const char OHOS_BOOTLOADER_VERSION[] = {"bootloader_bk72xx_uart2_v1.0.8"};	//������Ӱ�죬��֤ǰȷ��
const char OHOS_SECURITY_PATCH_TAG[] = {"2021-07-15"};	 //������Ӱ�죬��֤ǰȷ��
const char OHOS_ABI_LIST[] = {"riscv-liteos"};		// �̶�ֵ

#endif

/* �豸SN */
static const char *PRODUCT_SN = "";


/* ������Ϣ���� */
static const HILINK_SvcInfo SVC_INFO[] = {
    {"switch", "switch"},
};

typedef struct{
    char* name;    // 
    unsigned char type;    //
    unsigned char permissions;  //
} SRV_PROPERTY_S;

//
typedef struct{
    char* id; //
    char* type;//
    int prop_count;//
    const SRV_PROPERTY_S* props;//

} SRV_INFO_S;

#define OPR_REPORT 0x01
#define OPR_GET 0x02
#define OPR_PUT 0x04


#define DTYPE_BOOL 0

//
#define SRV_IDX_SWITCH 			0
#define SWITCH_PROP_COUNT 1
#define SWITCH_ON_IDX 0
const SRV_PROPERTY_S switch_props[SWITCH_PROP_COUNT] = {
    { "on", DTYPE_BOOL, OPR_GET | OPR_PUT | OPR_REPORT }
};
#define SRV_TOTAL_COUNT 1
const SRV_INFO_S services[SRV_TOTAL_COUNT] = {
    {"switch", "switch", SWITCH_PROP_COUNT, switch_props}
};


int HILINK_GetDevInfo(HILINK_DevInfo *devinfo)
{
    if (devinfo == NULL) {
        return -1;
    }
    int err = EOK;
    err |= strcpy_s(devinfo->sn, sizeof(devinfo->sn), PRODUCT_SN);
    err |= strcpy_s(devinfo->prodId, sizeof(devinfo->prodId), PRODUCT_ID);
    err |= strcpy_s(devinfo->subProdId, sizeof(devinfo->subProdId), SUB_PRODUCT_ID);
    err |= strcpy_s(devinfo->model, sizeof(devinfo->model), PRODUCT_MODEL);
    err |= strcpy_s(devinfo->devTypeId, sizeof(devinfo->devTypeId), DEVICE_TYPE_ID);
    err |= strcpy_s(devinfo->devTypeName, sizeof(devinfo->devTypeName), DEVICE_TYPE_NAME);
    err |= strcpy_s(devinfo->manuId, sizeof(devinfo->manuId), MANUAFACTURER_ID);
    err |= strcpy_s(devinfo->manuName, sizeof(devinfo->manuName), MANUAFACTURER_NAME);
    err |= strcpy_s(devinfo->fwv, sizeof(devinfo->fwv), FIRMWARE_VER);
    err |= strcpy_s(devinfo->hwv, sizeof(devinfo->hwv), HARDWARE_VER);
    err |= strcpy_s(devinfo->swv, sizeof(devinfo->swv), SOFTWARE_VER);
    if (err != EOK) {
        return -1;
    }
    return 0;
}

int HILINK_GetSvcInfo(HILINK_SvcInfo *svcInfo[], unsigned int size)
{
    unsigned int svcNum = sizeof(SVC_INFO) / sizeof(HILINK_SvcInfo);
    if ((svcInfo == NULL) || (size == 0) || (size < svcNum)) {
        return -1;
    }

    for (unsigned int i = 0; i < svcNum; ++i) {
        if (memcpy_s(svcInfo[i], sizeof(HILINK_SvcInfo), &SVC_INFO[i], sizeof(HILINK_SvcInfo)) != EOK) {
            return -1;
        }
    }
    return svcNum;
}
/*���ִ��ķ���IDת�ɶ�Ӧ�ı��*/
int getServiceIndex(const char* svc_id) {
    int i = 0;
    for (i = 0; i < SRV_TOTAL_COUNT; i++) {
        if ((strlen(services[i].id) == strlen(svc_id)) &&
            (strncmp(svc_id, services[i].id, strlen(svc_id)) == 0)) {
            return i;
        }
    }
    return -1;
}

/* ��ȡ���� AC ����  */
unsigned char *HILINK_GetAutoAc(void)
{
    return Ackey;
}

static bool g_switch = false;

void handle_switch_cmd(bool on)
{
    g_switch = on;
    printf("DEMO: Hilink recieve a PUT cmd, on = %d.\r\n", on);
}

bool get_switch_state(void)
{
    printf("DEMO: Hilink recieve a GET cmd, on = %d.\r\n", g_switch);
    return g_switch;
}

int report_switch_state(bool on)
{
    int err = 0;
    printf("start report switch service state\r\n");
    int out_len = 0;
    char out[20];
    (void)memset_s(out, sizeof(char)*20, 0, sizeof(char)*20);

    // ��ʽ��JSON����
    out_len = sprintf_s(out, 20, "{\"on\":%d}", on);
    // ����ԭHiLink�����ϱ��ӿ�
    err = HILINK_ReportCharState(services[SRV_IDX_SWITCH].id, out, out_len);
    printf("end report switch service state - ret [%d]--[%s]\r\n", err,out);
    return err;

}
/*
 * �޸ķ���ǰ�ֶ�ֵ
 * svcIdΪ�����ID��payloadΪ���յ���Ҫ�޸ĵ�Json��ʽ���ֶ�����ֵ��lenΪpayload�ĳ���
 * ����0��ʾ����״ֵ̬�޸ĳɹ�������Ҫ�ײ��豸�����ϱ�����Hilink Device SDK�ϱ���
 * ����-101��ʾ��ñ��Ĳ�����Ҫ��
 * ����-111��ʾ����״ֵ̬�����޸��У��޸ĳɹ���ײ��豸���������ϱ���
 */
int HILINK_PutCharState(const char *svcId, const char *payload, unsigned int len)
{
    if ((svcId == NULL) || (payload == NULL)) {
        return -1;
    }

    if (strcmp(svcId, "switch") != 0) {
        return -1;
    }

    cJSON *json = cJSON_Parse(payload);
    if (json == NULL) {
        return -1;
    }

    bool on = false;
    cJSON *item = cJSON_GetObjectItem(json, "on");
    if (item == NULL) {
        cJSON_Delete(json);
        return -1;
    }
    on = item->valueint;
    handle_switch_cmd(on);

    cJSON_Delete(json);
    return 0;
}

/*
 * ��ȡ�����ֶ�ֵ
 * svcId��ʾ����ID������ʵ�ָú���ʱ����Ҫ��svcId�����жϣ�
 * in��ʾ���յ���Json��ʽ���ֶ�����ֵ��
 * inLen��ʾ���յ���in�ĳ��ȣ�
 * out��ʾ��������ֶ�ֵ���ݵ�ָ��,�ڴ��ɳ��̿��٣�ʹ����ɺ���Hilink Device SDK�ͷţ�
 * outLen��ʾ��ȡ����payload�ĳ��ȣ�
 * ����0��ʾ����״̬�ֶ�ֵ��ȡ�ɹ������ط�0��ʾ��ȡ����״̬�ֶ�ֵ���ɹ���
 */
int HILINK_GetCharState(const char *svcId, const char *in, unsigned int inLen, char **out, unsigned int *outLen)
{
    if ((svcId == NULL) || (out == NULL) || (outLen == NULL)) {
        return -1;
    }

    if (strcmp(svcId, "switch") != 0) {
        return -1;
    }

    bool on = get_switch_state();

    *outLen = 20;
    *out = (char *)malloc(*outLen);
    if (*out == NULL) {
        printf("malloc failed in GET cmd\r\n");
    }
    *outLen = sprintf_s(*out, *outLen, "{\"on\":%d}", on);

    return 0;
}

/*
 * ��ȡ�豸sn��
 * ����len��ʾsn����󳤶�,39�ֽ�
 * ����sn��ʾ�豸sn
 * snָ����ַ�������Ϊ0ʱ��ʹ���豸mac��ַ��Ϊsn
 */
void HILINK_GetDeviceSn(unsigned int len, char *sn)
{
    /* �ڴ˴����ʵ�ִ���, ��sn��ֵ��*sn�ش� */

//	strcpy(sn, "123456789");
	strcpy(sn, OHOS_SERIAL);

	return;
}

/*
 * ��ȡ�豸��ذ汾��
 * ����0��ʾ�汾�Ż�ȡ�ɹ�������������ʾ�汾�Ż�ȡʧ��
 * ע�⣬�˽ӿ�ΪHiLink�ڲ����ú���
 */
int getDeviceVersion(char **firmwareVer, char **softwareVer, char **hardwareVer)
{
    *firmwareVer = FIRMWARE_VER;
    *softwareVer = SOFTWARE_VER;
    *hardwareVer = HARDWARE_VER;
    return 0;
}
/*
 * ��ȡSoftAp����PIN��
 * ����ֵΪ8λ����PIN��, ����-1��ʾʹ��HiLink SDK��Ĭ��PIN��
 * �ýӿ����豸������ʵ��
 * ��ȫ��֤Ҫ��PIN�벻����sn��mac���豸�̶���Ϣ���ɡ�
 */
int HILINK_GetPinCode(void)
{
    return -1;
}

/*
 * ֪ͨ�豸��״̬
 * status��ʾ�豸��ǰ��״̬
 * ע�⣬�˺������豸���̸��ݲ�Ʒҵ��ѡ����ʵ��
 */
void HILINK_NotifyDevStatus(int status)
{
    switch (status) {
        case HILINK_M2M_CLOUD_OFFLINE:
            /* �豸���ƶ����ӶϿ������ڴ˴����ʵ�� */
            break;
        case HILINK_M2M_CLOUD_ONLINE:
            /* �豸�����ƶ˳ɹ������ڴ˴����ʵ�� */
            break;
        case HILINK_M2M_LONG_OFFLINE:
            /* �豸���ƶ����ӳ�ʱ��Ͽ������ڴ˴����ʵ�� */
            break;
        case HILINK_M2M_LONG_OFFLINE_REBOOT:
            /* �豸���ƶ����ӳ�ʱ��Ͽ���������������ڴ˴����ʵ�� */
            break;
        case HILINK_UNINITIALIZED:
            /* HiLink�߳�δ���������ڴ˴����ʵ�� */
            break;
        case HILINK_LINK_UNDER_AUTO_CONFIG:
            /* �豸��������ģʽ�����ڴ˴����ʵ�� */
            break;
        case HILINK_LINK_CONFIG_TIMEOUT:
            /* �豸����10���ӳ�ʱ״̬�����ڴ˴����ʵ�� */
            break;
        case HILINK_LINK_CONNECTTING_WIFI:
            /* �豸��������·���������ڴ˴����ʵ�� */
            break;
        case HILINK_LINK_CONNECTED_WIFI:
            /* �豸�Ѿ�����·���������ڴ˴����ʵ�� */
            break;
        case HILINK_M2M_CONNECTTING_CLOUD:
            /* �豸���������ƶˣ����ڴ˴����ʵ�� */
            break;
        case HILINK_LINK_DISCONNECT:
            /* �豸��·���������ӶϿ������ڴ˴����ʵ�� */
            break;
        case HILINK_DEVICE_REGISTERED:
            /* �豸��ע�ᣬ���ڴ˴����ʵ�� */
            break;
        case HILINK_DEVICE_UNREGISTER:
            /* �豸��������ڴ˴����ʵ�� */
            break;
        case HILINK_REVOKE_FLAG_SET:
            /* �豸����λ�����λ�����ڴ˴����ʵ�� */
            break;
        case HILINK_NEGO_REG_INFO_FAIL:
            /* �豸Э��������Ϣʧ�� */
            break;
        case HILINK_LINK_CONNECTED_FAIL:
            /* �豸��·����������ʧ�� */
            break;
        default:
            break;
    }

    return;
}

/*
 * ʵ��ģ������ǰ���豸����
 * flagΪ0��ʾHiLink SDK �߳̿��Ź�����ģ������; Ϊ1��ʾAPPɾ���豸����ģ������
 * ����0��ʾ����ɹ�, ϵͳ����������ʹ��Ӳ����; ����1��ʾ����ɹ�, ϵͳ����������ʹ��������;
 * ���ظ�ֵ��ʾ����ʧ��, ϵͳ��������
 * ע�⣬�˺������豸����ʵ�֣���APPɾ���豸����ģ������ʱ���豸��������ط���0������ᵼ��ɾ���豸�쳣
 */
int HILINK_ProcessBeforeRestart(int flag)
{
    printf("HILINK_ProcessBeforeRestart start\r\n");

    int ret =-1;
    /* HiLink SDK�߳̿��Ź���ʱ����ģ������ */
    if (flag == 0) {
        /* ʵ��ģ������ǰ�Ĳ���(��:����ϵͳ״̬��) */
        return 0;
    }

    /* APPɾ���豸����ģ������ */
    if (flag == 1) {
        /* ʵ��ģ������ǰ�Ĳ���(��:����ϵͳ״̬��) */
		#if 0
         for(int i=0;i<WIFI_MAX_CONFIG_SIZE;i++)
        {
          ret = RemoveDevice(i);
          if(ret == 0)
            break;
        }
        reboot();
		#endif
        return 0;
    }

    return -1;
}
/*
 * ��ȡ�豸�����ͺţ����ȹ̶������ֽ�
 * subProdIdΪ�������ͺŵĻ�������lenΪ�������ĳ���
 * �����Ʒ���������ͺţ����������ֽ����ͺţ�����'\0'����, ����0
 * û�ж������ͺţ��򷵻�-1
 * �ýӿ����豸������ʵ��
 */
int HILINK_GetSubProdId(char *subProdId, int len)
{
    return -1;
}

#define commandpath  "/usrdata/hilink/command.cfg"
static int HalReadcommand(char *command, unsigned int len)
{
	int commandreadfd;
	int commandfilelen;
	commandreadfd = HalFileOpen(commandpath, O_RDWR_FS, 0);
	if(commandreadfd >= 0)
	{
		HalFileSeek(commandreadfd, 0, SEEK_SET_FS);
		commandfilelen = HalFileRead(commandreadfd,command,len);
		HalFileClose(commandreadfd);
	}
    return EC_SUCCESS;
}
static int HalWritecommand(const char *command, unsigned int len)
{

	int commandreadfd;
	int commandfilelen;

	commandreadfd = HalFileOpen(commandpath, O_CREAT_FS|O_RDWR_FS, 0);
	if(commandreadfd >= 0)
	{
		commandfilelen = HalFileWrite(commandreadfd,command,len);
		HalFileClose(commandreadfd);
	}
    return EC_SUCCESS;
}
#if 0
#define macpath  "/usrdata/hilink/mac.cfg"
static int HalReadMac(char *mac, unsigned int len)
{
	int macreadfd;
	int macfilelen;
	macreadfd = HalFileOpen(macpath, O_RDWR_FS, 0);
	if(macreadfd >= 0){
		HalFileSeek(macreadfd, 0, SEEK_SET_FS);
		macfilelen = HalFileRead(macreadfd,mac,len);
		//printf("ReadMac macfilelen=%d\r\n",macfilelen);
		HalFileClose(macreadfd);
	}else{

	}
    return EC_SUCCESS;
}
static int HalWriteMac(const char *mac, unsigned int len)
{

	int macreadfd;
	int macfilelen;
	//printf("HalWriteToken len=%d--mac=%s\r\n",len,mac);
	macreadfd = HalFileOpen(macpath, O_CREAT_FS|O_RDWR_FS, 0);
	if(macreadfd >= 0)
	{
		macfilelen = HalFileWrite(macreadfd,mac,len);
		//printf("HalWriteToken macfilelen=%d\r\n",macfilelen);
		HalFileClose(macreadfd);
	}
    return EC_SUCCESS;
}
#endif




BLE_AdvPara g_advPara = {
	.advType = 0x00,
	.minInterval = 0x20,
	.maxInterval = 0x40,
	.channelMap = 0x07,
};

static BLE_CfgNetCb g_BLE_CfgNetCb={
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	};


extern void bk_reboot(void);
#define in_range(c, lo, up)  ((unsigned char)c >= lo && (unsigned char)c <= up)
#define isprint(c)           in_range(c, 0x20, 0x7f)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))

void hilink_start(void)
{
	int mode = 0;
	int kit = 0;
	char *buf;
	char *command_buf = os_malloc(256);

	HalReadcommand(command_buf,256);

	//get mode
	if((buf = strstr(command_buf,"wifi"))!=NULL){
		mode = 1;//softAP
	}else if((buf = strstr(command_buf,"ble"))!=NULL){
		mode = 2;//ble
	}
	bk_printf("mode:%d\r\n",mode);

	//get kit 
	if((buf = strstr(command_buf,"kit"))!=NULL){
		kit = 1;
	}
	bk_printf("kit:%d\r\n",kit);

	//get mac
	if((buf = strstr(command_buf,"mac"))!=NULL){
		buf += strlen("mac");
	}else if((buf = strstr(command_buf,"write_mac"))!=NULL){
		buf += strlen("write_mac");
	}
	if(buf){
		char mac[6];
		int i;
		for(i=0;i<5;i++,buf++)
		{
			if(isxdigit(*buf))
				break;
		}
		if(i<5)
		{
			hexstr2bin(buf, mac, 6);
			wifi_set_mac_address((char *)mac);
			os_printf("Set MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
				  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	}

	os_free(command_buf);

	if(mode == 0)
		return;

	//set hilink_task
	HILINK_SdkAttr *sdkAttr = NULL; 
	sdkAttr = HILINK_GetSdkAttr(); 
	if (sdkAttr == NULL) { 
		bk_printf("sdk attr is null\r\n"); 
	} 
	else
	{
		sdkAttr->monitorTaskStackSize = 1024; 
		if(mode == 1){
			sdkAttr->deviceMainTaskStackSize = 1024*6; 
			sdkAttr->bridgeMainTaskStackSize = 1024*6; 
		}else if(mode == 2){
			sdkAttr->deviceMainTaskStackSize = 1024*7; 
			sdkAttr->bridgeMainTaskStackSize = 1024*7; 
		}
		sdkAttr->rebootSoftware = bk_reboot;
		sdkAttr->rebootHardware	= bk_reboot;
		HILINK_SetSdkAttr(*sdkAttr); 
	}

	if(mode == 1)
		HILINK_SetNetConfigMode(HILINK_NETCONFIG_WIFI);
	else if(mode == 2)
		HILINK_SetNetConfigMode(HILINK_NETCONFIG_OTHER);

	if(kit){
		HILINK_EnableKitframework();
	}
	//AuthWorkTask();

	HILINK_Main();

	//for BLE
	BLE_ConfPara isBlePair;
	isBlePair.isBlePair = 0;
	BLE_InitPara initPara;
	//BLE_AdvInfo advInfo;
	initPara.confPara = &isBlePair;
	//advInfo.advPara = &g_advPara;
	//advInfo.advData = &g_advData;
	//initPara.advInfo = &advInfo;
	initPara.advInfo = NULL;
	initPara.gattList = NULL;


	if(mode == 2){
		BLE_CfgNetInit(&initPara,&g_BLE_CfgNetCb);
		BLE_CfgNetAdvCtrl(0xFFFFFFFE);
	}


}

static void hilink_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *command_buf = os_malloc(128);
	os_memset(command_buf, 0, 128);
	if(os_strcmp(argv[0], "hi") == 0){
		os_strcpy(command_buf,"wifi ");
	}else if(os_strcmp(argv[0], "hi_ble") == 0){
		os_strcpy(command_buf,"ble ");
	}else if(os_strcmp(argv[0], "hilink") == 0){
		if(os_strcmp(argv[1], "del") == 0){
			if(strstr(argv[2],"dev")!=NULL){
				for(int i=0;i<WIFI_MAX_CONFIG_SIZE;i++)
				{
					if(RemoveDevice(i) == 0)
						break;
				}
				HILINK_DeleteDevice();
			}
			if(strstr(argv[2],"auth")!=NULL){
				os_printf("OEM Delete 3 files\r\n");
				OEMDeleteAuthStatus();
				OEMDeleteResetFlag();
				OEMDeleteTicket();
			}
			os_free(command_buf);
			return;
		}
	}

	if(argc>1){
		for(int i=1;i<argc;i++){
			strcat(command_buf,argv[i]);
			strcat(command_buf," ");
		}
	}

	HalWritecommand(command_buf,os_strlen(command_buf)+1);
	bk_printf("hilink command_buf:%s\r\n",command_buf);
	os_free(command_buf);

	hilink_start();

}


static void device_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

	if(argv[1] == NULL)
  {
    return ;
  }
   int svc_idx = getServiceIndex(argv[1]);
   if(svc_idx < 0 || svc_idx > SRV_TOTAL_COUNT)
   {
     return;
   }
   
  switch(svc_idx)
  {
    case SRV_IDX_SWITCH:
    {
      if(strcmp(argv[2],"on") == NULL)
      {
        printf("swith_on start\r\n");
      	handle_switch_cmd(true);
      	report_switch_state(true);
      }
      else if(strcmp(argv[2],"off") == NULL)
      {
     	  printf("swith_on_test start\r\n");
      	handle_switch_cmd(false);
      	report_switch_state(false);
      }
    }
    break;
    default:
    break;
  }
  

}


const struct cli_command hilink_clis[] = {
	//{"hi", "hi kit mac XXXXXX", hilink_command},
	//{"hi_ble", "hi_ble kit mac XXXXXX", hilink_command},
	{"hilink", "hilink wifi/ble kit mac XXXXXX", hilink_command},
  {"device", "device switch on/off", device_command},
};

void bk_hilink_cli_init(void)
{
	int ret;

	bk_printf("hilink cli int \r\n");
	ret = cli_register_commands(hilink_clis, sizeof(hilink_clis) / sizeof(struct cli_command));
	if (ret)
		bk_printf("ret: %d hilink commands fail.\r\n",ret);
}




