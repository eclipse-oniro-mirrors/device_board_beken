#include "wifi_device_util.h"
#include "wifi_device.h"
#include <securec.h>

#include "rw_msg_pub.h"
#include "wlan_ui_pub.h"
#include "param_config.h"
#include "wifi_hotspot_config.h"

static int g_wifiStaStatus = WIFI_STA_NOT_ACTIVE;
static ScanResult_adv scan_apList = {0,0};
static WifiDeviceConfig g_wifiConfigs[WIFI_MAX_CONFIG_SIZE] = {{{0}, {0}, {0}, 0, WIFI_CONFIG_INVALID, 0, 0}};
static WifiEvent * g_wifiEvents[WIFI_MAX_EVENT_SIZE] = {0};

static void DispatchScanStateChangeEvent(const rw_evt_type *evt, const WifiEvent* hosEvent, WifiEventState state)
{
	if(hosEvent->OnWifiScanStateChanged == NULL)
		return;
	int size = 0;
	if(state == WIFI_STATE_AVALIABLE)
	{
		size = scan_apList.ApNum;
	}
	hosEvent->OnWifiScanStateChanged(state, size);
	return;
}
static void DispatchStaConnectEvent(const rw_evt_type *evt, const WifiEvent* hosEvent)
{
	WifiLinkedInfo info = {0};
	
	if(hosEvent->OnWifiConnectionChanged == NULL)
		return;
	
	if(*evt == RW_EVT_STA_GOT_IP)
	{
		GetLinkedInfo(&info);
		hosEvent->OnWifiConnectionChanged(WIFI_STATE_AVALIABLE, &info);
		return;
	}
	if(*evt == RW_EVT_STA_DISCONNECTED)
	{
		hosEvent->OnWifiConnectionChanged(WIFI_STATE_NOT_AVALIABLE, NULL);
	}
	
}

static void DispatchAPConnectEvent(const rw_evt_type *evt, const WifiEvent* hosEvent)
{
	StationInfo info = {0};
	
	if(*evt == RW_EVT_AP_CONNECTED)
	{
		if(hosEvent->OnHotspotStaJoin == NULL)
			return;

		//TODO
		//fill info
		 hosEvent->OnHotspotStaJoin(&info);
		return;
	}

	if(hosEvent->OnHotspotStaLeave == NULL)
		return;

	//TODO
	//fill info
	 hosEvent->OnHotspotStaLeave(&info);
}
static void DispatchEvent(void* BkEvent, const WifiEvent* hosEvent)
{
	rw_evt_type evt = *((rw_evt_type *)BkEvent);
	
	switch(evt)
	{
		case RW_EVT_STA_GOT_IP:
		case RW_EVT_STA_DISCONNECTED:
			 DispatchStaConnectEvent(BkEvent, hosEvent);
			break;

		case RW_EVT_AP_CONNECTED:
		case RW_EVT_AP_DISCONNECTED:
		case RW_EVT_AP_CONNECT_FAILED:
			 DispatchAPConnectEvent(BkEvent, hosEvent);
			break;

		default:
			break;
		//ap started callback implements after ap start func
		
	}
	
}

static void BkWifiEventCb(void *ctx)
{
	if(ctx == NULL)
		return;

	for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
	{
		if(g_wifiEvents[i] == NULL)
			continue;
		DispatchEvent(ctx, g_wifiEvents[i]);
	}
}
static void RegisterBkCallback(void )
{
	bk_wlan_status_register_cb(BkWifiEventCb);
}



static void los_scan_cb(void *ctxt, uint8_t param)
{
	int i;
	
	if(scan_apList.ApList != NULL)
	{
		free(scan_apList.ApList);
		scan_apList.ApList = NULL;
		scan_apList.ApNum = 0;
	}
	
	if (wlan_sta_scan_result(&scan_apList) == 0) 
	{
		printf("Got ap count: %d\r\n", scan_apList.ApNum);
		for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
		{
			if(g_wifiEvents[i] == NULL)
				continue;
			DispatchScanStateChangeEvent(NULL, g_wifiEvents[i], WIFI_STATE_AVALIABLE);
		}
	}
}


