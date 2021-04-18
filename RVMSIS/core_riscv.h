/********************************** (C) COPYRIGHT  *******************************
* File Name          : core_riscv.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/07/31
* Description        : RISC-V Core Peripheral Access Layer Header File
*******************************************************************************/
#ifndef __CORE_RV3A_H__
#define __CORE_RV3A_H__

#ifdef __cplusplus
extern "C" {
#endif

/* IO definitions */
#ifdef __cplusplus
#define __I volatile /*!< defines 'read only' permissions      */
#else
#define __I volatile const /*!< defines 'read only' permissions     */
#endif
#define __O volatile /*!< defines 'write only' permissions     */
#define __IO volatile /*!< defines 'read / write' permissions   */
#define RV_STATIC_INLINE static inline

//typedef enum {SUCCESS = 0, ERROR = !SUCCESS} ErrorStatus;

typedef enum { DISABLE = 0,
    ENABLE = !DISABLE } FunctionalState;
typedef enum { RESET = 0,
    SET = !RESET } FlagStatus,
    ITStatus;

/* memory mapped structure for Program Fast Interrupt Controller (PFIC) */
typedef struct __attribute__((packed)) {
    __I uint32_t ISR[8];
    __I uint32_t IPR[8];
    __IO uint32_t ITHRESDR;
    __IO uint32_t FIBADDRR;
    __IO uint32_t CFGR;
    __I uint32_t GISR;
    uint8_t RESERVED0[0x10];
    __IO uint32_t FIOFADDRR[4];
    uint8_t RESERVED1[0x90];
    __O uint32_t IENR[8];
    uint8_t RESERVED2[0x60];
    __O uint32_t IRER[8];
    uint8_t RESERVED3[0x60];
    __O uint32_t IPSR[8];
    uint8_t RESERVED4[0x60];
    __O uint32_t IPRR[8];
    uint8_t RESERVED5[0x60];
    __IO uint32_t IACTR[8];
    uint8_t RESERVED6[0xE0];
    __IO uint8_t IPRIOR[256];
    uint8_t RESERVED7[0x810];
    __IO uint32_t SCTLR;
} PFIC_Type;

/* memory mapped structure for SysTick */
typedef struct __attribute__((packed)) {
    __IO uint32_t CTLR;
    __IO uint64_t CNT;
    __IO uint64_t CMP;
    __IO uint32_t CNTFG;
} SysTick_Type;

#define PFIC ((PFIC_Type*)0xE000E000)
#define SysTick ((SysTick_Type*)0xE000F000)

#define PFIC_KEY1 ((uint32_t)0xFA050000)
#define PFIC_KEY2 ((uint32_t)0xBCAF0000)
#define PFIC_KEY3 ((uint32_t)0xBEEF0000)

/* ##########################   define  #################################### */
#define __nop() asm volatile("nop")

/* ##########################   PFIC functions  #################################### */

/*******************************************************************************
* Function Name  : PFIC_EnableIRQ
* Description    : Enable Interrupt
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_EnableIRQ(IRQn_Type IRQn)
{
    PFIC->IENR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn)&0x1F));
}

/*******************************************************************************
* Function Name  : PFIC_DisableIRQ
* Description    : Disable Interrupt
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_DisableIRQ(IRQn_Type IRQn)
{
    uint32_t tem;
    tem = PFIC->ITHRESDR;
    PFIC->ITHRESDR = 0x10;
    PFIC->IRER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn)&0x1F));
    PFIC->ITHRESDR = tem;
    __nop();
    __nop();
}

/*******************************************************************************
* Function Name  : PFIC_GetStatusIRQ
* Description    : Get Interrupt Enable State
* Input          : IRQn: Interrupt Numbers
* Return         : 1: Interrupt Enable
*                  0: Interrupt Disable
*******************************************************************************/
RV_STATIC_INLINE uint32_t PFIC_GetStatusIRQ(IRQn_Type IRQn)
{
    return ((uint32_t)((PFIC->ISR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn)&0x1F))) ? 1 : 0));
}

/*******************************************************************************
* Function Name  : PFIC_GetPendingIRQ
* Description    : Get Interrupt Pending State
* Input          : IRQn: Interrupt Numbers
* Return         : 1: Interrupt Pending Enable
*                  0: Interrupt Pending Disable
*******************************************************************************/
RV_STATIC_INLINE uint32_t PFIC_GetPendingIRQ(IRQn_Type IRQn)
{
    return ((uint32_t)((PFIC->IPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn)&0x1F))) ? 1 : 0));
}

/*******************************************************************************
* Function Name  : PFIC_SetPendingIRQ
* Description    : Set Interrupt Pending
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_SetPendingIRQ(IRQn_Type IRQn)
{
    PFIC->IPSR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn)&0x1F));
}

/*******************************************************************************
* Function Name  : PFIC_ClearPendingIRQ
* Description    : Clear Interrupt Pending
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_ClearPendingIRQ(IRQn_Type IRQn)
{
    PFIC->IPRR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn)&0x1F));
}

/*******************************************************************************
* Function Name  : PFIC_GetActive
* Description    : Get Interrupt Active State
* Input          : IRQn: Interrupt Numbers
* Return         : 1: Interrupt Active
*                  0: Interrupt No Active
*******************************************************************************/
RV_STATIC_INLINE uint32_t PFIC_GetActive(IRQn_Type IRQn)
{
    return ((uint32_t)((PFIC->IACTR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn)&0x1F))) ? 1 : 0));
}

