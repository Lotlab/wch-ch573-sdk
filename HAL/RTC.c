/********************************** (C) COPYRIGHT *******************************
 * File Name          : RTC.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2019/11/05
 * Description        : RTC配置及其初始化
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include "CH57x_clk.h"
#include "HAL.h"

/*********************************************************************
 * CONSTANTS
 */
#define RTC_INIT_TIME_HOUR 0
#define RTC_INIT_TIME_MINUTE 0
#define RTC_INIT_TIME_SECEND 0

/***************************************************
 * Global variables
 */
volatile uint32_t RTCTigFlag;

/*******************************************************************************
 * Function Name  : RTC_SetTignTime
 * Description    : 配置RTC触发时间
 * Input          : 
 * Output         : None
 * Return         : None
 *******************************************************************************/
void RTC_SetTignTime(uint32_t time)
{
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R32_RTC_TRIG = time;
    RTCTigFlag = 0;
}

/*******************************************************************************
 * @fn          RTC_IRQHandler
 *
 * @brief       RTC中断处理
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
__attribute__((interrupt(WCH_INT_TYPE)))
__attribute__((section(".highcode"))) void
RTC_IRQHandler(void)
{
    R8_RTC_FLAG_CTRL = (RB_RTC_TMR_CLR | RB_RTC_TRIG_CLR);
    RTCTigFlag = 1;
}

/*******************************************************************************
 * @fn          HAL_Time0Init
 *
 * @brief       系统定时器初始化
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void HAL_TimeInit(void)
{
#if (CLK_OSC32K)
    R8_SAFE_ACCESS_SIG = 0x57;
    R8_SAFE_ACCESS_SIG = 0xa8;
    R8_CK32K_CONFIG &= ~(RB_CLK_OSC32K_XT|RB_CLK_XT32K_PON);
    R8_CK32K_CONFIG |= RB_CLK_INT32K_PON;
    Lib_Calibration_LSI();
#else
    R8_SAFE_ACCESS_SIG = 0x57;
    R8_SAFE_ACCESS_SIG = 0xa8;
    R8_CK32K_CONFIG |= RB_CLK_OSC32K_XT | RB_CLK_INT32K_PON | RB_CLK_XT32K_PON;
    R8_SAFE_ACCESS_SIG = 0;
#endif
    RTC_InitTime(2020, 1, 1, 0, 0, 0); //RTC时钟初始化当前时间
    TMOS_TimerInit(0);
}

/******************************** endfile @ time ******************************/