//for ap start callback
void DispatchApStartEvent(int state)
{
	for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
	{
		if(g_wifiEvents[i] == NULL)
			continue;
		if(g_wifiEvents[i]->OnHotspotStateChanged == NULL)
			continue;
		g_wifiEvents[i]->OnHotspotStateChanged(state);
	}
}


WifiErrorCode EnableWifi(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    if (g_wifiStaStatus == WIFI_STA_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_BUSY;
    }
	
    g_wifiStaStatus = WIFI_STA_ACTIVE;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    return WIFI_SUCCESS;
}
WifiErrorCode DisableWifi(void)
{
    if (LockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    if (g_wifiStaStatus == WIFI_STA_NOT_ACTIVE) {
        if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
            return ERROR_WIFI_UNKNOWN;
        }
        return ERROR_WIFI_NOT_STARTED;
    }

    g_wifiStaStatus = WIFI_STA_NOT_ACTIVE;
    if (UnlockWifiGlobalLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

    return WIFI_SUCCESS;
}
int IsWifiActive(void)
{
	return g_wifiStaStatus;
}

WifiErrorCode Scan(void)
{
	//if(g_wifiStaStatus == WIFI_STA_NOT_ACTIVE)
		//return ERROR_WIFI_UNKNOWN;
	
    if (LockWifiEventLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }
    for (int i = 0; i < WIFI_MAX_EVENT_SIZE; i++) {
        if (g_wifiEvents[i] == NULL) {
            continue;
        }
        DispatchScanStateChangeEvent(NULL, g_wifiEvents[i], WIFI_STATE_NOT_AVALIABLE);
    }
    if (UnlockWifiEventLock() != WIFI_SUCCESS) {
        return ERROR_WIFI_UNKNOWN;
    }

	mhdr_scanu_reg_cb(los_scan_cb, 0);
	bk_wlan_start_scan();
	
	return WIFI_SUCCESS;
}

WifiErrorCode GetScanInfoList(WifiScanInfo* result, unsigned int* size)
{
	unsigned int i;
	if( (result == NULL) || (size == NULL) || (size == 0) )
		return ERROR_WIFI_INVALID_ARGS;
	
	if(scan_apList.ApNum > *size)
		return ERROR_WIFI_INVALID_ARGS;

	for(i=0;i<scan_apList.ApNum;i++)
	{
		memcpy(result[i].ssid, scan_apList.ApList[i].ssid, WIFI_MAX_SSID_LEN);
		memcpy(result[i].bssid,scan_apList.ApList[i].bssid, WIFI_MAC_LEN);
		result[i].securityType = BKSecToHoSec(scan_apList.ApList[i].security);
		result[i].rssi = scan_apList.ApList[i].ApPower;
		result[i].frequency = ChannelToFrequency(scan_apList.ApList[i].channel);
		result[i].band = HOTSPOT_BAND_TYPE_2G;
	}
	*size = scan_apList.ApNum;
	if(scan_apList.ApList != NULL)
	{
		free(scan_apList.ApList);
		scan_apList.ApList = NULL;
		scan_apList.ApNum = 0;
	}
	return WIFI_SUCCESS;
}
WifiErrorCode AddDeviceConfig(const WifiDeviceConfig* config, int* result)
{

 	if (config == NULL || result == NULL) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	int netId = WIFI_CONFIG_INVALID;
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
		
	for (int i = 0; i < WIFI_MAX_CONFIG_SIZE; i++) 
	{
		if (g_wifiConfigs[i].netId != i) 
		{
			netId = i;
			break;
		}
	}
	if(netId == WIFI_CONFIG_INVALID)
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_BUSY;
	}

	memcpy(&g_wifiConfigs[netId],config,sizeof(WifiDeviceConfig));
	g_wifiConfigs[netId].netId = netId;
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
		
	*result = netId;
	return WIFI_SUCCESS;
	
}

WifiErrorCode GetDeviceConfigs(WifiDeviceConfig* result, unsigned int* size)
{
	if(result == NULL || size == NULL || *size == 0)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	unsigned int retIndex = 0;
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	for(int i=0; i<WIFI_MAX_CONFIG_SIZE; i++)
	{
		if(g_wifiConfigs[i].netId != i)
		{
			continue;
		}
		memcpy(&result[retIndex], &g_wifiConfigs[i], sizeof(WifiDeviceConfig));
		retIndex++;

		if(*size < retIndex)
		{
			if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
			{
				return ERROR_WIFI_UNKNOWN;
			}
			return ERROR_WIFI_INVALID_ARGS;
		}
	}
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if(retIndex == 0)
		return ERROR_WIFI_NOT_AVAILABLE;
	
	*size = retIndex;
	return WIFI_SUCCESS;
}

