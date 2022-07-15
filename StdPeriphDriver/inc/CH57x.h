#pragma once

#ifdef CH57x
#include "CH573SFR.h"
#include "core_riscv_ch573.h"
#else
#ifdef CH58x
#include "CH583SFR.h"
#include "core_riscv_ch583.h"
#else
#error "Please define your chip, CH57x or CH58x"
#endif
#endif

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

#ifndef __HIGH_CODE
#define __HIGH_CODE __attribute__((section(".highcode")))
#endif

#ifndef __INTERRUPT
#define __INTERRUPT __attribute__((interrupt(WCH_INT_TYPE)))
#endif

#ifndef FREQ_SYS
#define FREQ_SYS 16000000
#endif

#ifndef SAFEOPERATE
#define SAFEOPERATE \
    __nop();        \
    __nop()
#endif

#ifdef CLK_OSC32K
 #if ( CLK_OSC32K == 1 )
 #define CAB_LSIFQ     	32000
 #else
 #define CAB_LSIFQ     	32768
 #endif
#else
#define CAB_LSIFQ       32000
#endif
