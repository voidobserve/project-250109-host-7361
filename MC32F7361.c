/******************************************************************************
;  *       @�ͺ�                 : MC32F7361
;  *       @��������             : 2021.12.21
;  *       @��˾/����            : SINOMCU-FAE
;  *       @����΢����֧��       : 2048615934
;  *       @����΢����           : http://www.sinomcu.com/
;  *       @��Ȩ                 : 2021 SINOMCU��˾��Ȩ����.
;  *----------------------ժҪ����---------------------------------
;  *
******************************************************************************/

#include "user.h"

/************************************************
;  *    @������          : CLR_RAM
;  *    @˵��            : ��RAM
;  *    @�������        :
;  *    @���ز���        :
;  ***********************************************/
void CLR_RAM(void)
{
    for (FSR0 = 0; FSR0 < 0xff; FSR0++)
    {
        INDF0 = 0x00;
    }
    FSR0 = 0xFF;
    INDF0 = 0x00;
}
/************************************************
;  *    @������            : IO_Init
;  *    @˵��              : IO��ʼ��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void IO_Init(void)
{
    IOP0 = 0x00;   // io������λ
    OEP0 = 0x3F;   // io�ڷ��� 1:out  0:in
    PUP0 = 0x00;   // io����������   1:enable  0:disable
    PDP0 = 0x00;   // io����������   1:enable  0:disable
    P0ADCR = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

    // IOP1 = 0x00;   // io������λ
    IOP1 = 0xD0;   // ������LED
    OEP1 = 0xFF;   // io�ڷ��� 1:out  0:in
    PUP1 = 0x00;   // io����������   1:enable  0:disable
    PDP1 = 0x00;   // io����������   1:enable  0:disable
    P1ADCR = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

    IOP2 = 0x00; // io������λ
    OEP2 = 0x0F; // io�ڷ��� 1:out  0:in
    PUP2 = 0x00; // io����������   1:enable  0:disable
    PDP2 = 0x00; // io����������   1:enable  0:disablea

    PMOD = 0x00;  // P00��P01��P13 io�˿�ֵ�ӼĴ��������������
    // DRVCR = 0xB0; // P16��P17�����������100mA
    // DRVCR = 0x90; // P16��P17�����������50mA
    DRVCR = 0x80; // P16��P17�����������25mA��������ͨIO���������������
}

/************************************************
;  *    @������            : ADC_Init
;  *    @˵��              : ADC��ʼ��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void adc_config(void)
{
    ADCR0 = 0xAB; // 12λ���ȣ�ʹ��adc,adcͨ��ѡ��  1/4 VDD
    ADCR1 = 0x80; // adcת��ʱ��ѡ�� FHIRC/32��ʹ�ÃȲ�2.0V�ο���ѹ
    ADCR2 = 0x0F; // ����ʱ�䣬ֻ�̶ܹ���15 �� ADCLK
}

// ��ȡadc����ת�����ֵ
u16 adc_get_val(void)
{
    u8 i = 0; // adc�ɼ������ļ���
    u16 g_temp_value = 0;
    u32 g_tmpbuff = 0;
    u16 g_adcmax = 0;
    u16 g_adcmin = 0xFFFF;

    // �ɼ�20�Σ�ȥ��ǰ���β�������ȥ��һ�����ֵ��һ����Сֵ����ȡƽ��ֵ
    for (i = 0; i < 20; i++)
    {
        ADEOC = 0; // ���ADCת����ɱ�־λ������ADת��
        while (!ADEOC)
            ;                // �ȴ�ת�����
        g_temp_value = ADRH; // ȡ��ת�����ֵ
        g_temp_value = g_temp_value << 4 | (ADRL & 0x0F);
        if (i < 2)
            continue; // ����ǰ���β�����
        if (g_temp_value > g_adcmax)
            g_adcmax = g_temp_value; // ���
        if (g_temp_value < g_adcmin)
            g_adcmin = g_temp_value; // ��С
        g_tmpbuff += g_temp_value;
    }
    g_tmpbuff -= g_adcmax;           // ȥ��һ�����
    g_tmpbuff -= g_adcmin;           // ȥ��һ����С
    g_temp_value = (g_tmpbuff >> 4); // ����16��ȡƽ��ֵ

    return g_temp_value;
}

// ��ȡadc����ת�����ֵ
u16 adc_get_val_once(void)
{
    u16 g_temp_value = 0;
    ADEOC = 0; // ���ADCת����ɱ�־λ������ADת��
    while (!ADEOC)
        ;                // �ȴ�ת�����
    g_temp_value = ADRH; // ȡ��ת�����ֵ
    g_temp_value = g_temp_value << 4 | (ADRL & 0x0F);
    return g_temp_value;
}

// ��ʱ��3
void timer3_config(void)
{
    T3LOAD = 135 - 1; // FCPU 64��Ƶ��������1ms����һ���жϣ��ü��������ֵ���������������һЩ������
    T3CR = 0x86;      // ʹ�ܶ�ʱ����ʱ��Դѡ��FCPU��64��Ƶ
    T3IE = 1;
}

/************************************************
;  *    @������            : Sys_Init
;  *    @˵��              : ϵͳ��ʼ��
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void Sys_Init(void)
{
    GIE = 0;
    CLR_RAM();
    IO_Init();

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();

    // �����������
    P00PU = 1; // ����--�����ܲ���ȥ������(����ȥ��������Ҫ���ⲿ����)
    P00OE = 0; // ����ģʽ

// ���������:
#if USE_MY_DEBUG
    P05OE = 0; // ����
#else
    P04OE = 0; // ����ģʽ
#endif

    adc_config();
    timer3_config();

    GIE = 1;
}

// ����ɨ�躯��������õ���Ӧ�İ����¼�
// ��Ҫ����10ms�Ķ�ʱ����
void key_scan(void)
{
    static volatile u8 last_key_id = KEY_ID_NONE;
    static volatile u8 press_cnt = 0;               // �������µ�ʱ�����
    static volatile u8 filter_cnt = 0;              // ����������ʹ�õı���
    static volatile u8 filter_key_id = KEY_ID_NONE; // ��������ʱʹ�õı���

    volatile u8 cur_key_id = KEY_ID_NONE;
    static volatile u8 flag_is_key_mode_hold = 0;

    if (0 == KEY_SCAN_PIN)
    {
        cur_key_id = KEY_ID_BUTTOM;
    }
    else
    {
        cur_key_id = KEY_ID_NONE;
    }

    if (cur_key_id != filter_key_id)
    {
        // ����а�������/�ɿ�
        filter_cnt = 0;
        filter_key_id = cur_key_id;
        return;
    }

    if (filter_cnt < KEY_FILTER_TIMES)
    {
        // �����⵽��ͬ�İ�������/�ɿ�
        // ��ֹ�������
        filter_cnt++;
        return;
    }

    // �˲�/������ɺ�ִ�е�����

    if (last_key_id != cur_key_id)
    {
        if (last_key_id == KEY_ID_NONE)
        {
            // ����а������£�����������µ�ʱ�����
            press_cnt = 0;
        }
        else if (cur_key_id == KEY_ID_NONE)
        {
            // ��������ɿ�
            if (press_cnt < 75)
            {
                // ����ʱ��С�� 750ms ���Ƕ̰�
                if (KEY_ID_BUTTOM == last_key_id)
                {
                    //  �����̰�
                    key_event = KEY_EVENT_PRESS;
                }
            }
            else
            {
                // ��������������֮������
                flag_is_key_mode_hold = 0;
            }
        }
    }
    else if (cur_key_id != KEY_ID_NONE)
    {
        // ���������ס����
        if (press_cnt < 255)
            press_cnt++;

        if (0 == flag_is_key_mode_hold)
        {
            if (press_cnt >= 100) // xx ms
            {
                key_event = KEY_EVENT_HOLD;
                flag_is_key_mode_hold = 1;
            }
        }
    }

    last_key_id = cur_key_id;
}

// ��ɨ�赽�İ����¼����д���
void key_event_handle(void)
{
    if (flag_is_in_charging)
    {
        // ������ڳ�磬ֱ����������¼����˳�
        key_event = KEY_EVENT_NONE;
        return;
    }

    if (KEY_EVENT_PRESS == key_event)
    {
        if (flag_is_dev_open)
        {
            if (0 == led_mode)
            {
                // �Ϲ� - > ��
                LED_BLUE_OFF();
                LED_RED_ON();
                led_mode = 1;
            }
            else if (1 == led_mode)
            {
                // ��->��
                LED_RED_OFF();
                LED_BLUE_ON();
                led_mode = 2;
            }
            else if (2 == led_mode)
            {
                // ������� - > �Ϲ� ���� + ����
                LED_RED_ON();
                LED_BLUE_ON();
                led_mode = 0;
            }

            power_off_cnt = 0; // ��չػ�����
        }
    }
    else if (KEY_EVENT_HOLD == key_event)
    {
        if (flag_is_dev_open)
        {
            LED_RED_OFF();
            LED_BLUE_OFF();
            flag_is_dev_open = 0;
        }
        else
        {
            // �ػ�->����
            LED_RED_ON();
            LED_BLUE_ON();
            led_mode = 0; // ��ʾ��ǰ���Ϲ�
            flag_is_dev_open = 1;
        }
    }

    // ������ɺ���������¼�
    key_event = KEY_EVENT_NONE;
}

/************************************************
;  *    @������            : main
;  *    @˵��              : ������
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void main(void)
{
    Sys_Init();

#if 0 // ����ʱʹ��
    LED_BLUE_OFF();
    LED_RED_OFF();
    LED_GREEN_OFF();
    // P15D = 0;
    T3EN = 0;
    P15D = 1; // ʹ�ܶ�������صĳ��
#endif

    

    while (1)
    {

#if 1
        // ����Ƿ��ڳ��
        if (0 == flag_is_in_charging)
        {
            if (CHARGE_SCAN_PIN)
            {
                // delay_ms(20);
                delay_ms(100);
                if (CHARGE_SCAN_PIN)
                {
                    flag_is_in_charging = 1;
                    P15D = 1; // ʹ��������صĳ��
                }
            }
        }
        else
        {
            // ���ʱ������Ƿ�Ͽ��˳��
            if (0 == CHARGE_SCAN_PIN)
            {
                // delay_ms(20);
                delay_ms(100);
                if (0 == CHARGE_SCAN_PIN)
                {
                    flag_is_in_charging = 0;
                    flag_is_full_charged = 0;
                }
            }
        }

        // ����Ƿ������,�Լ����ʱ�Զ��ػ�
        if (flag_is_in_charging)
        {
            adc_val = adc_get_val();
            // if (adc_val >= 2124 - AD_OFFSET) // �����ص�ѹ����4.15V
            // if (adc_val >= 2124) // �����ص�ѹ����4.15V
            if (adc_val >= 2150) // �����ص�ѹ����4.2V
            {
                flag_is_full_charged = 1;
                P15D = 0; // �Ͽ���������صĳ��
            }
            else if (adc_val < 2124 - AD_OFFSET) // �����ص�ѹС��4.15V-����ֵ
            {
                flag_is_full_charged = 0;
                P15D = 1; // ʹ��������صĳ��
            }

            // ���ʱ�Զ��ػ�
            LED_RED_OFF();
            LED_BLUE_OFF();
            flag_is_dev_open = 0;
            led_mode = 2; // ������¿����ٶϿ���磬�ͻ������ƺ����ƣ�������ǣ�������͹��ģ��͹��Ļ��Ѻ����б�����������
        }

        if (flag_is_dev_open && 0 == flag_is_in_charging)
        {
            // ���������������δ�ڳ��: 
            adc_val = adc_get_val();
            // if (adc_val < 1638 - AD_OFFSET) // ��ص�ѹС��3.2V(ʵ�ʲ�����3.14V)
            if (adc_val < 1638) // ��ص�ѹС��3.2V(ʵ�ʲ�����3.22-3.23V)
            {
                flag_is_power_low = 1;
            }
            // else if  (adc_val > 1638 + AD_OFFSET) // �����ص�ѹ����3.2V+adֵ��������Ϊ��ص���δ���ػ�����ֵ
            // {
            //     flag_is_power_low = 0;
            // }

            // if (adc_val < 1485 - AD_OFFSET) // ��ص�ѹС��2.9V
            // {
            //     flag_is_dev_open = 0; // �ػ�
            // }

            // if (adc_val < 1587 - AD_OFFSET) // ��ص�ѹС��3.1V(ʵ�ʲ�����3.04V)
            if (adc_val < 1587)  // �����ص�ѹС��3.099V,ʵ�ʲ�����3.12-3.13V
            {
                flag_is_dev_open = 0; // �ػ�
            }
        }
        else
        {
            flag_is_power_low = 0;
        }

        if (flag_is_in_charging && flag_is_full_charged)
        {
            // ��������� �̵Ƴ���
            // LED_GREEN_ON();

            // ��������磬�ر��̵�
            LED_GREEN_OFF();
        }

        key_event_handle();

        if (0 == flag_is_dev_open &&    // �豸����ʱ��������͹���
            0 == flag_is_in_charging && // ���ʱ��������͹���
            KEY_SCAN_PIN)               // �а������£�������͹���
        {
            // ����͹���
            GIE = 0;      // ���������ж�
            DRVCR = 0x80; // IO�Ļ���ͨ����
            LED_BLUE_OFF();
            LED_GREEN_OFF();
            LED_RED_OFF();
            P15D = 0; // �Ͽ���������صĳ��
            // P15OE = 0;
            // LED��������Ϊ����:
            // P17OE = 0;
            // P13OE = 0;
            // P16OE = 0;

            T3EN = 0;
            T3IE = 0;

            // adcͨ��ѡ��һ������ 1/4 VDD��
            ADCHS3 = 0;
            ADCHS2 = 0;
            ADCHS1 = 0;
            ADCHS0 = 0;
            ADEN = 0; // �ر�adc

            // ��������Ϊ�����жϴ���
            P00KE = 1;
// �������������Ϊ�����жϴ���
#if USE_MY_DEBUG
            P05KE = 1;
#else
            P04KE = 1;
#endif
            KBIF = 0;
            KBIE = 1;
            LVDEN = 0;
            HFEN = 0; // �رո���ʱ��
            LFEN = 0; // �رյ���ʱ��
            // ����ǰ�ر�����
            Nop();
            Nop();
            Stop();
            Nop();
            Nop();
            HFEN = 1; // ��������ʱ��
            LVDEN = 1;
            P00KE = 0;
#if USE_MY_DEBUG
            P05KE = 0;
#else
            P04KE = 0;
#endif
            KBIE = 0;
            KBIF = 0;

            Sys_Init(); // ��������ڲ��ı�־λ�;�̬����û�����������ֱ��������б�����ֵ��ȫΪ0,�������³�ʼ��
            delay_ms(1);
            GIE = 1;
        }

#endif

        __asm;
        clrwdt;
        __endasm;
    }
}
/************************************************
;  *    @������            : interrupt
;  *    @˵��              : �жϺ���
;  *    @�������          :
;  *    @���ز���          :
;  ***********************************************/
void int_isr(void) __interrupt
{
    __asm;
    movra _abuf;
    swapar _PFLAG;
    movra _statusbuf;
    __endasm;

    if (T3IF & T3IE)
    {
        // Ŀǰÿ1ms����һ���ж�
        { // ����ɨ��
            static u8 key_scan_cnt = 0;
            key_scan_cnt++;
            if (key_scan_cnt >= 10)
            {
                key_scan_cnt = 0;
                key_scan();
            }
        } // ����ɨ��

        { // ��硢�͵���ʱ������ƹ���˸
            static u16 blink_cnt = 0;
            if (flag_is_power_low)
            {
                blink_cnt++;
                if (blink_cnt <= 350)
                {
                    LED_GREEN_ON();
                }
                else if (blink_cnt <= 700)
                {
                    LED_GREEN_OFF();
                }
                else
                {
                    blink_cnt = 0;
                }
            }
            // else if (flag_is_in_charging && 0 == flag_is_full_charged)
            // {

            //     blink_cnt++;
            //     if (blink_cnt <= 500)
            //     {
            //         LED_GREEN_ON();
            //     }
            //     else if (blink_cnt <= 1000)
            //     {
            //         LED_GREEN_OFF();
            //     }
            //     else
            //     {
            //         blink_cnt = 0;
            //     }
            // }
            else
            {
                blink_cnt = 0;
                LED_GREEN_OFF();
            }
        }

        { // �����󣬸����Զ��ػ�
            if (flag_is_dev_open)
            {
                power_off_cnt++;
                if (power_off_cnt >= 360000) // 6min
                {
                    power_off_cnt = 0;
                    flag_is_dev_open = 0;
                }
            }
            else
            {
                power_off_cnt = 0;
            }
        }

        T3IF = 0;
    }

    __asm;
    swapar _statusbuf;
    movra _PFLAG;
    swapr _abuf;
    swapar _abuf;
    __endasm;
}

/**************************** end of file *********************************************/
