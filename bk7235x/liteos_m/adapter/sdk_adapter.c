// Copyright (C) 2022 Beken Corporation
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include "cmsis_os2.h"
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

void bk_wrap__ctype_ptr__(void)
{
}
void HAL_NVIC_SystemReset(void)
{
}

#define vprintf_buf_len 128
int bk_wrap_vprintf(const char *fmt, va_list ap)
{
    int len;
    char string[vprintf_buf_len];

    len = __wrap_vsnprintf(string, sizeof(string) - 1, fmt, ap);
    string[vprintf_buf_len-1] = 0;
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
        char string[vprintf_buf_len];

        va_start(ap, fmt);
        len = __wrap_vsnprintf(string, sizeof(string) - 1, fmt, ap);
        string[sizeof(string)-1] = 0;
        uart_write_string(CONFIG_UART_PRINT_PORT, string);
        va_end(ap);
    }
}

ssize_t _write(int filedes, const void *buf, size_t nbytes)
{
    return 0;
}

