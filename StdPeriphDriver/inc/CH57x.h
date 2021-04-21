#pragma once

#include <stdint.h>
#include "CH573SFR.h"
#include "core_riscv.h"

#define Debug_UART0 0
#define Debug_UART1 1
#define Debug_UART2 2
#define Debug_UART3 3

#ifndef DEBUG_UART
#define DEBUG_UART Debug_UART0
#endif

#ifndef FREQ_SYS
#define FREQ_SYS 16000000
#endif

#if (CLK_OSC32K == 1)
#define CAB_LSIFQ 32000
#else
#define CAB_LSIFQ 32768
#endif
