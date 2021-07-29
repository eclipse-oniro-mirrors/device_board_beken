#include "iot_errno.h"
#include "iot_i2c.h"

#include "i2c_pub.h"
#include "drv_model_pub.h"

struct i2c_ctrl {
	unsigned int inited;
	unsigned int baudrate;
	DD_HANDLE i2c_handle;
};

#define I2C_NUM		2

static struct i2c_ctrl i2c_ctrls[I2C_NUM] = {
	{0, I2C_BAUD_400KHZ, DD_HANDLE_UNVALID},
	{0, I2C_BAUD_400KHZ, DD_HANDLE_UNVALID},
};

unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen)
{
	I2C_OP_ST i2c_op;
	
	if (id > I2C_NUM)
		return IOT_FAILURE;

	if ((deviceAddr == 0) || (data == NULL) || (dataLen == 0))
		return IOT_FAILURE;

	if (!i2c_ctrls[id].inited)
		return IOT_FAILURE;

	i2c_op.op_addr = deviceAddr;
	

	return IOT_SUCCESS;
}

unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen)
{
		return IOT_FAILURE;
}

unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate)
{
		return IOT_FAILURE;
}

unsigned int IoTI2cDeinit(unsigned int id)
{
		return IOT_FAILURE;
}

unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate)
{
		return IOT_FAILURE;
}
