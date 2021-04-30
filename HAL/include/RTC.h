/********************************** (C) COPYRIGHT *******************************
* File Name          : RTC.h
* Author             : WCH
* Version            : V1.0
* Date               : 2016/04/12
* Description        : 
*******************************************************************************/

/******************************************************************************/
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_config.h"

#define RTC_TIMER_MAX_VALUE 0xa8c00000

extern volatile uint32_t RTCTigFlag;

/*
 * Initialize time Service.
 */
void HAL_TimeInit(void);

/*
 * System Clock Service.
 */
extern void RTC_SetTignTime(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif
