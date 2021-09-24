#include <string.h>
#include <stdio.h>

#include "ohos_bt_def.h"
#include "ohos_bt_gatt.h"
#include "ohos_bt_gatt_server.h"

#include "uart_pub.h"
#include "include.h"
#include "mem_pub.h"
#include "rtos_pub.h"

static const uint8_t ohos_adv_data[]={0x02,0x01,0x06,0xb,0x9,'7','2','3','1','N','_','B','L','E',0x0};
static const uint8_t ohos_adv_rsp_data[]={0x7,0x8,'7','2','3','1','N',0x0};
static const uint8_t ntf_ind_data[6]={0x11,0x22,0x33,0x44,0x55,0x66};


#define BK_ATT_DECL_PRIMARY_SERVICE     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DECL_CHARACTERISTIC      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define BK_ATT_DESC_CLIENT_CHAR_CFG     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define WRITE_REQ_CHARACTERISTIC        {0x01,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define INDICATE_CHARACTERISTIC         {0x02,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define NOTIFY_CHARACTERISTIC           {0x03,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}


#define BLE_ADV_ENABLE 	1

enum
{
	TEST_IDX_SVC,
	TEST_IDX_FF01_VAL_CHAR,
	TEST_IDX_FF01_VAL_VALUE,
	TEST_IDX_FF02_VAL_CHAR,
	TEST_IDX_FF02_VAL_VALUE,
	TEST_IDX_FF02_VAL_IND_CFG,
	TEST_IDX_FF03_VAL_CHAR,
	TEST_IDX_FF03_VAL_VALUE,
	TEST_IDX_FF03_VAL_NTF_CFG,
	TEST_IDX_NB,
};


BleGattAttr attrList[TEST_IDX_NB]={

	//  Service Declaration
	[TEST_IDX_SVC]              = {OHOS_BLE_ATTRIB_TYPE_SERVICE,    OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, BK_ATT_DECL_PRIMARY_SERVICE,0,0,0,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_FF01_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_FF01_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, WRITE_REQ_CHARACTERISTIC,   0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_READ|OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE,{0}},

	//  Level Characteristic Declaration
	[TEST_IDX_FF02_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_FF02_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, INDICATE_CHARACTERISTIC,    0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,{0}},
	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[TEST_IDX_FF02_VAL_IND_CFG] = {OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG, OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE, OHOS_UUID_TYPE_16_BIT, BK_ATT_DESC_CLIENT_CHAR_CFG,  0,0,0,{0}},

	//  Level Characteristic Declaration
    [TEST_IDX_FF03_VAL_CHAR]    = {OHOS_BLE_ATTRIB_TYPE_CHAR,       OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, BK_ATT_DECL_CHARACTERISTIC, 0,0,0,{0}},
	//  Level Characteristic Value
	[TEST_IDX_FF03_VAL_VALUE]   = {OHOS_BLE_ATTRIB_TYPE_CHAR_VALUE, OHOS_GATT_PERMISSION_READ, OHOS_UUID_TYPE_16_BIT, NOTIFY_CHARACTERISTIC,      0,0,OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY,{0}},
	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[TEST_IDX_FF03_VAL_NTF_CFG] = {OHOS_BLE_ATTRIB_TYPE_CHAR_CLIENT_CONFIG,  OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE, OHOS_UUID_TYPE_16_BIT, BK_ATT_DESC_CLIENT_CHAR_CFG, 0,0,0,{0}},

};

void ohos_advEnable_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_advDisable_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_advData_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_advUpdate_callback(int advId, int status)
{
	bk_printf("fun:%s,advId:%d,status:%d\r\n",__FUNCTION__,advId,status);
}

void ohos_securityRespond_callback(const BdAddr *bdAddr)
{
	bk_printf("fun:%s,line:%d\r\n",__FUNCTION__,__LINE__);
}

void ohos_scanResult_callback(BtScanResultData *scanResultdata)
{
	bk_printf("fun:%s,line:%d\r\n",__FUNCTION__,__LINE__);
}

void ohos_scanParamSet_callback(int clientId, int status)
{
	bk_printf("fun:%s,line:%d,status:%d\r\n",__FUNCTION__,__LINE__,status);
}