/*******************************************************************************
* Function Name  : PFIC_SetPriority
* Description    : Set Interrupt Priority
* Input          : IRQn: Interrupt Numbers
*                  priority: bit7:pre-emption priority
*                            bit6-bit4: subpriority
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
    PFIC->IPRIOR[(uint32_t)(IRQn)] = priority;
}

/*******************************************************************************
* Function Name  : PFIC_FastINT0CFG
* Description    : Set fast Interrupt 0,
* Input          : IRQn: Interrupt Numbers
*                  addr: interrupt service addr
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_FastINT0CFG(IRQn_Type IRQn, uint32_t addr)
{
    PFIC->FIBADDRR = addr & 0xF0000000;
    PFIC->FIOFADDRR[0] = (IRQn << 24) | (addr & 0x000FFFFF);
}

/*******************************************************************************
* Function Name  : PFIC_FastINT1CFG
* Description    : Set fast Interrupt 1,
* Input          : IRQn: Interrupt Numbers
*                  addr: interrupt service addr
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_FastINT1CFG(IRQn_Type IRQn, uint32_t addr)
{
    PFIC->FIBADDRR = addr & 0xF0000000;
    PFIC->FIOFADDRR[1] = (IRQn << 24) | (addr & 0x000FFFFF);
}

/*******************************************************************************
* Function Name  : PFIC_FastINT2CFG
* Description    : Set fast Interrupt 2, occupied if use BLE.
* Input          : IRQn: Interrupt Numbers
*                  addr: interrupt service addr
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_FastINT2CFG(IRQn_Type IRQn, uint32_t addr)
{
    PFIC->FIBADDRR = addr & 0xF0000000;
    PFIC->FIOFADDRR[2] = (IRQn << 24) | (addr & 0x000FFFFF);
}

/*******************************************************************************
* Function Name  : PFIC_FastINT3CFG
* Description    : Set fast Interrupt 3, occupied if use BLE.
* Input          : IRQn: Interrupt Numbers
*                  addr: interrupt service addr
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_FastINT3CFG(IRQn_Type IRQn, uint32_t addr)
{
    PFIC->FIBADDRR = addr & 0xF0000000;
    PFIC->FIOFADDRR[3] = (IRQn << 24) | (addr & 0x000FFFFF);
}

/*******************************************************************************
* Function Name  : __SEV
* Description    : Wait for Events
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__((always_inline)) RV_STATIC_INLINE void __SEV(void)
{
    PFIC->SCTLR |= (1 << 3);
}

/*******************************************************************************
* Function Name  : __WFI
* Description    : Wait for Interrupt
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__((always_inline)) RV_STATIC_INLINE void __WFI(void)
{
    PFIC->SCTLR &= ~(1 << 3); // wfi
    asm volatile("wfi");
}

/*******************************************************************************
* Function Name  : __WFE
* Description    : Wait for Events
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__((always_inline)) RV_STATIC_INLINE void __WFE(void)
{
    PFIC->SCTLR |= (1 << 3) | (1 << 5); // (wfi->wfe)+(__sev)
    asm volatile("wfi");
    PFIC->SCTLR |= (1 << 3);
    asm volatile("wfi");
}

/*******************************************************************************
* Function Name  : PFIC_SetFastIRQ
* Description    : Set Fast Interrupt
* Input          : addr：Fast interrupt service function base address.
*                  IRQn：Interrupt Numbers
*                  num：Fast Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_SetFastIRQ(uint32_t addr, IRQn_Type IRQn, uint8_t num)
{
    if (num > 3)
        return;
    PFIC->FIBADDRR = addr;
    PFIC->FIOFADDRR[num] = ((uint32_t)IRQn << 24) | (addr & 0xfffff);
}

/*******************************************************************************
* Function Name  : PFIC_SystemReset
* Description    : Initiate a system reset request
* Input          : None
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_SystemReset(void)
{
    PFIC->CFGR = PFIC_KEY3 | (1 << 7);
}

/*******************************************************************************
* Function Name  : PFIC_HaltPushCfg
* Description    : Enable Hardware Stack
* Input          : NewState: DISABLE or ENABLE
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_HaltPushCfg(FunctionalState NewState)
{
    if (NewState != DISABLE) {
        PFIC->CFGR = PFIC_KEY1;
    } else {
        PFIC->CFGR = PFIC_KEY1 | (1 << 0);
    }
}

/*******************************************************************************
* Function Name  : PFIC_INTNestCfg
* Description    : Enable Interrupt Nesting
* Input          : NewState: DISABLE or ENABLE
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void PFIC_INTNestCfg(FunctionalState NewState)
{
    if (NewState != DISABLE) {
        PFIC->CFGR = PFIC_KEY1;
    } else {
        PFIC->CFGR = PFIC_KEY1 | (1 << 1);
    }
}

#define SysTick_LOAD_RELOAD_Msk (0xFFFFFFFFFFFFFFFF)
#define SysTick_CTRL_RELOAD_Msk (1 << 8)
#define SysTick_CTRL_CLKSOURCE_Msk (1 << 2)
#define SysTick_CTRL_TICKINT_Msk (1 << 1)
#define SysTick_CTRL_ENABLE_Msk (1 << 0)

RV_STATIC_INLINE uint32_t SysTick_Config(uint64_t ticks)
{
    if ((ticks - 1) > SysTick_LOAD_RELOAD_Msk)
        return (1); /* Reload value impossible */

    SysTick->CMP = ticks - 1; /* set reload register */
    PFIC_EnableIRQ(SysTick_IRQn);
    SysTick->CTLR = SysTick_CTRL_RELOAD_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; /* Enable SysTick IRQ and SysTick Timer */
    return (0); /* Function successful */
}

#ifdef __cplusplus
}
#endif

#endif /* __CORE_RV3A_H__ */
