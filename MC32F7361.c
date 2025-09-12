// encoding GB2312
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
    IOP1 = 0xC0;   // ������LED
    OEP1 = 0xFF;   // io�ڷ��� 1:out  0:in
    PUP1 = 0x00;   // io����������   1:enable  0:disable
    PDP1 = 0x00;   // io����������   1:enable  0:disable
    P1ADCR = 0x00; // io����ѡ��  1:ģ������  0:ͨ��io

    IOP2 = 0x00; // io������λ
    OEP2 = 0x0F; // io�ڷ��� 1:out  0:in
    PUP2 = 0x00; // io����������   1:enable  0:disable
    PDP2 = 0x00; // io����������   1:enable  0:disablea

    PMOD = 0x00;  // P00��P01��P13 io�˿�ֵ�ӼĴ��������������
    DRVCR = 0xB0; // P16��P17�����������100mA
    // DRVCR = 0x90; // P16��P17�����������50mA
    // DRVCR = 0x80; // P16��P17�����������25mA��������ͨIO���������������
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
    // FCPU == FHOSC / 4 ʱ��FCPU == 8MHz��
    // T3LOAD = 135 - 1; // FCPU 64��Ƶ��������1ms����һ���жϣ��ü��������ֵ���������������һЩ������
    // T3CR = 0x86;      // ʹ�ܶ�ʱ����ʱ��Դѡ��FCPU��64��Ƶ

    // FCPU == FHOSC / 8 ʱ��FCPU == 4MHz��
    T3LOAD = 135 - 1; // ��Ƶ��������1ms����һ���жϣ��ü��������ֵ���������������һЩ������

    T3CR = (0x01 << 7) | (0x01 << 2) | (0x01); // ʹ�ܶ�ʱ����ʱ��Դѡ��FCPU��32��Ƶ
    T3IE = 1;
}

// ��ƺ����������޸�̫�࣬���ܲ��Ǻ��pwm��ʼ��
void led_red_pwm_config(void)
{
    // FCPU == FHOSC / 8 ʱ��FCPU == 4MHz��
    T1CR = 0x01 << 1; // ��ʹ��PWM,CPU,4��Ƶ

    T1LOAD = 100 - 1; // 100us
    // T1DATA = 50;
    // T1DATA = LED_BLUE_LUMINANCE;
    T1EN = 0;
}

// ��ƺ����������޸�̫�࣬���ܲ�������pwm��ʼ��
void led_blue_pwm_config(void)
{
    // FCPU == FHOSC / 8 ʱ��FCPU == 4MHz��
    T0CR = 0x01 << 1; // ��ʹ��PWM,CPU,4��Ƶ

    T0LOAD = 100 - 1; // 100us
    // T0DATA = 60;
    // T0DATA = LED_RED_LUMINANCE;
    T0EN = 0;
}

void led_red_on(void)
{
    PWM0EC = 1;
    T0EN = 1;
}

void led_red_off(void)
{
    PWM0EC = 0;
    T0EN = 0;
    LED_RED_PIN = 1; // �ߵ�ƽ��ʾϨ��
}

void led_blue_on(void)
{

    PWM1EC = 1;
    T1EN = 1;
}

