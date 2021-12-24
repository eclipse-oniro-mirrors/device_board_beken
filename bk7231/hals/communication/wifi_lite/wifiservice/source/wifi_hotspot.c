#include "wifi_hotspot.h"
#include "wifi_device_util.h"
#include "wifi_ui_extra.h"

#define WLAN_DEFAULT_IP         "192.168.0.1"
#define WLAN_DEFAULT_GW         "192.168.0.1"
#define WLAN_DEFAULT_MASK       "255.255.255.0"

#define RSSI_LEVEL_4_2_G (-65)
#define RSSI_LEVEL_3_2_G (-75)
#define RSSI_LEVEL_2_2_G (-82)
#define RSSI_LEVEL_1_2_G (-88)

#define RSSI_LEVEL_4_5_G (-65)
#define RSSI_LEVEL_3_5_G (-72)
#define RSSI_LEVEL_2_5_G (-79)
#define RSSI_LEVEL_1_5_G (-85)


static int g_wifiApStatus = WIFI_HOTSPOT_NOT_ACTIVE;
static bool g_wifiApCfgFlag = 0;
static unsigned char g_band = 0;

extern void DispatchApStartEvent(int state);
WifiErrorCode EnableHotspot(void)
{
	unsigned char security,chann;
	network_InitTypeDef_st ap_cfg = {0};
	
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_ACTIVE)
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_BUSY;
	}

	if(g_wifiApCfgFlag != 1)
		return ERROR_WIFI_NOT_AVAILABLE;
	los_wlan_ap_para_info_get(&ap_cfg, &security, &chann);
	
		
	ap_cfg.wifi_mode = BK_SOFT_AP;
	ap_cfg.dhcp_mode = DHCP_SERVER;
	ap_cfg.wifi_retry_interval = 100;
	strcpy((char *)ap_cfg.local_ip_addr, WLAN_DEFAULT_IP);
	strcpy((char *)ap_cfg.net_mask, WLAN_DEFAULT_MASK);
	strcpy((char *)ap_cfg.gateway_ip_addr, WLAN_DEFAULT_GW);
	strcpy((char *)ap_cfg.dns_server_ip_addr, WLAN_DEFAULT_GW);

	los_wlan_start_ap(&ap_cfg, security, chann);
	
	g_wifiApStatus = WIFI_HOTSPOT_ACTIVE;
	DispatchApStartEvent(WIFI_STATE_AVALIABLE);
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	return WIFI_SUCCESS;
}

WifiErrorCode DisableHotspot(void)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}

		return ERROR_WIFI_NOT_STARTED;
	}

	bk_wlan_stop(BK_SOFT_AP);
	
	DispatchApStartEvent(WIFI_STATE_NOT_AVALIABLE);
	
	g_wifiApStatus = WIFI_HOTSPOT_NOT_ACTIVE;
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}
	
	return WIFI_SUCCESS;
}

WifiErrorCode SetHotspotConfig(const HotspotConfig* config)
{
	if (config == NULL)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	network_InitTypeDef_st ap_info={0};
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	memcpy(ap_info.wifi_ssid, config->ssid,WIFI_MAX_SSID_LEN - 1);
	memcpy(ap_info.wifi_key, config->preSharedKey,WIFI_MAX_KEY_LEN - 1);
	los_wlan_ap_para_info_set(&ap_info, HoSecToBKSec(config->securityType),11 /*config->channelNum*/ );
	g_wifiApCfgFlag = 1;
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}

	return WIFI_SUCCESS;
}

WifiErrorCode GetHotspotConfig(HotspotConfig* result)
{
	network_InitTypeDef_st ap_info={0};
	unsigned char security,chann;
	
	if (result == NULL)
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}

	los_wlan_ap_para_info_get(&ap_info,&security,&chann);
	memcpy(result->preSharedKey, ap_info.wifi_key, WIFI_MAX_KEY_LEN - 1);
	memcpy(result->ssid, ap_info.wifi_ssid, WIFI_MAX_SSID_LEN - 1);
	result->channelNum = chann;
	result->securityType = BKSecToHoSec(security);
	result->band =g_band;
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	return WIFI_SUCCESS;
}

