/******************************************************************************
;  *       @型号                 : MC32F7361
;  *       @创建日期             : 2021.12.21
;  *       @公司/作者            : SINOMCU-FAE
;  *       @晟矽微技术支持       : 2048615934
;  *       @晟矽微官网           : http://www.sinomcu.com/
;  *       @版权                 : 2021 SINOMCU公司版权所有.
;  *----------------------摘要描述---------------------------------
;  *
******************************************************************************/

#include "user.h"

/************************************************
;  *    @函数名          : CLR_RAM
;  *    @说明            : 清RAM
;  *    @输入参数        :
;  *    @返回参数        :
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
;  *    @函数名            : IO_Init
;  *    @说明              : IO初始化
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void IO_Init(void)
{
    IOP0 = 0x00;   // io口数据位
    OEP0 = 0x3F;   // io口方向 1:out  0:in
    PUP0 = 0x00;   // io口上拉电阻   1:enable  0:disable
    PDP0 = 0x00;   // io口下拉电阻   1:enable  0:disable
    P0ADCR = 0x00; // io类型选择  1:模拟输入  0:通用io

    // IOP1 = 0x00;   // io口数据位
    IOP1 = 0xD0;   // 不点亮LED
    OEP1 = 0xFF;   // io口方向 1:out  0:in
    PUP1 = 0x00;   // io口上拉电阻   1:enable  0:disable
    PDP1 = 0x00;   // io口下拉电阻   1:enable  0:disable
    P1ADCR = 0x00; // io类型选择  1:模拟输入  0:通用io

    IOP2 = 0x00; // io口数据位
    OEP2 = 0x0F; // io口方向 1:out  0:in
    PUP2 = 0x00; // io口上拉电阻   1:enable  0:disable
    PDP2 = 0x00; // io口下拉电阻   1:enable  0:disablea

    PMOD = 0x00;  // P00、P01、P13 io端口值从寄存器读，推挽输出
    // DRVCR = 0xB0; // P16、P17输出驱动电流100mA
    // DRVCR = 0x90; // P16、P17输出驱动电流50mA
    DRVCR = 0x80; // P16、P17输出驱动电流25mA，其他普通IO正常驱动电流输出
}

/************************************************
;  *    @函数名            : ADC_Init
;  *    @说明              : ADC初始化
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void adc_config(void)
{
    ADCR0 = 0xAB; // 12位精度，使能adc,adc通道选择  1/4 VDD
    ADCR1 = 0x80; // adc转换时钟选择 FHIRC/32，使用內部2.0V参考电压
    ADCR2 = 0x0F; // 采样时间，只能固定是15 个 ADCLK
}

// 获取adc单次转换后的值
u16 adc_get_val(void)
{
    u8 i = 0; // adc采集次数的计数
    u16 g_temp_value = 0;
    u32 g_tmpbuff = 0;
    u16 g_adcmax = 0;
    u16 g_adcmin = 0xFFFF;

    // 采集20次，去掉前两次采样，再去掉一个最大值和一个最小值，再取平均值
    for (i = 0; i < 20; i++)
    {
        ADEOC = 0; // 清除ADC转换完成标志位，启动AD转换
        while (!ADEOC)
            ;                // 等待转换完成
        g_temp_value = ADRH; // 取出转换后的值
        g_temp_value = g_temp_value << 4 | (ADRL & 0x0F);
        if (i < 2)
            continue; // 丢弃前两次采样的
        if (g_temp_value > g_adcmax)
            g_adcmax = g_temp_value; // 最大
        if (g_temp_value < g_adcmin)
            g_adcmin = g_temp_value; // 最小
        g_tmpbuff += g_temp_value;
    }
    g_tmpbuff -= g_adcmax;           // 去掉一个最大
    g_tmpbuff -= g_adcmin;           // 去掉一个最小
    g_temp_value = (g_tmpbuff >> 4); // 除以16，取平均值

    return g_temp_value;
}

// 获取adc单次转换后的值
u16 adc_get_val_once(void)
{
    u16 g_temp_value = 0;
    ADEOC = 0; // 清除ADC转换完成标志位，启动AD转换
    while (!ADEOC)
        ;                // 等待转换完成
    g_temp_value = ADRH; // 取出转换后的值
    g_temp_value = g_temp_value << 4 | (ADRL & 0x0F);
    return g_temp_value;
}

// 定时器3
void timer3_config(void)
{
    T3LOAD = 135 - 1; // FCPU 64分频后，这里是1ms触发一次中断（用计算出来的值会有误差，这里加上了一些补偿）
    T3CR = 0x86;      // 使能定时器，时钟源选择FCPU，64分频
    T3IE = 1;
}

/************************************************
;  *    @函数名            : Sys_Init
;  *    @说明              : 系统初始化
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void Sys_Init(void)
{
    GIE = 0;
    CLR_RAM();
    IO_Init();

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();

    // 按键检测引脚
    P00PU = 1; // 上拉--看看能不能去掉这里(可以去掉，但是要加外部上拉)
    P00OE = 0; // 输入模式

// 充电检测引脚:
#if USE_MY_DEBUG
    P05OE = 0; // 输入
#else
    P04OE = 0; // 输入模式
#endif

    adc_config();
    timer3_config();

    GIE = 1;
}

// 按键扫描函数，最后会得到对应的按键事件
// 需要放在10ms的定时器中
void key_scan(void)
{
    static volatile u8 last_key_id = KEY_ID_NONE;
    static volatile u8 press_cnt = 0;               // 按键按下的时间计数
    static volatile u8 filter_cnt = 0;              // 按键消抖，使用的变量
    static volatile u8 filter_key_id = KEY_ID_NONE; // 按键消抖时使用的变量

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
        // 如果有按键按下/松开
        filter_cnt = 0;
        filter_key_id = cur_key_id;
        return;
    }

    if (filter_cnt < KEY_FILTER_TIMES)
    {
        // 如果检测到相同的按键按下/松开
        // 防止计数溢出
        filter_cnt++;
        return;
    }

    // 滤波/消抖完成后，执行到这里

    if (last_key_id != cur_key_id)
    {
        if (last_key_id == KEY_ID_NONE)
        {
            // 如果有按键按下，清除按键按下的时间计数
            press_cnt = 0;
        }
        else if (cur_key_id == KEY_ID_NONE)
        {
            // 如果按键松开
            if (press_cnt < 75)
            {
                // 按下时间小于 750ms ，是短按
                if (KEY_ID_BUTTOM == last_key_id)
                {
                    //  按键短按
                    key_event = KEY_EVENT_PRESS;
                }
            }
            else
            {
                // 长按、长按持续之后松手
                flag_is_key_mode_hold = 0;
            }
        }
    }
    else if (cur_key_id != KEY_ID_NONE)
    {
        // 如果按键按住不放
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

// 对扫描到的按键事件进行处理
void key_event_handle(void)
{
    if (flag_is_in_charging)
    {
        // 如果正在充电，直接清除按键事件并退出
        key_event = KEY_EVENT_NONE;
        return;
    }

    if (KEY_EVENT_PRESS == key_event)
    {
        if (flag_is_dev_open)
        {
            if (0 == led_mode)
            {
                // 紫光 - > 红
                LED_BLUE_OFF();
                LED_RED_ON();
                led_mode = 1;
            }
            else if (1 == led_mode)
            {
                // 红->蓝
                LED_RED_OFF();
                LED_BLUE_ON();
                led_mode = 2;
            }
            else if (2 == led_mode)
            {
                // 其他情况 - > 紫光 （红 + 蓝）
                LED_RED_ON();
                LED_BLUE_ON();
                led_mode = 0;
            }

            power_off_cnt = 0; // 清空关机计数
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
            // 关机->开机
            LED_RED_ON();
            LED_BLUE_ON();
            led_mode = 0; // 表示当前是紫光
            flag_is_dev_open = 1;
        }
    }

    // 处理完成后，清除按键事件
    key_event = KEY_EVENT_NONE;
}

/************************************************
;  *    @函数名            : main
;  *    @说明              : 主程序
;  *    @输入参数          :
;  *    @返回参数          :
;  ***********************************************/
void main(void)
{
    Sys_Init();

#if 0 // 测试时使用
    LED_BLUE_OFF();
    LED_RED_OFF();
    LED_GREEN_OFF();
    // P15D = 0;
    T3EN = 0;
    P15D = 1; // 使能对主机电池的充电
#endif

    

    while (1)
    {

#if 1
        // 检测是否在充电
        if (0 == flag_is_in_charging)
        {
            if (CHARGE_SCAN_PIN)
            {
                // delay_ms(20);
                delay_ms(100);
                if (CHARGE_SCAN_PIN)
                {
                    flag_is_in_charging = 1;
                    P15D = 1; // 使能主机电池的充电
                }
            }
        }
        else
        {
            // 充电时，检测是否断开了充电
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

        // 检测是否充满电,以及充电时自动关机
        if (flag_is_in_charging)
        {
            adc_val = adc_get_val();
            // if (adc_val >= 2124 - AD_OFFSET) // 如果电池电压大于4.15V
            // if (adc_val >= 2124) // 如果电池电压大于4.15V
            if (adc_val >= 2150) // 如果电池电压大于4.2V
            {
                flag_is_full_charged = 1;
                P15D = 0; // 断开对主机电池的充电
            }
            else if (adc_val < 2124 - AD_OFFSET) // 如果电池电压小于4.15V-死区值
            {
                flag_is_full_charged = 0;
                P15D = 1; // 使能主机电池的充电
            }

            // 充电时自动关机
            LED_RED_OFF();
            LED_BLUE_OFF();
            flag_is_dev_open = 0;
            led_mode = 2; // 如果按下开机再断开充电，就会点亮红灯和蓝灯，如果不是，则会进入低功耗，低功耗唤醒后所有变量都会清零
        }

        if (flag_is_dev_open && 0 == flag_is_in_charging)
        {
            // 如果主机开启，且未在充电: 
            adc_val = adc_get_val();
            // if (adc_val < 1638 - AD_OFFSET) // 电池电压小于3.2V(实际测试是3.14V)
            if (adc_val < 1638) // 电池电压小于3.2V(实际测试是3.22-3.23V)
            {
                flag_is_power_low = 1;
            }
            // else if  (adc_val > 1638 + AD_OFFSET) // 如果电池电压大于3.2V+ad值死区，认为电池电量未到关机的阈值
            // {
            //     flag_is_power_low = 0;
            // }

            // if (adc_val < 1485 - AD_OFFSET) // 电池电压小于2.9V
            // {
            //     flag_is_dev_open = 0; // 关机
            // }

            // if (adc_val < 1587 - AD_OFFSET) // 电池电压小于3.1V(实际测试是3.04V)
            if (adc_val < 1587)  // 如果电池电压小于3.099V,实际测试是3.12-3.13V
            {
                flag_is_dev_open = 0; // 关机
            }
        }
        else
        {
            flag_is_power_low = 0;
        }

        if (flag_is_in_charging && flag_is_full_charged)
        {
            // 如果充满电 绿灯常亮
            // LED_GREEN_ON();

            // 如果充满电，关闭绿灯
            LED_GREEN_OFF();
        }

        key_event_handle();

        if (0 == flag_is_dev_open &&    // 设备工作时，不进入低功耗
            0 == flag_is_in_charging && // 充电时，不进入低功耗
            KEY_SCAN_PIN)               // 有按键按下，不进入低功耗
        {
            // 进入低功耗
            GIE = 0;      // 禁用所有中断
            DRVCR = 0x80; // IO改回普通驱动
            LED_BLUE_OFF();
            LED_GREEN_OFF();
            LED_RED_OFF();
            P15D = 0; // 断开给主机电池的充电
            // P15OE = 0;
            // LED引脚配置为输入:
            // P17OE = 0;
            // P13OE = 0;
            // P16OE = 0;

            T3EN = 0;
            T3IE = 0;

            // adc通道选择一个不是 1/4 VDD的
            ADCHS3 = 0;
            ADCHS2 = 0;
            ADCHS1 = 0;
            ADCHS0 = 0;
            ADEN = 0; // 关闭adc

            // 按键配置为键盘中断触发
            P00KE = 1;
// 充电检测引脚配置为键盘中断触发
#if USE_MY_DEBUG
            P05KE = 1;
#else
            P04KE = 1;
#endif
            KBIF = 0;
            KBIE = 1;
            LVDEN = 0;
            HFEN = 0; // 关闭高速时钟
            LFEN = 0; // 关闭低速时钟
            // 休眠前关闭外设
            Nop();
            Nop();
            Stop();
            Nop();
            Nop();
            HFEN = 1; // 开启高速时钟
            LVDEN = 1;
            P00KE = 0;
#if USE_MY_DEBUG
            P05KE = 0;
#else
            P04KE = 0;
#endif
            KBIE = 0;
            KBIF = 0;

            Sys_Init(); // 按键检测内部的标志位和静态变量没有清除，这里直接清除所有变量的值，全为0,并且重新初始化
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
;  *    @函数名            : interrupt
;  *    @说明              : 中断函数
;  *    @输入参数          :
;  *    @返回参数          :
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
        // 目前每1ms进入一次中断
        { // 按键扫描
            static u8 key_scan_cnt = 0;
            key_scan_cnt++;
            if (key_scan_cnt >= 10)
            {
                key_scan_cnt = 0;
                key_scan();
            }
        } // 按键扫描

        { // 充电、低电量时，负责灯光闪烁
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

        { // 开机后，负责自动关机
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
