#pragma once

#include "CH573SFR.h"
#include "core_riscv.h"
#include <stdint.h>

#define Debug_UART0 0
#define Debug_UART1 1
#define Debug_UART2 2
#define Debug_UART3 3

// 是否使用的是WCH专用的编译器
#ifndef WCH_RISCV_COMPILER
#define WCH_INT_TYPE "machine"
#else
#define WCH_INT_TYPE "WCH-Interrupt-fast"
#endif

#ifndef FREQ_SYS
#define FREQ_SYS 16000000
#endif

#if (CLK_OSC32K == 1)
#define CAB_LSIFQ 32000
#else
#define CAB_LSIFQ 32768
#endif
