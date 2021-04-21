
#ifndef __CH57x_FLASH_H__
#define __CH57x_FLASH_H__

#include "CH57x.h"

#ifdef __cplusplus
extern "C" {
#endif

void FLASH_ROM_READ(uint32_t StartAddr, void* Buffer, uint32_t len); /* 读取Flash-ROM */

#ifdef __cplusplus
}
#endif

#endif // __CH57x_FLASH_H__
