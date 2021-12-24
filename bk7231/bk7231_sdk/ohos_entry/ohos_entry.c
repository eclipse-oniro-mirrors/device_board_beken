#include <stdio.h>

//#include "los_context.h"
//#include "los_task.h"

#include "core_main.h"
#include <ohos_init.h>
#include <samgr_lite.h>

#include "cmsis_os2.h"
#include "libc.h"
#include "utils_file.h"
#include "los_memory.h"
#include "rtos_pub.h"
#include "kitframework_client.h"
#include "oem_auth_config.h"
#include "oem_auth_result_storage.h"
#include "ble_cfg_net_api.h"
#include "ohos_bt_gatt_server.h"
void StatusCallback(const AuthStatus* status)
{
    printf("Receiving Status!\n");
    return;
}
#if 1
unsigned int __stack_chk_guard = 0xd00a0dff;
void __stack_chk_fail(VOID)
{
	printf("here you go!!!\n");
    LOS_Panic("stack-protector: Kernel stack is corrupted in: %p\n",
              __builtin_return_address(0));
}
#endif

const char * const FUN_ARG_S = "0123456I";

void HiLogPrintf(uint8 module, uint8 level, const char *nums, const char *fmt, ...)
{
}

#include <stdarg.h>
#include "uart_pub.h"
void __wrap_vprintf(const char *fmt, va_list ap)
{
	char string[128];

	__wrap_vsnprintf(string, sizeof(string) - 1, fmt, ap);
	string[127] = 0;
	bk_send_string(uart_print_port, string);
}


#include "wlan_cli_pub.h"
#include "hilink.h"
#include "ohos_bt_gatt.h"
int HILINK_GetDevSurfacePower (char *power)
{
	return -1;
}

void hilink_bt_test_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	ble_test_main();
}
UINT32 OsGetAllTskInfo(VOID);

extern void bk_token_cli_init(void);
extern void bk_hilink_cli_init(void);

static void hilink_handle(void const *argument)
{
	extended_app_waiting_for_launch();
	bk_token_cli_init();
	bk_hilink_cli_init();
	hilink_start();
}


static int test_task222(void)
{
	osThreadId_t id;
	osThreadAttr_t attr;

    attr.name = "test_task222";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024*2;
    attr.priority = 30;
	
    id = osThreadNew((osThreadFunc_t)hilink_handle, NULL, &attr);
    if (!id) {
        printf("Task [test222] create failed\n");
    } else {
    	printf("Task [test222] create %p\n", id);
    }
	
	return 0;
}

void OHOS_SystemInit(void)
{
	printf("@-OHOS-@\n");
	printf("libc : %s\n", libc_get_version_string());

    MODULE_INIT(bsp);
    MODULE_INIT(device);
    MODULE_INIT(core);
    SYS_INIT(service);
    SYS_INIT(feature);

	MODULE_INIT(app_service);
	MODULE_INIT(app_feature);
    MODULE_INIT(run);
	
    SAMGR_Bootstrap();
	printf("OHOS_SystemInit ok\n");

	test_task222();

    return;
}
