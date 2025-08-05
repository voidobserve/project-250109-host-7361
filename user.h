/******************************************************************************
;  *       @�ͺ�                   : MC32F7361
;  *       @��������               : 2021.12.21
;  *       @��˾/����              : SINOMCU-FAE
;  *       @����΢����֧��         : 2048615934
;  *       @����΢����             : http://www.sinomcu.com/
;  *       @��Ȩ                   : 2021 SINOMCU��˾��Ȩ����.
;  *---------------------- ���� ---------------------------------
;  *                   ��������ʱʹ��ȫ�ֱ���
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
#define AD_OFFSET 41 // ��⵽��adֵ��ʵ�ʵĵ�ѹֵ��ƫ�Ҫ��ȥ���ֵ

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
	�ƹ����ȣ�����PWMռ�ձȣ�(��ֵԽС��PWMռ�ձ�ԽС���������ƹ�ĵ���Խ�󡣵ƹ�͵�ƽ����)

	��ֵԽС���ƹ�Խ��
	��ֵԽ�󣬵ƹ�Խ��
*/ 

// #define LED_RED_LUMINANCE (90) // ����Ӧ��PWMռ�ձ�  
// ==================================================== 
// #define LED_RED_LUMINANCE (89) // ����Ӧ��PWMռ�ձ�   
// ====================================================
// #define LED_RED_LUMINANCE (91) // ����Ӧ��PWMռ�ձ�   
// #define LED_RED_LUMINANCE (92) // ����Ӧ��PWMռ�ձ�   
#define LED_RED_LUMINANCE (93) // ����Ӧ��PWMռ�ձ�   

// #define LED_RED_LUMINANCE (95) // ����Ӧ��PWMռ�ձ� ==================

// #define LED_BLUE_LUMINANCE (0) // �����Ӧ��PWMռ�ձ� 
// #define LED_BLUE_LUMINANCE (5) // �����Ӧ��PWMռ�ձ� 
// #define LED_BLUE_LUMINANCE (10) // �����Ӧ��PWMռ�ձ� 

// #define LED_BLUE_LUMINANCE (15) // �����Ӧ��PWMռ�ձ� 
// #define LED_BLUE_LUMINANCE (20) // �����Ӧ��PWMռ�ձ� 
// ====================================================
// #define LED_BLUE_LUMINANCE (30) // �����Ӧ��PWMռ�ձ� 
// ====================================================
// #define LED_BLUE_LUMINANCE (32) // �����Ӧ��PWMռ�ձ� 
#define LED_BLUE_LUMINANCE (40) // �����Ӧ��PWMռ�ձ� 
// #define LED_BLUE_LUMINANCE (45) // �����Ӧ��PWMռ�ձ� 
// #define LED_BLUE_LUMINANCE (60) // �����Ӧ��PWMռ�ձ� 


// #define LED_RED_LUMINANCE_IN_PURPLE (90) // �Ϲ�ʱ����ƶ�Ӧ��PWMռ�ձ�
// ====================================================
// #define LED_RED_LUMINANCE_IN_PURPLE (89) // �Ϲ�ʱ����ƶ�Ӧ��PWMռ�ձ�
// ====================================================
// #define LED_RED_LUMINANCE_IN_PURPLE (91) // �Ϲ�ʱ����ƶ�Ӧ��PWMռ�ձ� 
// #define LED_RED_LUMINANCE_IN_PURPLE (92) // �Ϲ�ʱ����ƶ�Ӧ��PWMռ�ձ� 
#define LED_RED_LUMINANCE_IN_PURPLE (93) // �Ϲ�ʱ����ƶ�Ӧ��PWMռ�ձ� 
// #define LED_RED_LUMINANCE_IN_PURPLE (95) // �Ϲ�ʱ����ƶ�Ӧ��PWMռ�ձ� ===================



// #define LED_BLUE_LUMINANCE_IN_PURPLE (0) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// ====================================================
// #define LED_BLUE_LUMINANCE_IN_PURPLE (30) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// ====================================================
#define LED_BLUE_LUMINANCE_IN_PURPLE (40) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// #define LED_BLUE_LUMINANCE_IN_PURPLE (50) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// #define LED_BLUE_LUMINANCE_IN_PURPLE (60) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// #define LED_BLUE_LUMINANCE_IN_PURPLE (65) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// #define LED_BLUE_LUMINANCE_IN_PURPLE (70) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�
// #define LED_BLUE_LUMINANCE_IN_PURPLE (80) // �Ϲ�ʱ�����ƶ�Ӧ��PWMռ�ձ�

#define LED_RED_TIMER_DATA (T1DATA) // ������Ƶ�pwmռ�ձȼĴ��� TxDATA
#define LED_BLUE_TIMER_DATA (T0DATA) // �������Ƶ�pwmռ�ձȼĴ��� TxDATA



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
	CUR_STATUS_NONE = 0,	// �޲���
	CUR_STATUS_WORKING,		// ���ڹ���
	CUR_STATUS_BE_CHARGING, // ���ڱ����
};
volatile u8 cur_dev_status; // ״̬������ʾ��ǰ�豸��״̬

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

volatile u32 power_off_cnt; // �����Զ��ػ���ʱ�����

//===============Field Protection Variables===============
volatile u8 abuf;
volatile u8 statusbuf;

volatile u16 adc_val;

//===============Global Variable===============
u8 i; // ѭ������ֵ

enum
{
	LED_MODE_RED = 0,	   // Ĭ�Ͽ����Ǻ��
	LED_MODE_BLUE,		   // ��
	LED_MODE_RED_AND_BLUE, // ��+��
};

volatile u8 led_mode;

// ===================================================
// ��������ɨ����������                            //
// ===================================================
// static volatile u8 last_key_id;
// static volatile u8 press_cnt;	  // �������µ�ʱ�����
// static volatile u8 filter_cnt;	  // ����������ʹ�õı���
// static volatile u8 filter_key_id; // ��������ʱʹ�õı���

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
#define flag_is_enable_into_low_power flag1.bits.bit4 // ��־λ���Ƿ�ʹ�ܽ���͹���
#define flag_is_cut_down_charge flag1.bits.bit5		  // ��־λ���Ƿ��ڳ��ʱ�ж�һ�θ�������صĳ�磬��ȷ���Ƿ��������߳��

// // ���뼶��ʱ (����1%���ڣ�1ms��10ms��100ms��ʱ������С��1%)
// // ǰ��������FCPU = FHOSC / 4
// void delay_ms(u16 xms)
// {
// 	while (xms)
// 	{
// 		u16 i = 572;
// 		while (i--)
// 		{
// 			Nop();
// 		}
// 		xms--; // �� --��������while()�ж��������棬����ʡ�ռ�

// 		__asm;
// 		clrwdt; // ι��
// 		__endasm;
// 	}
// }

// ���뼶��ʱ,����� 1% ~ 2%
// ǰ��������FCPU = FHOSC / 8
void delay_ms(u16 xms)
{
	while (xms)
	{
		u16 i = 284;
		while (i--)
		{
			Nop();
		}
		xms--; // �� --��������while()�ж��������棬����ʡ�ռ�

		__asm;
		clrwdt; // ι��
		__endasm;
	}
}

// #if USE_MY_DEBUG
#define DEBUG_PIN P22D
#if 0  // ���³���Լռ��81�ֽڿռ�
// ͨ��һ�������������(����һ��Լ400ms)
// #define DEBUG_PIN P22D
void send_data_msb(u32 send_data)
{
	// �ȷ��͸�ʽͷ
	// __set_input_pull_up(); // �ߵ�ƽ
	DEBUG_PIN = 1;
	delay_ms(15);
	// __set_output_open_drain(); // �͵�ƽ
	DEBUG_PIN = 0;
	delay_ms(7); //

	for (u8 i = 0; i < 32; i++)
	{
		if ((send_data >> (32 - 1 - i)) & 0x01)
		{
			// ���Ҫ�����߼�1
			// __set_input_pull_up();  	   // �ߵ�ƽ
			DEBUG_PIN = 1;
			delay_ms(5); //
			// __set_output_open_drain(); // �͵�ƽ
			DEBUG_PIN = 0;
			delay_ms(10); //
		}
		else
		{
			// ���Ҫ�����߼�0
			// __set_input_pull_up();  	   // �ߵ�ƽ
			DEBUG_PIN = 1;
			delay_ms(5); //
			// __set_output_open_drain(); // �͵�ƽ
			DEBUG_PIN = 0;
			delay_ms(5); //
		}
	}

	// �������Ϊ�͵�ƽ
	// __set_output_open_drain(); // �͵�ƽ
	DEBUG_PIN = 0;
	delay_ms(1);
	DEBUG_PIN = 1;
	delay_ms(1);
	DEBUG_PIN = 0;
}
#endif // #if USE_MY_DEBUG

#endif

/**************************** end of file *********************************************/