void ohos_registerServerCb_callback(int status, int serverId, BtUuid *appUuid)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_connectServerCb_callback(int connId, int serverId, const BdAddr *bdAddr)
{
	bk_printf("fun:%s,serverId:%d,connId:%d\r\n",__FUNCTION__,serverId,connId);
}
	
void ohos_disconnectServerCb_callback(int connId, int serverId, const BdAddr *bdAddr)
{
	bk_printf("fun:%s,serverId:%d,connId:%d\r\n",__FUNCTION__,serverId,connId);
}

void ohos_ServiceAdd_callback(int status, int serverId, BtUuid *appUuid,int srvcHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_characteristicAddCb_callback(int status, int serverId, BtUuid *uuid,int srvcHandle, int characteristicHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_descriptorAddCb_callback(int status, int serverId, BtUuid *uuid,int srvcHandle, int descriptorHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}

void ohos_serviceStartCb_callback(int status, int serverId, int srvcHandle)
{
	bk_printf("fun:%s,serverId:%d,status:%d\r\n",__FUNCTION__,serverId,status);
}


void ohos_requestReadCb_callback(BtReqReadCbPara readCbPara)
{
	bk_printf("fun:%s\r\n",__FUNCTION__);
}

void ohos_requestWriteCb_callback(BtReqWriteCbPara writeCbPara)
{
	bk_printf("fun:%s,peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",__FUNCTION__,writeCbPara.bdAddr->addr[0], writeCbPara.bdAddr->addr[1],writeCbPara.bdAddr->addr[2], writeCbPara.bdAddr->addr[3], writeCbPara.bdAddr->addr[4], writeCbPara.bdAddr->addr[5]);
	for(int index=0;index<writeCbPara.length;index++)
	{
		bk_printf("%02x ",writeCbPara.value[index]);
	}
	bk_printf("\r\n");
}


void ohos_responseConfirmationCb_callback(int status, int handle)
{
	bk_printf("fun:%s,handle:%d,status:%d\r\n",__FUNCTION__,handle,status);
}

void ohos_indicationSentCb_callback(int connId, int status)
{
	bk_printf("fun:%s,serverId:%d,connId:%d\r\n",__FUNCTION__,connId,status);
}

void ble_test_main(void)
{
	/******* init ble stack **********/

	InitBtStack();
#if 1
	EnableBtStack();


	BtGattCallbacks gatt_func={ 							
		.advEnableCb=ohos_advEnable_callback,	
	    .advDisableCb=ohos_advDisable_callback,		
	    .advDataCb=ohos_advData_callback,			
	    .advUpdateCb=ohos_advUpdate_callback,
	    .securityRespondCb=ohos_securityRespond_callback,	
	    .scanResultCb=ohos_scanResult_callback,				
	    .scanParamSetCb=ohos_scanParamSet_callback			
	};


	BleGattRegisterCallbacks(&gatt_func);

#if 1

	BtGattServerCallbacks gatt_server_func={ 							
		.registerServerCb=ohos_registerServerCb_callback,	
	    .connectServerCb=ohos_connectServerCb_callback,		
	    .disconnectServerCb=ohos_disconnectServerCb_callback,			
	    .serviceAddCb=ohos_ServiceAdd_callback,			
	    .characteristicAddCb=ohos_characteristicAddCb_callback,	
	    .descriptorAddCb=ohos_descriptorAddCb_callback,	
	    .serviceStartCb=ohos_serviceStartCb_callback,		
	    .requestReadCb=ohos_requestReadCb_callback,
	    .requestWriteCb=ohos_requestWriteCb_callback,	
	    .responseConfirmationCb=ohos_responseConfirmationCb_callback,				
	    .indicationSentCb=ohos_indicationSentCb_callback				
	};
#endif

	BleGattsRegisterCallbacks(&gatt_server_func);


	/******* create db server **********/
	BtUuid server_uuid;
	server_uuid.uuidLen=2;
	server_uuid.uuid=os_malloc(server_uuid.uuidLen);

	server_uuid.uuid[0]=0xee;
	server_uuid.uuid[1]=0xff;

#if 1
	BleGattsRegister(server_uuid);


	BleGattService srvcInfo;
	srvcInfo.attrNum=9;
	srvcInfo.attrList=attrList;

	BleGattsStartServiceEx(NULL,  &srvcInfo);
#else
	server_uuid.uuid[0]=0xaa;
	server_uuid.uuid[1]=0xbb;
	BleGattsAddService(0,server_uuid,0,9);

	server_uuid.uuid[0]=0x11;
	server_uuid.uuid[1]=0x22;
	BleGattsAddCharacteristic(0,0,server_uuid,OHOS_GATT_CHARACTER_PROPERTY_BIT_READ|OHOS_GATT_CHARACTER_PROPERTY_BIT_WRITE,0);

	server_uuid.uuid[0]=0x33;
	server_uuid.uuid[1]=0x44;
	BleGattsAddCharacteristic(0,0,server_uuid,OHOS_GATT_CHARACTER_PROPERTY_BIT_INDICATE,0);

	server_uuid.uuid[0]=0x02;
	server_uuid.uuid[1]=0x29;
	BleGattsAddDescriptor(0,0,server_uuid,OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE);


	server_uuid.uuid[0]=0x55;
	server_uuid.uuid[1]=0x66;
	BleGattsAddCharacteristic(0,0,server_uuid,OHOS_GATT_CHARACTER_PROPERTY_BIT_NOTIFY,0);


	server_uuid.uuid[0]=0x02;
	server_uuid.uuid[1]=0x29;
	BleGattsAddDescriptor(0,0,server_uuid,OHOS_GATT_PERMISSION_READ|OHOS_GATT_PERMISSION_WRITE);

	BleGattsStartService(0,0);
#endif


#if BLE_ADV_ENABLE

#if 0
	/*****set adv data */
	BleConfigAdvData adv;
	adv.advLength=sizeof(ohos_adv_data);
	adv.scanRspLength=sizeof(ohos_adv_rsp_data);
	adv.advData= malloc(adv.advLength);
	memcpy(adv.advData,ohos_adv_data,adv.advLength);
	adv.scanRspData= malloc(adv.scanRspLength);
	memcpy(adv.scanRspData,ohos_adv_rsp_data,adv.scanRspLength);

	BleSetAdvData(0,&adv);

	/******* start ble adv **********/
	BleAdvParams adv_para;
	adv_para.minInterval=160;
	adv_para.maxInterval=160;
	adv_para.channelMap=7;
	adv_para.duration = 0;

	BleStartAdv(0,&adv_para);

#else
	int advId=0;
		
	StartAdvRawData rawData;
	rawData.advDataLen=sizeof(ohos_adv_data);
	rawData.rspDataLen=sizeof(ohos_adv_rsp_data);
	rawData.advData= os_malloc(rawData.advDataLen);
	memcpy(rawData.advData,ohos_adv_data,rawData.advDataLen);
	rawData.rspData= os_malloc(rawData.rspDataLen);
	memcpy(rawData.rspData,ohos_adv_rsp_data,rawData.rspDataLen);


	BleAdvParams advParam;
	advParam.minInterval=160;
	advParam.maxInterval=160;
	advParam.channelMap=7;
	advParam.duration = 0;

	BleStartAdvEx(&advId, rawData, advParam);
#endif

#endif


#if BLE_SCAN_ENABLE
	BleScanParams scan_param={.scanInterval=100,.scanWindow=50};
	BleSetScanParameters(0, &scan_param);

	BleStartScan();

	rtos_delay_milliseconds(5000);
	BleStopScan();
#endif


	rtos_delay_milliseconds(20000);

	GattsSendIndParam send_param;
	send_param.confirm=1;
	send_param.valueLen=sizeof(ntf_ind_data);
	send_param.value=(char *)ntf_ind_data;
	send_param.attrHandle=TEST_IDX_FF02_VAL_VALUE;

	BleGattsSendIndication(0, &send_param);

	rtos_delay_milliseconds(2000);

	send_param.confirm=0;
	send_param.valueLen=sizeof(ntf_ind_data);
	send_param.value=(char *)ntf_ind_data;
	send_param.attrHandle=TEST_IDX_FF03_VAL_VALUE;

	BleGattsSendIndication(0, &send_param);
#endif

}