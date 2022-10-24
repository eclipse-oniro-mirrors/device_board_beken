#include <stdio.h>
#include "cmsis_os2.h"
#include "ohos_types.h"
#include "sdkconfig.h"
#include "platform.h"
#include "bk_cli.h"

void HalIrqDisable(uint32 irq_no)
{
	clear_csr(NDS_MIE, (1<<irq_no));
}
void HalIrqEnable(uint32 irq_no)
{
	set_csr(NDS_MIE, (1<<irq_no));
}

int __wrap_vprintf(const char *fmt, va_list ap)
{
	int len;
	char string[128];

	len = __wrap_vsnprintf(string, sizeof(string) - 1, fmt, ap);
	string[127] = 0;
	uart_write_string(CONFIG_UART_PRINT_PORT, string);
	
	return len;
}

__attribute__((section(".interrupt.HalTrapVector.text")))
void HalSetLocalInterPri(UINT32 interPriNum, UINT16 prior)
{
	__nds__plic_set_priority(interPriNum, prior);
}

static int hdf_log_default_level = 4;

int hal_trace_printf(int attr, const char *fmt, ...)
{
	if (attr <= hdf_log_default_level) {
		va_list ap;
		int len;
		char string[128];

		va_start(ap, fmt);
		len = __wrap_vsnprintf(string, sizeof(string) - 1, fmt, ap);
		string[127] = 0;
		uart_write_string(CONFIG_UART_PRINT_PORT, string);
		va_end(ap);		
	}
}


unsigned int __stack_chk_guard = 0xd00a0dff;

void __stack_chk_fail(VOID)
{
	printf("here you go!!!\n");
    printf("add is %lu\n", __builtin_return_address(0));
}

ssize_t _write(int filedes, const void *buf, size_t nbytes)
{
	return 0;
}

