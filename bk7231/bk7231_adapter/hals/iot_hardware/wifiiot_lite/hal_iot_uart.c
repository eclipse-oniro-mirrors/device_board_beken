#include "iot_errno.h"
#include "iot_uart.h"

#include "BkDriverUart.h"

struct uart_ctrl {
	unsigned int inited;
};

static struct uart_ctrl uart_ctrls[BK_UART_MAX] = {
	[ 0 ... (BK_UART_MAX-1) ] = {0},
};

static int convert_uart_config(bk_uart_config_t *dst, const IotUartAttribute *src)
{
	dst->baud_rate = src->baudRate;
	
	if (src->dataBits == IOT_UART_DATA_BIT_5)
		dst->data_width = BK_DATA_WIDTH_5BIT;
	else if (src->dataBits == IOT_UART_DATA_BIT_6)
		dst->data_width = BK_DATA_WIDTH_6BIT;
	else if (src->dataBits == IOT_UART_DATA_BIT_7)
		dst->data_width = BK_DATA_WIDTH_7BIT;
	else if (src->dataBits == IOT_UART_DATA_BIT_8)
		dst->data_width = BK_DATA_WIDTH_8BIT;
	else
		return -1;
	
	dst->parity = src->parity ;
	
	if (src->stopBits == IOT_UART_STOP_BIT_1)
		dst->stop_bits = BK_STOP_BITS_1;
	else if (src->stopBits == IOT_UART_STOP_BIT_2)
		dst->stop_bits = BK_STOP_BITS_2;
	else
		return -1;
	
	dst->flow_control = FLOW_CTRL_DISABLED;
	dst->flags = 0;

	return 0;
}

unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param)
{
	unsigned int ret;
	bk_uart_config_t uart_config;
	
	if (id >= BK_UART_MAX)
		return IOT_FAILURE;

	if (uart_ctrls[id].inited)
		return IOT_FAILURE;
	
	ret = convert_uart_config(&uart_config, param);
	if (ret)
		return IOT_FAILURE;

	ret = bk_uart_initialize(id, &uart_config, NULL);
	if (ret)
		return IOT_FAILURE;

	uart_ctrls[id].inited = 1;
	return IOT_SUCCESS;
}

unsigned int IoTUartDeinit(unsigned int id)
{
	if (id >= BK_UART_MAX)
		return IOT_FAILURE;

	uart_ctrls[id].inited = 0;	
	return IOT_SUCCESS;
}

int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen)
{
	unsigned int ret;
	
	if (id >= BK_UART_MAX)
		return IOT_FAILURE;
	if ((data == NULL) || (dataLen == 0))
		return IOT_FAILURE;

	if (!uart_ctrls[id].inited)
		return IOT_FAILURE;

	ret = bk_uart_get_length_in_buffer(id);
	if (ret < dataLen)
		dataLen = ret;
	
	ret = bk_uart_recv(id, data, dataLen, 0);
	if (ret)
		return IOT_FAILURE;

	return dataLen;
}

int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen)
{
	unsigned int ret;

	if (id >= BK_UART_MAX)
		return IOT_FAILURE;
	if ((data == NULL) || (dataLen == 0))
		return IOT_FAILURE;

	if (!uart_ctrls[id].inited)
		return IOT_FAILURE;

	ret = bk_uart_send(id, data, dataLen);
	if (ret)
		return IOT_FAILURE;

	return dataLen;	
}

unsigned int IoTUartSetFlowCtrl(unsigned int id, IotFlowCtrl flowCtrl)
{
	return IOT_FAILURE;	//TODO
}

