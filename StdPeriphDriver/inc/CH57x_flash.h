/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH57x_flash.h
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2021/11/17
 * Description
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#ifndef __CH57x_FLASH_H__
#define __CH57x_FLASH_H__

#include "CH57x.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   读取Flash-ROM
 *
 * @param   StartAddr   - read address
 * @param   Buffer      - read buffer
 * @param   len         - read len
 */
void FLASH_ROM_READ(uint32_t StartAddr, void* Buffer, uint32_t len);

#ifdef CH58x

uint8_t UserOptionByteConfig(FunctionalState RESET_EN, FunctionalState BOOT_PIN, FunctionalState UART_NO_KEY_EN,
    uint32_t FLASHProt_Size);

uint8_t UserOptionByteClose_SWD(void);

void UserOptionByte_Active(void);

#endif

#ifdef __cplusplus
}
#endif

#endif // __CH57x_FLASH_H__
