/********************************** (C) COPYRIGHT *******************************
 * File Name          : SLEEP.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2019/11/05
 * Description        : 睡眠配置及其初始化
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#include <stdint.h>
#include "hal_config.h"

#include "SLEEP.h"
#include "RTC.h"
#include "CH57x_sys.h"
#include "CH57x_pwr.h"
#include "CH57x_clk.h"
#include "CH57x_ble.h"
// #include "HAL.h"

/*******************************************************************************
 * @fn          CH57X_LowPower
 *
 * @brief       启动睡眠
 *
 * input parameters
 *
 * @param       time-唤醒的时间点（RTC绝对值）
 *
 * output parameters
 *
 * @param       
 *
 * @return      None.
 */
uint32_t CH57X_LowPower(uint32_t time)
{
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    uint32_t tmp, irq_status;

    SYS_DisableAllIrq(&irq_status);
    tmp = RTC_GetCycle32k();
    if ((time < tmp) || ((time - tmp) < 30)) { // 检测睡眠的最短时间
        SYS_RecoverIrq(irq_status);
        return 2;
    }
    RTC_SetTignTime(time);
    SYS_RecoverIrq(irq_status);
#if (DEBUG_UART == Debug_UART1) // 使用其他串口输出打印信息需要修改这行代码
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0)
        __nop();
#endif
    // LOW POWER-sleep模式
    if (!RTCTigFlag) {
        LowPower_Sleep(RB_PWR_RAM2K | RB_PWR_RAM16K | RB_PWR_EXTEND);
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
        PWR_DCDCCfg(ENABLE);
#endif
        HSECFG_Current(HSE_RCur_100); // 降为额定电流(低功耗函数中提升了HSE偏置电流)
        time += 47; // ? 这是什么
        if (time > 0xA8C00000)
            time -= 0xA8C00000;
        RTC_SetTignTime(time);
        LowPower_Idle();
    } else {
        return 3;
    }
#endif
    return 0;
}

/*******************************************************************************
 * @fn          HAL_SleepInit
 *
 * @brief       配置睡眠唤醒的方式-RTC唤醒，触发模式
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       
 *
 * @return      None.
 */
void HAL_SleepInit(void)
{
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R8_SLP_WAKE_CTRL |= RB_SLP_RTC_WAKE | RB_SLP_USB_WAKE; // RTC唤醒
    R8_RTC_MODE_CTRL |= RB_RTC_TRIG_EN; // 触发模式
    R8_SAFE_ACCESS_SIG = 0; //
    PFIC_EnableIRQ(RTC_IRQn);
#endif
}
