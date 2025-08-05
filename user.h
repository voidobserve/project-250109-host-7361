/******************************************************************************
;  *       @型号                   : MC32F7361
;  *       @创建日期               : 2021.12.21
;  *       @公司/作者              : SINOMCU-FAE
;  *       @晟矽微技术支持         : 2048615934
;  *       @晟矽微官网             : http://www.sinomcu.com/
;  *       @版权                   : 2021 SINOMCU公司版权所有.
;  *---------------------- 建议 ---------------------------------
;  *                   变量定义时使用全局变量
******************************************************************************/
#ifndef USER
#define USER
#include "mc32-common.h"
#include "MC32F7361.h"

/*****************************************************************
;       Function : Define variables
;*****************************************************************/

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long int
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int

#define DEF_SET_BIT0 0x01
#define DEF_SET_BIT1 0x02
#define DEF_SET_BIT2 0x04
#define DEF_SET_BIT3 0x08
#define DEF_SET_BIT4 0x10
#define DEF_SET_BIT5 0x20
#define DEF_SET_BIT6 0x40
#define DEF_SET_BIT7 0x80

#define DEF_CLR_BIT0 0xFE
#define DEF_CLR_BIT1 0xFD
#define DEF_CLR_BIT2 0xFB
#define DEF_CLR_BIT3 0xF7
#define DEF_CLR_BIT4 0xEF
#define DEF_CLR_BIT5 0xDF
#define DEF_CLR_BIT6 0xBF
#define DEF_CLR_BIT7 0x7F

#define FAIL 1
#define PASS 0

#define USE_MY_DEBUG 0
#define AD_OFFSET 41 // 检测到的ad值与实际的电压值有偏差，要减去这个值

#define LED_BLUE_PIN P16D
#define LED_GREEN_PIN P13D 
#define LED_RED_PIN P17D
#define KEY_SCAN_PIN P00D
#if USE_MY_DEBUG
#define CHARGE_SCAN_PIN P05D
#else
#define CHARGE_SCAN_PIN P04D
#endif

/*
	灯光亮度，控制PWM占空比：(数值越小，PWM占空比越小，而驱动灯光的电流越大。灯光低电平点亮)

	数值越小，灯光越亮
	数值越大，灯光越暗
*/ 

// #define LED_RED_LUMINANCE (90) // 红光对应的PWM占空比  
// ==================================================== 
// #define LED_RED_LUMINANCE (89) // 红光对应的PWM占空比   
// ====================================================
// #define LED_RED_LUMINANCE (91) // 红光对应的PWM占空比   
// #define LED_RED_LUMINANCE (92) // 红光对应的PWM占空比   
#define LED_RED_LUMINANCE (93) // 红光对应的PWM占空比   

// #define LED_RED_LUMINANCE (95) // 红光对应的PWM占空比 ==================

// #define LED_BLUE_LUMINANCE (0) // 蓝光对应的PWM占空比 
// #define LED_BLUE_LUMINANCE (5) // 蓝光对应的PWM占空比 
// #define LED_BLUE_LUMINANCE (10) // 蓝光对应的PWM占空比 

// #define LED_BLUE_LUMINANCE (15) // 蓝光对应的PWM占空比 
// #define LED_BLUE_LUMINANCE (20) // 蓝光对应的PWM占空比 
// ====================================================
// #define LED_BLUE_LUMINANCE (30) // 蓝光对应的PWM占空比 
// ====================================================
// #define LED_BLUE_LUMINANCE (32) // 蓝光对应的PWM占空比 
#define LED_BLUE_LUMINANCE (40) // 蓝光对应的PWM占空比 
// #define LED_BLUE_LUMINANCE (45) // 蓝光对应的PWM占空比 
// #define LED_BLUE_LUMINANCE (60) // 蓝光对应的PWM占空比 


// #define LED_RED_LUMINANCE_IN_PURPLE (90) // 紫光时，红灯对应的PWM占空比
// ====================================================
// #define LED_RED_LUMINANCE_IN_PURPLE (89) // 紫光时，红灯对应的PWM占空比
// ====================================================
// #define LED_RED_LUMINANCE_IN_PURPLE (91) // 紫光时，红灯对应的PWM占空比 
// #define LED_RED_LUMINANCE_IN_PURPLE (92) // 紫光时，红灯对应的PWM占空比 
#define LED_RED_LUMINANCE_IN_PURPLE (93) // 紫光时，红灯对应的PWM占空比 
// #define LED_RED_LUMINANCE_IN_PURPLE (95) // 紫光时，红灯对应的PWM占空比 ===================



// #define LED_BLUE_LUMINANCE_IN_PURPLE (0) // 紫光时，蓝灯对应的PWM占空比
// ====================================================
// #define LED_BLUE_LUMINANCE_IN_PURPLE (30) // 紫光时，蓝灯对应的PWM占空比
// ====================================================
#define LED_BLUE_LUMINANCE_IN_PURPLE (40) // 紫光时，蓝灯对应的PWM占空比
// #define LED_BLUE_LUMINANCE_IN_PURPLE (50) // 紫光时，蓝灯对应的PWM占空比
// #define LED_BLUE_LUMINANCE_IN_PURPLE (60) // 紫光时，蓝灯对应的PWM占空比
// #define LED_BLUE_LUMINANCE_IN_PURPLE (65) // 紫光时，蓝灯对应的PWM占空比
// #define LED_BLUE_LUMINANCE_IN_PURPLE (70) // 紫光时，蓝灯对应的PWM占空比
// #define LED_BLUE_LUMINANCE_IN_PURPLE (80) // 紫光时，蓝灯对应的PWM占空比

#define LED_RED_TIMER_DATA (T1DATA) // 驱动红灯的pwm占空比寄存器 TxDATA
#define LED_BLUE_TIMER_DATA (T0DATA) // 驱动蓝灯的pwm占空比寄存器 TxDATA



extern void led_red_on(void);
extern void led_red_off(void);
extern void led_blue_on(void);
extern void led_blue_off(void);

// #define LED_BLUE_ON()         \
// 	{                         \
// 		do                    \
// 		{                     \
// 			LED_BLUE_PIN = 0; \
// 		} while (0);          \
// 	}
// #define LED_BLUE_OFF()        \
// 	{                         \
// 		do                    \
// 		{                     \
// 			LED_BLUE_PIN = 1; \
// 		} while (0);          \
// 	}

#define LED_BLUE_ON()      \
	{                      \
		do                 \
		{                  \
			led_blue_on(); \
		} while (0);       \
	}
#define LED_BLUE_OFF()      \
	{                       \
		do                  \
		{                   \
			led_blue_off(); \
		} while (0);        \
	}
#define LED_GREEN_ON()         \
	{                          \
		do                     \
		{                      \
			LED_GREEN_PIN = 1; \
		} while (0);           \
	}
#define LED_GREEN_OFF()        \
	{                          \
		do                     \
		{                      \
			LED_GREEN_PIN = 0; \
		} while (0);           \
	}

// #define LED_RED_ON()         \
// 	{                        \
// 		do                   \
// 		{                    \
// 			LED_RED_PIN = 0; \
// 		} while (0);         \
// 	}
// #define LED_RED_OFF()        \
// 	{                        \
// 		do                   \
// 		{                    \
// 			LED_RED_PIN = 1; \
// 		} while (0);         \
// 	}

#define LED_RED_ON()      \
	{                     \
		do                \
		{                 \
			led_red_on(); \
		} while (0);      \
	}

#define LED_RED_OFF()      \
	{                      \
		do                 \
		{                  \
			led_red_off(); \
		} while (0);       \
	}

enum
{
	CUR_STATUS_NONE = 0,	// 无操作
	CUR_STATUS_WORKING,		// 正在工作
	CUR_STATUS_BE_CHARGING, // 正在被充电
};
volatile u8 cur_dev_status; // 状态机，表示当前设备的状态

enum
{
	KEY_ID_NONE,
	KEY_ID_BUTTOM,
};
enum
{
	KEY_EVENT_NONE,
	KEY_EVENT_PRESS,
	KEY_EVENT_HOLD,
};

#define KEY_FILTER_TIMES 2
volatile u8 key_event;

volatile u32 power_off_cnt; // 负责自动关机的时间计数