void led_blue_off(void)
{
    PWM1EC = 0;
    T1EN = 0;
    LED_BLUE_PIN = 1; // �ߵ�ƽ��ʾϨ��
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

    // ������Ƶ�PWM�����ţ�
    led_red_pwm_config();
    // �������Ƶ�pwm������
    led_blue_pwm_config();
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
            if (LED_MODE_RED == led_mode)
            {
                // ��� -> ��
                LED_BLUE_TIMER_DATA = LED_BLUE_LUMINANCE;
                LED_RED_OFF();
                LED_BLUE_ON();
                led_mode = LED_MODE_BLUE;
            }
            else if (LED_MODE_BLUE == led_mode)
            {
                // �� -> �Ϲ� ���� + ����
                LED_RED_TIMER_DATA = LED_RED_LUMINANCE_IN_PURPLE;
                LED_BLUE_TIMER_DATA = LED_BLUE_LUMINANCE_IN_PURPLE;
                LED_RED_ON();
                LED_BLUE_ON();
                led_mode = LED_MODE_RED_AND_BLUE;
            }
            else if (LED_MODE_RED_AND_BLUE == led_mode)
            {
                // �Ϲ� ���� + ���� - > ��
                LED_RED_TIMER_DATA = LED_RED_LUMINANCE;
                LED_BLUE_OFF();
                LED_RED_ON();
                led_mode = LED_MODE_RED;
            }

            power_off_cnt = 0; // ��չػ�����
        }
        else
        {
            // �ػ�->����
            // LED_RED_TIMER_DATA = LED_RED_LUMINANCE;
            // LED_RED_ON();
            // led_mode = LED_MODE_RED; // ��ʾ��ǰ�Ǻ��
            // flag_is_dev_open = 1;

            LED_RED_TIMER_DATA = LED_RED_LUMINANCE_IN_PURPLE;
            LED_BLUE_TIMER_DATA = LED_BLUE_LUMINANCE_IN_PURPLE;
            LED_RED_ON();
            LED_BLUE_ON();
            led_mode = LED_MODE_RED_AND_BLUE;
            flag_is_dev_open = 1;
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
        // else
        // {
        //     // �ػ�->����
        //     LED_RED_ON();
        //     LED_BLUE_ON();
        //     led_mode = 0; // ��ʾ��ǰ���Ϲ�
        // flag_is_dev_open = 1;
        // }

        flag_is_enable_into_low_power = 1; // ֻҪʶ�𵽳�����ʹ�ܽ���͹���
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
    // flag_is_dev_open = 1; // ����ʱʹ��
    // DEBUG_PIN = ~DEBUG_PIN;// ����ʱʹ��
    flag_is_enable_into_low_power = 1; // һ�ϵ��ʹ�ܽ���͹���

    while (1)
    {
        // DEBUG_PIN = ~DEBUG_PIN;
        // delay_ms(1);
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
            // if (adc_val >= 2150) // �����ص�ѹ����4.2V,ʵ�ʲ�����4.1V����
            // if (adc_val >= 2048) // ʵ�ʲ��ԣ���絽3.95V�ͶϿ���
            // if (adc_val >= 2099) // �����ص�ѹ����4.1V��ʵ�ʲ����� 4.04V
            // if (adc_val >= 2124)  // 4.15
            // if (adc_val >= 2130) // �����ص�ѹ����4.16V��ʵ�ʲ����� 4.12V ===============================
            // if (adc_val >= 2150) // �����ص�ѹ���� 4.2V
            // if (adc_val >= 2151) // �����ص�ѹ���� 4.201171875 V��ʵ�ʲ����� V
            if (adc_val >= 2165) // �����ص�ѹ���� 4.23V ======================================
            {
                flag_is_full_charged = 1;
                P15D = 0; // �Ͽ���������صĳ��
                // delay_ms(5000);
                delay_ms(10000);
            }
            // else if (adc_val < 2124 - AD_OFFSET) // �����ص�ѹС��4.15V-����ֵ
            // else if (adc_val < 2048 - AD_OFFSET) // �����ص�ѹС��
            else
            {
                flag_is_full_charged = 0;
                P15D = 1; // ʹ��������صĳ��
            }

            if (flag_is_cut_down_charge && 0 == flag_is_full_charged)
            {
                flag_is_cut_down_charge = 0;
                P15D = 0; // �Ͽ���������صĳ��
                delay_ms(10);
                if (CHARGE_SCAN_PIN)
                {
                    // �����⵽���г��
                    P15D = 1; // �ָ���������صĳ��
                }
            }

            // ���ʱ�Զ��ػ�
            LED_RED_OFF();
            LED_BLUE_OFF();
            flag_is_dev_open = 0;
            flag_is_enable_into_low_power = 1; // ʹ�ܽ���͹���
            // led_mode = 2;                      // ������¿����ٶϿ���磬�ͻ������ƺ����ƣ�������ǣ�������͹��ģ��͹��Ļ��Ѻ����б�����������
            led_mode = LED_MODE_RED_AND_BLUE; // �´ζ̰��������Ӻ�+����Ϊ���
        }

        if (flag_is_dev_open && 0 == flag_is_in_charging)
        {
            // ���������������δ�ڳ��:
            adc_val = adc_get_val();
            // if (adc_val < 1638 - AD_OFFSET) // ��ص�ѹС��3.2V(ʵ�ʲ�����3.14V)
            // if (adc_val < 1638) // ��ص�ѹС��3.2V(ʵ�ʲ�����3.22-3.23V)
            if (adc_val < 1689) // ��ص�ѹС��3.3V
            {
                flag_is_power_low = 1;
            }

            // if (adc_val < 1587) // �����ص�ѹС��3.099V,ʵ�ʲ�����3.12-3.13V
            // if (adc_val < 1536) // �����ص�ѹС��3.0V
            if (adc_val < 1484) // �����ص�ѹС��2.8984V
            {
                flag_is_dev_open = 0;              // �ػ�
                flag_is_enable_into_low_power = 1; // ʹ�ܽ���͹���
            }
        }
        else
        {
            flag_is_power_low = 0;
        }

        key_event_handle();

#if 1 // �͹���
        if (0 == flag_is_dev_open &&       // �豸����ʱ��������͹���
            0 == flag_is_in_charging &&    // ���ʱ��������͹���
            KEY_SCAN_PIN &&                /* �а�������(Ϊ�͵�ƽ)��������͹��� */
            flag_is_enable_into_low_power) /* ���ʹ�ܽ���͹��� */
        {
        label_low_power:
            // flag_is_enable_into_low_power = 0; // ��һ����Բ��ӣ���Ϊ��������RAM
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

            // ���Ѻ�ֱ�ӵ���һ�ΰ���ɨ�裬
            // �п����ǰ������¶����ѣ�Ҳ�п����ǳ�绽��
            // key_scan(); // ����ǰ������»��ѣ������ܹ���ȡһ�μ�ֵ

            if (0 == KEY_SCAN_PIN) // ������Ѻ󣬷��ְ����ǰ��µ�
            {
                // filter_key_id = KEY_ID_BUTTOM;
                // filter_cnt = KEY_FILTER_TIMES + 1; // �������˲�����ֵ�ӵ������������ɨ�躯�� key_scan() �ڲ����˲�����
                // last_key_id = KEY_ID_BUTTOM;
                // key_scan(); // ����ǰ������»��ѣ������ܹ���ȡһ�� key_id

                volatile u8 key_press_cnt = 0;

                T3IE = 0;  // ���ζ�ʱ���ж�
                T3EN = 0;  // ��ʹ�ܶ�ʱ��
                T3CNT = 0; // �����ʱ������ֵ

                while (0 == KEY_SCAN_PIN)
                {
                    if (key_press_cnt < 255)
                    {
                        key_press_cnt++;
                    }

                    delay_ms(10);
                }

                if (key_press_cnt < 75) // ��������ʱ��С��750ms���Ƕ̰�
                {
                    key_event = KEY_EVENT_PRESS;
                    key_event_handle();
                }

                T3IE = 1; // ʹ�ܶ�ʱ���ж�
                T3EN = 1; // ʹ�ܶ�ʱ��
            }
        }
#endif

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
        // DEBUG_PIN = ~DEBUG_PIN;

        { // ����ɨ��
            static u8 key_scan_cnt = 0;
            key_scan_cnt++;
            if (key_scan_cnt >= 10)
            {
                key_scan_cnt = 0;
                key_scan();
            }
        } // ����ɨ��

        { // �͵���ʱ������ƹ���˸
            static u16 blink_cnt = 0;
            if (flag_is_power_low)
            {
                blink_cnt++;
                if (blink_cnt <= 1000)
                {
                    LED_GREEN_ON();
                }
                else if (blink_cnt <= 2000)
                {
                    LED_GREEN_OFF();
                }
                else
                {
                    blink_cnt = 0;
                }
            }
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
                // if (power_off_cnt >= 360000) // 6min--�ͻ������� 390s,ʵ�ʲ����� 388s

                // if (power_off_cnt >= 334020) // 6min����������Ļ������������˲�����
                if (power_off_cnt >= ((u32)334020 / 2)) // 3min ,ֱ�Ӹ���6min�����ݳ���2
                {
                    power_off_cnt = 0;
                    flag_is_dev_open = 0;
                    flag_is_enable_into_low_power = 1; // ʹ�ܽ���͹���

                    // DEBUG_PIN = ~DEBUG_PIN;
                }
            }
            else
            {
                power_off_cnt = 0;
            }
        }

        {
            static volatile u16 into_low_power_cnt = 0;
            if (0 == flag_is_enable_into_low_power &&
                0 == flag_is_dev_open &&
                0 == flag_is_in_charging)
            {
                // ���û�п�������û��ʹ�ܽ���͹���
                into_low_power_cnt++;
                if (into_low_power_cnt >= 1000) // xx ms
                {
                    into_low_power_cnt = 0;
                    flag_is_enable_into_low_power = 1;
                }
            }
            else
            {
                into_low_power_cnt = 0;
            }
        }

        { // ���ʱ������ÿ��һ��ʱ��Ͽ�����صĳ�磬�����Ƿ����ڳ��
            /*
                ���������Ѿ��Ͽ���磬����ȴ�����Ÿ���صĳ�磬��������������
                ���ܻ����⵽�г��ĵ�ѹ��Ӧ����©�����ĵ�ѹ��
            */

            static u16 cut_down_charge_cnt = 0;
            if (flag_is_in_charging && 0 == flag_is_full_charged)
            {
                cut_down_charge_cnt++;
                if (cut_down_charge_cnt >= 5000) // xx ms
                {
                    cut_down_charge_cnt = 0;
                    flag_is_cut_down_charge = 1;
                }
            }
            else
            {
                cut_down_charge_cnt = 0;
                flag_is_cut_down_charge = 0; // ���ڳ��ʱ����ոñ�־λ
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
