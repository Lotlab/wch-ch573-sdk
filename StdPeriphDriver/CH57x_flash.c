/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH57x_flash.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description
 *******************************************************************************/

#include "CH57x_flash.h"

/*******************************************************************************
 * Function Name  : FLASH_ROM_READ
 * Description    : Read Flash
 * Input          :
 * Return         : None
 *******************************************************************************/
void FLASH_ROM_READ(uint32_t StartAddr, void* Buffer, uint32_t len)
{
    uint32_t i, Length = (len + 3) >> 2;
    uint32_t* pCode = (uint32_t*)StartAddr;
    uint32_t* pBuf = (uint32_t*)Buffer;

    for (i = 0; i < Length; i++) {
        *pBuf++ = *pCode++;
    }
}
