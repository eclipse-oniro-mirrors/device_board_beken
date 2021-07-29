#include "iot_errno.h"
#include "iot_watchdog.h"

#include "BkDriverWdg.h"

static int wdg_inited = 0;

#define DEFAULT_WDG_TIMEOUT_MS		1000

void IoTWatchDogEnable(void)
{
	if (wdg_inited)
		return;

	bk_wdg_initialize(DEFAULT_WDG_TIMEOUT_MS);
	wdg_inited = 1;
}

void IoTWatchDogKick(void)
{
	if (!wdg_inited)
		return;
	
	bk_wdg_reload();
}

void IoTWatchDogDisable(void)
{
	if (!wdg_inited)
		return;

	bk_wdg_finalize();
	wdg_inited = 0;
}