WifiErrorCode RemoveDevice(int networkId)
{
	if (networkId >= WIFI_MAX_CONFIG_SIZE || networkId < 0) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	memset(&g_wifiConfigs[networkId], 0, sizeof(WifiDeviceConfig)) ;
	g_wifiConfigs[networkId].netId = WIFI_CONFIG_INVALID;
	
	return WIFI_SUCCESS;
}

WifiErrorCode ConnectTo(int networkId)
{
	int chan = 0;
	int ret;
	
	if (networkId >= WIFI_MAX_CONFIG_SIZE || networkId < 0) 
	{
       	return ERROR_WIFI_INVALID_ARGS;
	}
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	if (g_wifiConfigs[networkId].netId != networkId) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
       	return ERROR_WIFI_NOT_AVAILABLE;
	}

	
	network_InitTypeDef_st wNetConfig;
	memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));
	
	wNetConfig.wifi_mode = BK_STATION;
	wNetConfig.dhcp_mode = DHCP_CLIENT;
	wNetConfig.wifi_retry_interval = 100;


	ret = memcpy_s(wNetConfig.wifi_ssid, sizeof(wNetConfig.wifi_ssid),g_wifiConfigs[networkId].ssid, sizeof(g_wifiConfigs[networkId].ssid));
	ret += memcpy_s(wNetConfig.wifi_bssid, sizeof(wNetConfig.wifi_bssid), g_wifiConfigs[networkId].bssid, sizeof(g_wifiConfigs[networkId].bssid));
	ret + memcpy_s(wNetConfig.wifi_key, sizeof(wNetConfig.wifi_key), g_wifiConfigs[networkId].preSharedKey, sizeof(g_wifiConfigs[networkId].preSharedKey)-1);
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if(ret != EOK)
		return ERROR_WIFI_UNKNOWN;
	
	chan = FrequencyToChannel(g_wifiConfigs[networkId].freq);

	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	 
	los_wlan_start_sta(&wNetConfig, g_wifiConfigs[networkId].preSharedKey,64,chan);

	if (UnlockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
//wait for connect result		
	int time_out_cnt = 15;
	WifiLinkedInfo info ={0};
	ret = ERROR_WIFI_NOT_AVAILABLE;
	
	while(time_out_cnt --)
	{
		osDelay(500);//4//1s
		GetLinkedInfo(&info);
		if(info.connState == WIFI_CONNECTED)
		{
			ret = WIFI_SUCCESS;
			break;
		}
	}
	return ret;
}

WifiErrorCode Disconnect(void)
{
	int ret;
	WifiLinkedInfo info ={0};
	
	ret = GetLinkedInfo(&info);
	if(ret != WIFI_SUCCESS)
		return ret;
	
	if(info.connState != WIFI_CONNECTED)
		return ERROR_WIFI_NOT_AVAILABLE;
	bk_wlan_stop(BK_STATION);
	return WIFI_SUCCESS;
}

WifiErrorCode GetLinkedInfo(WifiLinkedInfo* result)
{
	LinkStatusTypeDef linkstat ={0};
	if(result == NULL)
		return ERROR_WIFI_INVALID_ARGS;
	if(bk_wlan_get_link_status(&linkstat) != 0)
		return ERROR_WIFI_UNKNOWN;
	
	memcpy(result->ssid,  linkstat.ssid, sizeof(linkstat.ssid));
	memcpy(result->bssid, linkstat.bssid, WIFI_MAC_LEN);
	if(linkstat.conn_state == RW_EVT_STA_GOT_IP)
	{
		result->connState = WIFI_CONNECTED;
	}
	else
		result->connState = WIFI_DISCONNECTED;
	return WIFI_SUCCESS;
}

