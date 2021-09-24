#include "iot_errno.h"
#include "iot_pwm.h"

#include "BkDriverPwm.h"

unsigned int IoTPwmInit(unsigned int port)
{
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;
	
	return IOT_SUCCESS;
}

unsigned int IoTPwmDeinit(unsigned int port)
{
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;

	return IOT_SUCCESS;
}

unsigned int IoTPwmStart(unsigned int port, unsigned short duty, unsigned int freq)
{
	unsigned int ret;
	
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;
#if (CFG_SOC_NAME != SOC_BK7231N) && (CFG_SOC_NAME != SOC_BK7236)
	ret = bk_pwm_initialize(port,duty,freq);
#else
	ret = bk_pwm_initialize(port, freq, duty, 0, 0);
#endif

	if (ret)
		return IOT_FAILURE;
	
	ret = bk_pwm_start(port);
	if (ret)
		return IOT_FAILURE;
	
	return IOT_SUCCESS;
}

unsigned int IoTPwmStop(unsigned int port)
{
	if(port >= BK_PWM_MAX)
		return IOT_FAILURE;

	bk_pwm_stop(port);
	return IOT_SUCCESS;
}

