#ifdef IMMVIBESPIAPI#undef IMMVIBESPIAPI#endif#define IMMVIBESPIAPI static#include <linux/platform_device.h>#include <mach/gpio.h>#include <plat/dmtimer.h>#include <linux/regulator/consumer.h>#include <linux/time.h>#define NUM_ACTUATORS 1static bool g_bAmpEnabled = false;#if defined(CONFIG_MACH_LGE_COSMO_REV_A)	#define GPIO_VIB_EN		109#else#define GPIO_VIB_EN		25#endif#define CLK_COUNT 38400000#define MOTOR_RESONANCE_TIMER_ID 8static struct omap_dm_timer *omap_vibrator_timer = NULL;static struct timeval startTime;static struct timeval endTime;int moter_hz_cnt = 0;int moter_hz_index = 0;int moter_hz[5] = {226, 227, 228, 224, 225};unsigned int MOTOR_RESONANCE_COUTER_VALUE;unsigned int PWM_DUTY_MAX;int MOTOR_RESONANCE_HZ;void get_moter_hz(){#if defined(CONFIG_MACH_LGE_COSMO_REV_A) || defined(CONFIG_MACH_LGE_COSMO_REV_B) || defined(CONFIG_MACH_LGE_COSMO_REV_C)		MOTOR_RESONANCE_HZ = 175;	MOTOR_RESONANCE_COUTER_VALUE = (0xFFFFFFFE - ((CLK_COUNT/MOTOR_RESONANCE_HZ)/128));	PWM_DUTY_MAX = ((CLK_COUNT/MOTOR_RESONANCE_HZ)/128);#else	if((moter_hz_cnt%16) == 0){		if(moter_hz_index == 5 )			moter_hz_index = 0;		MOTOR_RESONANCE_HZ = moter_hz[moter_hz_index++];		MOTOR_RESONANCE_COUTER_VALUE = (0xFFFFFFFE - ((CLK_COUNT/MOTOR_RESONANCE_HZ)/128));		PWM_DUTY_MAX = ((CLK_COUNT/MOTOR_RESONANCE_HZ)/128);	}	moter_hz_cnt++;#endif	return;}static void vib_enable(bool enable ){	if (enable)		gpio_set_value(GPIO_VIB_EN, 1);    else 			gpio_set_value(GPIO_VIB_EN, 0);}struct timeval calcConsumedTime(struct timeval z_stStartTime, struct timeval z_stEndTime){	struct timeval    z_stConsumedTime;	z_stConsumedTime.tv_sec = z_stEndTime.tv_sec - z_stStartTime.tv_sec;	z_stConsumedTime.tv_usec = z_stEndTime.tv_usec - z_stStartTime.tv_usec;	if ( z_stConsumedTime.tv_usec < 0 )	{		z_stConsumedTime.tv_sec -= 1;		z_stConsumedTime.tv_usec = (z_stEndTime.tv_usec - z_stStartTime.tv_usec + 1000000);	}	return z_stConsumedTime;}   static void vib_generatePWM(bool enable){	unsigned int PWM_DUTY_HALF;    if(enable)    {		get_moter_hz();		PWM_DUTY_HALF = (0xFFFFFFFF - (PWM_DUTY_MAX >> 1));		omap_dm_timer_enable(omap_vibrator_timer);		omap_dm_timer_set_match(omap_vibrator_timer, 1, PWM_DUTY_HALF);		omap_dm_timer_set_pwm(omap_vibrator_timer, 0, 1, OMAP_TIMER_TRIGGER_OVERFLOW_AND_COMPARE);		omap_dm_timer_set_load_start(omap_vibrator_timer, 1, MOTOR_RESONANCE_COUTER_VALUE);    }	else{		omap_dm_timer_stop(omap_vibrator_timer);			omap_dm_timer_disable(omap_vibrator_timer);	}}IMMVIBESPIAPI VibeStatus ImmVibeSPI_ForceOut_AmpDisable(VibeUInt8 nActuatorIndex){   	if ( g_bAmpEnabled ) {       	g_bAmpEnabled = false;		vib_enable(false);		vib_generatePWM(false);    }    return VIBE_S_SUCCESS;}IMMVIBESPIAPI VibeStatus ImmVibeSPI_ForceOut_AmpEnable(VibeUInt8 nActuatorIndex){	moter_hz_index = 0;	if ( ! g_bAmpEnabled )       {        g_bAmpEnabled = true;		vib_generatePWM(true);		vib_enable(true);		moter_hz_cnt = 0;		moter_hz_index = 0;	 }   	return VIBE_S_SUCCESS;}IMMVIBESPIAPI VibeStatus ImmVibeSPI_ForceOut_Initialize(void){    	gpio_request(GPIO_VIB_EN, "vib_en_gpio");	gpio_direction_output(GPIO_VIB_EN, 1);	gpio_set_value(GPIO_VIB_EN, 0);	omap_vibrator_timer	=	omap_dm_timer_request_specific(MOTOR_RESONANCE_TIMER_ID);	if (omap_vibrator_timer == NULL) {	}	omap_dm_timer_set_source(omap_vibrator_timer, OMAP_TIMER_SRC_SYS_CLK);	omap_dm_timer_disable(omap_vibrator_timer);    return VIBE_S_SUCCESS;}IMMVIBESPIAPI VibeStatus ImmVibeSPI_ForceOut_Terminate(void){   	        ImmVibeSPI_ForceOut_AmpDisable(0);    return VIBE_S_SUCCESS;}IMMVIBESPIAPI VibeStatus ImmVibeSPI_ForceOut_SetSamples(VibeUInt8 nActuatorIndex, VibeUInt16 nOutputSignalBitDepth, VibeUInt16 nBufferSizeInBytes, VibeInt8* pForceOutputBuffer){	unsigned int DUTY_HALF;	get_moter_hz();	DUTY_HALF = (PWM_DUTY_MAX >> 1);#if 1    	unsigned int nTmp;#endif    VibeInt8 nForce;    switch (nOutputSignalBitDepth)    {        case 8:                        if (nBufferSizeInBytes != 1) return VIBE_E_FAIL;            nForce = pForceOutputBuffer[0];            break;        case 16:                        if (nBufferSizeInBytes != 2) return VIBE_E_FAIL;                        nForce = ((VibeInt16*)pForceOutputBuffer)[0] >> 8;            break;        default:                        return VIBE_E_FAIL;    }#if 1	if (nForce > 124) nForce = 127;	if (nForce < -127) nForce = -127;	if (nForce == 0) {		vib_enable(false);	} else {		vib_enable(true);		nTmp = 0xFFFFFFFE - (DUTY_HALF - (DUTY_HALF * nForce /127));				omap_dm_timer_set_match(omap_vibrator_timer, 1, nTmp);		omap_dm_timer_set_load_start(omap_vibrator_timer, 1, MOTOR_RESONANCE_COUTER_VALUE);	}#endif    return VIBE_S_SUCCESS;}IMMVIBESPIAPI VibeStatus ImmVibeSPI_ForceOut_SetFrequency(VibeUInt8 nActuatorIndex, VibeUInt16 nFrequencyParameterID, VibeUInt32 nFrequencyParameterValue){    return VIBE_S_SUCCESS;}IMMVIBESPIAPI VibeStatus ImmVibeSPI_Device_GetName(VibeUInt8 nActuatorIndex, char *szDevName, int nSize){    return VIBE_S_SUCCESS;}