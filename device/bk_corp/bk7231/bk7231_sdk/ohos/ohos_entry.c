#include <stdio.h>

//#include "los_context.h"
//#include "los_task.h"

#include "core_main.h"
#include <ohos_init.h>
#include <samgr_lite.h>

#include "cmsis_os2.h"
#include "libc.h"
extern int wif_test_task(void);

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

    return;
}