int IsHotspotActive(void)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	int ret = g_wifiApStatus;
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}

	return ret;
}

WifiErrorCode GetStationList(StationInfo* result, unsigned int* size)
{
	if (result == NULL || size == NULL || *size == 0) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}

	//TODO

	return WIFI_SUCCESS;
}

WifiErrorCode SetBand(int band)
{
	if(band != HOTSPOT_BAND_TYPE_2G)
		return ERROR_WIFI_NOT_SUPPORTED;
	g_band = band;
	return 	WIFI_SUCCESS;
}

WifiErrorCode GetBand(int* result)
{
	if (result == NULL) 
	{
		return ERROR_WIFI_INVALID_ARGS;
	}
	if(g_band == 0)
		return ERROR_WIFI_NOT_AVAILABLE;
	
	*result = HOTSPOT_BAND_TYPE_2G;
	return WIFI_SUCCESS;
}

int GetSignalLevel(int rssi, int band)
{

	if (band == HOTSPOT_BAND_TYPE_5G) 
	{
		if (rssi >= RSSI_LEVEL_4_5_G) 
		{
			return RSSI_LEVEL_4;
		}
		if (rssi >= RSSI_LEVEL_3_5_G) 
		{
			return RSSI_LEVEL_3;
		}
		if (rssi >= RSSI_LEVEL_2_5_G) 
		{
			return RSSI_LEVEL_2;
		}
		if (rssi >= RSSI_LEVEL_1_5_G) 
		{
			return RSSI_LEVEL_1;
		}
	}
	if(band == HOTSPOT_BAND_TYPE_2G)
	{

		if (rssi >= RSSI_LEVEL_4_2_G) 
		{
			return RSSI_LEVEL_4;
		}
		if (rssi >= RSSI_LEVEL_3_2_G) 
		{
			return RSSI_LEVEL_3;
		}
		if (rssi >= RSSI_LEVEL_2_2_G) 
		{
			return RSSI_LEVEL_2;
		}
		if (rssi >= RSSI_LEVEL_1_2_G)
		{
			return RSSI_LEVEL_1;
		}
	}

	return ERROR_WIFI_INVALID_ARGS;
}

int GetHotspotChannel(void)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
		{
			return ERROR_WIFI_UNKNOWN;
		}

		return ERROR_WIFI_NOT_STARTED;
	}

	int channel = bk_wlan_ap_get_channel_config();
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	return channel;
}

WifiErrorCode GetHotspotInterfaceName(char* result, int size)
{
	if (LockWifiGlobalLock() != WIFI_SUCCESS) 
	{
		return ERROR_WIFI_UNKNOWN;
	}
	if (g_wifiApStatus == WIFI_HOTSPOT_NOT_ACTIVE) 
	{
		if (UnlockWifiGlobalLock() != WIFI_SUCCESS) 
		{
			return ERROR_WIFI_UNKNOWN;
		}
		return ERROR_WIFI_NOT_STARTED;
	}
	HotspotConfig ap_config = {0};
	GetHotspotConfig(&ap_config);
	memcpy(result, ap_config.ssid, size);
	
	if (UnlockWifiGlobalLock() != WIFI_SUCCESS)
	{
		return ERROR_WIFI_UNKNOWN;
	}
	return WIFI_SUCCESS;
}

void test_setHotspot(char *ssid,char *key,char security)
{
	HotspotConfig cfg={0};
	if(ssid == NULL)
		return;
	
	memcpy(cfg.ssid, ssid, strlen(ssid));
	if(key != NULL)
	{
		memcpy(cfg.preSharedKey,key,strlen(key));
		if(security > WIFI_SEC_TYPE_SAE)
			security = WIFI_SEC_TYPE_OPEN;
		cfg.securityType = security;
	}
	else
	{
		cfg.securityType = WIFI_SEC_TYPE_OPEN;
	}
	cfg.channelNum = 11;//default
	SetHotspotConfig(&cfg);
}