WifiErrorCode RegisterWifiEvent(WifiEvent* event)
{
	if(event == NULL)
		return ERROR_WIFI_INVALID_ARGS;

	int emptySlot = WIFI_CONFIG_INVALID;

	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	for(int i=0; i<WIFI_MAX_EVENT_SIZE;i++)
	{
		if(g_wifiEvents[i] == event)
		{
			return ERROR_WIFI_INVALID_ARGS;
		}
		if(g_wifiEvents[i] != NULL)
		{
			continue;
		}
		emptySlot = i;
		break;
	}
	 if (emptySlot == WIFI_CONFIG_INVALID) 
	 {
		if (UnlockWifiEventLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_BUSY;
	 }
	 g_wifiEvents[emptySlot] = event;
	if (UnlockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	} 
	 RegisterBkCallback();
	 return WIFI_SUCCESS;
}

WifiErrorCode UnRegisterWifiEvent(const WifiEvent* event)
{
	if (event == NULL)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	
	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	for(int i=0; i<WIFI_MAX_EVENT_SIZE; i++)
	{
		if(g_wifiEvents[i] != event)
			continue;

		g_wifiEvents[i] = NULL;
		if (UnlockWifiEventLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return WIFI_SUCCESS;
	}
	
	if (UnlockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}	
	return ERROR_WIFI_UNKNOWN;	
}

WifiErrorCode GetDeviceMacAddress(unsigned char* result)
{
	wifi_get_mac_address((char *)result, CONFIG_ROLE_STA);
	return WIFI_SUCCESS;
}


static void AdvScan_Cb(void *ctxt, uint8_t param)
{
	los_scan_cb(ctxt,param);
	set_wifiscan_sema();
}


#define SSID_SCAN_TYPE 0
#define NORMAL_SCAN_TYPE 1
static WifiErrorCode ScanTypeJudge(WifiScanParams *params, uint8_t *BkScanType)
{
	*BkScanType = NORMAL_SCAN_TYPE;
	
	if((params->scanType == WIFI_FREQ_SCAN) && ((params->freqs < 2412 ) || (params->freqs > 2472)))
		return ERROR_WIFI_UNKNOWN;
	
	if( (params->scanType == WIFI_BSSID_SCAN) && 
	    ( (params->bssid[0] == 0)&& (params->bssid[1] == 0)&& (params->bssid[2] == 0)&& (params->bssid[3] == 0)&&
	      (params->bssid[4] == 0)&& (params->bssid[5] == 0)) )
	    return ERROR_WIFI_UNKNOWN;
	
	if((params->scanType == WIFI_SSID_SCAN) &&(params->ssidLen == 0))
		return ERROR_WIFI_UNKNOWN;

	if(params->scanType == WIFI_SSID_SCAN)
		*BkScanType = SSID_SCAN_TYPE;

	return WIFI_SUCCESS;

}

//only support ssid advance scan now TODO
WifiErrorCode AdvanceScan(WifiScanParams *params)
{
	uint8_t **ssid_array;
	uint8_t scan_type;
	WifiErrorCode ret;
	
	if(params == NULL)
		return ERROR_WIFI_UNKNOWN;

	if (LockWifiEventLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	for (int i = 0; i < WIFI_MAX_EVENT_SIZE; i++) 
	{
		if (g_wifiEvents[i] == NULL)
		{
			continue;
		}
		DispatchScanStateChangeEvent(NULL, g_wifiEvents[i], WIFI_STATE_NOT_AVALIABLE);
	}
	if (UnlockWifiEventLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}

	ret = ScanTypeJudge(params, &scan_type);
	if(ret != WIFI_SUCCESS)
		return ret;

	printf("==scan type:%d==\r\n",scan_type);
	if(scan_type== NORMAL_SCAN_TYPE)
	{
		mhdr_scanu_reg_cb(los_scan_cb, 0);
		bk_wlan_start_scan();
	}
	else
	{
		ssid_array = &params->ssid;
		mhdr_scanu_reg_cb(AdvScan_Cb, 0);
		bk_wlan_start_assign_scan(ssid_array, 1);
		if(get_wifiscan_sema() == WIFI_SUCCESS)
		{
			if( scan_apList.ApNum != 0)
				return WIFI_SUCCESS;
		}
		return ERROR_WIFI_UNKNOWN;
	}
	return WIFI_SUCCESS;
}