//===============Field Protection Variables===============
volatile u8 abuf;
volatile u8 statusbuf;

volatile u16 adc_val;

//===============Global Variable===============
u8 i; // 循环计数值

enum
{
	LED_MODE_RED = 0,	   // 默认开机是红光
	LED_MODE_BLUE,		   // 蓝
	LED_MODE_RED_AND_BLUE, // 红+蓝
};

volatile u8 led_mode;

// ===================================================
// 主机按键扫描的相关配置                            //
// ===================================================
// static volatile u8 last_key_id;
// static volatile u8 press_cnt;	  // 按键按下的时间计数
// static volatile u8 filter_cnt;	  // 按键消抖，使用的变量
// static volatile u8 filter_key_id; // 按键消抖时使用的变量

// static volatile u8 flag_is_key_mode_hold;

//============Define  Flag=================
typedef union
{
	unsigned char byte;
	struct
	{
		u8 bit0 : 1;
		u8 bit1 : 1;
		u8 bit2 : 1;
		u8 bit3 : 1;
		u8 bit4 : 1;
		u8 bit5 : 1;
		u8 bit6 : 1;
		u8 bit7 : 1;
	} bits;
} bit_flag;
volatile bit_flag flag1;
#define flag_is_dev_open flag1.bits.bit0
#define flag_is_in_charging flag1.bits.bit1
#define flag_is_full_charged flag1.bits.bit2
#define flag_is_power_low flag1.bits.bit3
#define flag_is_enable_into_low_power flag1.bits.bit4 // 标志位，是否使能进入低功耗
#define flag_is_cut_down_charge flag1.bits.bit5		  // 标志位，是否在充电时切断一次给主机电池的充电，以确认是否还有在无线充电

// // 毫秒级延时 (误差：在1%以内，1ms、10ms、100ms延时的误差均小于1%)
// // 前提条件：FCPU = FHOSC / 4
// void delay_ms(u16 xms)
// {
// 	while (xms)
// 	{
// 		u16 i = 572;
// 		while (i--)
// 		{
// 			Nop();
// 		}
// 		xms--; // 把 --操作放在while()判断条件外面，更节省空间

// 		__asm;
// 		clrwdt; // 喂狗
// 		__endasm;
// 	}
// }

// 毫秒级延时,误差在 1% ~ 2%
// 前提条件：FCPU = FHOSC / 8
void delay_ms(u16 xms)
{
	while (xms)
	{
		u16 i = 284;
		while (i--)
		{
			Nop();
		}
		xms--; // 把 --操作放在while()判断条件外面，更节省空间

		__asm;
		clrwdt; // 喂狗
		__endasm;
	}
}

// #if USE_MY_DEBUG
#define DEBUG_PIN P22D
#if 0  // 以下程序约占用81字节空间
// 通过一个引脚输出数据(发送一次约400ms)
// #define DEBUG_PIN P22D
void send_data_msb(u32 send_data)
{
	// 先发送格式头
	// __set_input_pull_up(); // 高电平
	DEBUG_PIN = 1;
	delay_ms(15);
	// __set_output_open_drain(); // 低电平
	DEBUG_PIN = 0;
	delay_ms(7); //

	for (u8 i = 0; i < 32; i++)
	{
		if ((send_data >> (32 - 1 - i)) & 0x01)
		{
			// 如果要发送逻辑1
			// __set_input_pull_up();  	   // 高电平
			DEBUG_PIN = 1;
			delay_ms(5); //
			// __set_output_open_drain(); // 低电平
			DEBUG_PIN = 0;
			delay_ms(10); //
		}
		else
		{
			// 如果要发送逻辑0
			// __set_input_pull_up();  	   // 高电平
			DEBUG_PIN = 1;
			delay_ms(5); //
			// __set_output_open_drain(); // 低电平
			DEBUG_PIN = 0;
			delay_ms(5); //
		}
	}

	// 最后，设置为低电平
	// __set_output_open_drain(); // 低电平
	DEBUG_PIN = 0;
	delay_ms(1);
	DEBUG_PIN = 1;
	delay_ms(1);
	DEBUG_PIN = 0;
}
#endif // #if USE_MY_DEBUG

#endif

/**************************** end of file *********************************************/
