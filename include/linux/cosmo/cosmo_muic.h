#ifndef _MUIC_H_#define _MUIC_H_#if defined(CONFIG_MACH_LGE_COSMO_REV_A)#define MUIC_INT 2#else#define MUIC_INT 7#endif#define DP3T_IN_1_GPIO	11 		#define DP3T_IN_2_GPIO	12 		#define USIF_IN_1_GPIO	165 	#define IFX_USB_VBUS_EN_GPIO 55	#define TD_INT_STAT	70000	#define TD_STATUS	250000	#define TD_DP_DM	1000	#define	DEVICE_ID	(u8)0x00#define	CONTROL_1	(u8)0x01#define	CONTROL_2	(u8)0x02#define	SW_CONTROL	(u8)0x03#define	INT_STAT	(u8)0x04#if defined (CONFIG_MACH_LGE_CX2)#define	STATUS		(u8)0x06#else#define	STATUS		(u8)0x05#endif#define	ID_2P2		(u8)0x40#define	ID_620		(u8)0x20#define	ID_200		(u8)0x10#if defined (CONFIG_MACH_LGE_CX2)#define VLDO	(u8)0x00	#else#define VLDO	(u8)0x08	#endif#define	SEMREN		(u8)0x04#define	ADC_EN		(u8)0x02#define	CP_EN		(u8)0x01#define	INTPOL		(u8)0x80#define	INT_EN		(u8)0x40#define	MIC_LP		(u8)0x20#define	CP_AUD		(u8)0x10#define	CHG_TYPE	(u8)0x02#define	USB_DET_DIS	(u8)0x01#define	MIC_ON		(u8)0x40#define DP			(u8)0x38#define DM			(u8)0x07#define DP_USB		(u8)0x00#define	DP_UART		(u8)0x08#define	DP_AUDIO	(u8)0x10#define	DP_OPEN		(u8)0x38#define DM_USB		(u8)0x00#define	DM_UART		(u8)0x01#define	DM_AUDIO	(u8)0x02#define	DM_OPEN		(u8)0x07#define USB		DP_USB   | DM_USB 	#define UART 	DP_UART  | DM_UART 	#define AUDIO	DP_AUDIO | DM_AUDIO #define OPEN	DP_OPEN  | DM_OPEN 	#define	CHGDET		(u8)0x80#define	MR_COMP		(u8)0x40#define	SENDEND		(u8)0x20#define	VBUS		(u8)0x10#define	IDNO		(u8)0x0f#define	DCPORT		(u8)0x80#define	CHPORT		(u8)0x40#define C1COMP      (u8)0x01	#define IDNO_0000     0x00#define IDNO_0001     0x01#define IDNO_0010     0x02#define IDNO_0011     0x03#define IDNO_0100     0x04#define IDNO_0101     0x05#define IDNO_0110     0x06#define IDNO_0111     0x07#define IDNO_1000     0x08#define IDNO_1001     0x09#define IDNO_1010     0x0A#define IDNO_1011     0x0B#define COMP2_TO_DP2  	0x00#define COMP2_TO_U2   	0x08#define COMP2_TO_AUD2 	0x10#define COMP2_TO_HZ   	0x20#define COMN1_TO_DN1  	0x00#define COMN1_TO_U1   	0x01#define COMN1_TO_AUD1 	0x02#define COMN1_TO_C1COMP 0x03#define COMN1_TO_HZ   	0x04#define TS5USBA33402 	0x10#define MAX14526 		0x20#define ANY_VENDOR 		0xfftypedef enum {	MUIC_UNKNOWN,				MUIC_NONE,					MUIC_NA_TA,					MUIC_LG_TA,					MUIC_TA_1A,					MUIC_INVALID_CHG,			MUIC_AP_UART,				MUIC_CP_UART,				MUIC_AP_USB,				MUIC_CP_USB,			#if defined (CONFIG_CX2_MHL_TX_SII9244) 	MUIC_MHL,				#endif} TYPE_MUIC_MODE;typedef enum {	CHARGING_UNKNOWN,	CHARGING_NONE,	CHARGING_NA_TA,	CHARGING_LG_TA,	CHARGING_TA_1A,	CHARGING_INVALID_CHG,	CHARGING_USB,	CHARGING_FACTORY,} TYPE_CHARGING_MODE;typedef enum {	USIF_AP,			USIF_DP3T,		} TYPE_USIF_MODE;typedef enum {	DP3T_NC,			DP3T_AP_UART,		DP3T_CP_UART,		DP3T_CP_USB,	} TYPE_DP3T_MODE;typedef enum {	NOT_UPON_IRQ,		UPON_IRQ,		} TYPE_UPON_IRQ;typedef enum {	DEFAULT,		RESET,		} TYPE_RESET;typedef enum {	NO_RETAIN,	RETAIN,	BOOT_AP_USB,	BOOT_CP_USB,	BOOT_FIRST,} TYPE_RETAIN_MODE;extern const char *muic_path_str[];extern const char *charging_mode_str[];extern TYPE_MUIC_MODE muic_path;extern TYPE_CHARGING_MODE charging_mode;extern atomic_t muic_charger_detected;extern void muic_mdelay(u32 microsec);extern s32 muic_i2c_read_byte(u8 addr, u8 *value);extern s32 muic_i2c_write_byte(u8 addr, u8 value);extern void check_charging_mode(void);TYPE_MUIC_MODE get_muic_mode(void);int get_muic_charger_detected(void);void set_muic_charger_detected(void);void dp3t_switch_ctrl(TYPE_DP3T_MODE mode);void usif_switch_ctrl(TYPE_USIF_MODE mode);#endif