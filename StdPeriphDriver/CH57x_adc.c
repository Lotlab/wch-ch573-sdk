/********************************** (C) COPYRIGHT *******************************
* File Name          : CH57x_adc.c
* Author             : WCH
* Version            : V1.1
* Date               : 2020/04/01
* Description 
*******************************************************************************/

#include "CH57x_adc.h"

/*******************************************************************************
* Function Name  : ADC_DataCalib_Rough
* Description    : 采样数据粗调,获取偏差值
* Input          : None
* Return         : 偏差值
*******************************************************************************/
signed short ADC_DataCalib_Rough(void) // 采样数据粗调,获取偏差值
{
    uint16_t i;
    uint32_t sum = 0;
    uint8_t ch = 0; // 备份通道
    uint8_t ctrl = 0; // 备份控制寄存器

    ch = R8_ADC_CHANNEL;
    ctrl = R8_ADC_CFG;

    ADC_ChannelCfg(6); // 6/7/10/11 可选
    R8_ADC_CFG |= RB_ADC_OFS_TEST | RB_ADC_POWER_ON; // 进入测试模式
    R8_ADC_CONVERT = RB_ADC_START;
    while (R8_ADC_CONVERT & RB_ADC_START)
        ;
    for (i = 0; i < 16; i++) {
        R8_ADC_CONVERT = RB_ADC_START;
        while (R8_ADC_CONVERT & RB_ADC_START)
            ;
        sum += (~R16_ADC_DATA) & RB_ADC_DATA;
    }
    sum = (sum + 8) >> 4;
    R8_ADC_CFG &= ~RB_ADC_OFS_TEST; // 关闭测试模式

    R8_ADC_CHANNEL = ch;
    R8_ADC_CFG = ctrl;
    return (2048 - sum);
}

/*******************************************************************************
* Function Name  : ADC_ExtSingleChSampInit
* Description    : 外部信号单通道采样初始化
* Input          : sp:
					refer to ADC_SampClkTypeDef
				   ga:
					refer to ADC_SignalPGATypeDef
* Return         : None
*******************************************************************************/
void ADC_ExtSingleChSampInit(ADC_SampClkTypeDef sp, ADC_SignalPGATypeDef ga)
{
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_ADC_CFG = RB_ADC_POWER_ON
        | RB_ADC_BUF_EN
        | (sp << 6)
        | (ga << 4);
}

/*******************************************************************************
* Function Name  : ADC_ExtDiffChSampInit
* Description    : 外部信号差分通道采样初始化
* Input          : sp:
					refer to ADC_SampClkTypeDef
				   ga:
					refer to ADC_SignalPGATypeDef
* Return         : None
*******************************************************************************/
void ADC_ExtDiffChSampInit(ADC_SampClkTypeDef sp, ADC_SignalPGATypeDef ga)
{
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_ADC_CFG = RB_ADC_POWER_ON
        | RB_ADC_DIFF_EN
        | (sp << 6)
        | (ga << 4);
}

/*******************************************************************************
* Function Name  : ADC_InterTSSampInit
* Description    : 内置温度传感器采样初始化
* Input          : None
* Return         : None
*******************************************************************************/
void ADC_InterTSSampInit(void)
{
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_TEM_SENSOR = RB_TEM_SEN_PWR_ON;
    R8_ADC_CHANNEL = CH_INTE_VTEMP;
    R8_ADC_CFG = RB_ADC_POWER_ON
        | RB_ADC_DIFF_EN
        | (3 << 4);
}

/*******************************************************************************
* Function Name  : ADC_InterBATSampInit
* Description    : 内置电池电压采样初始化
* Input          : None
* Return         : None
*******************************************************************************/
void ADC_InterBATSampInit(void)
{
    R8_TKEY_CFG &= ~RB_TKEY_PWR_ON;
    R8_ADC_CHANNEL = CH_INTE_VBAT;
    R8_ADC_CFG = RB_ADC_POWER_ON
        | RB_ADC_BUF_EN
        | (0 << 4); // 使用-12dB模式，
}

/*******************************************************************************
* Function Name  : TouchKey_ChSampInit
* Description    : 触摸按键通道采样初始化
* Input          : None
* Return         : None
*******************************************************************************/
void TouchKey_ChSampInit(void)
{
    R8_ADC_CFG = RB_ADC_POWER_ON | RB_ADC_BUF_EN | (2 << 4);
    R8_TKEY_CFG |= RB_TKEY_PWR_ON;
}

/*******************************************************************************
* Function Name  : ADC_ExcutSingleConver
* Description    : ADC执行单次转换
* Input          : None
* Return         : ADC转换后的数据
*******************************************************************************/
uint16_t ADC_ExcutSingleConver(void)
{
    R8_ADC_CONVERT = RB_ADC_START;
    while (R8_ADC_CONVERT & RB_ADC_START)
        ;

    return (R16_ADC_DATA & RB_ADC_DATA);
}

/*******************************************************************************
* Function Name  : TouchKey_ExcutSingleConver
* Description    : TouchKey转换后数据
* Input          : charg:  Touchkey充电时间,5bits有效, t=charg*Tadc
*                  disch：    Touchkey放电时间,3bits有效, t=disch*Tadc
* Return         : 当前TouchKey等效数据
*******************************************************************************/
uint16_t TouchKey_ExcutSingleConver(uint8_t charg, uint8_t disch)
{
    R8_TKEY_COUNT = (disch << 5) | (charg & 0x1f);
    R8_TKEY_CONVERT = RB_TKEY_START;
    while (R8_TKEY_CONVERT & RB_TKEY_START)
        ;
    return (R16_ADC_DATA & RB_ADC_DATA);
}

/*******************************************************************************
* Function Name  : ADC_GetCurrentTS
* Description    : 获取当前采样的温度值（℃）
* Input          : ts_v：当前温度传感器采样输出
* Return         : 转换后的温度值（℃）
*******************************************************************************/
int ADC_GetCurrentTS(uint16_t ts_v)
{
    uint16_t vol_ts;
    uint16_t D85_tem, D85_vol;
    uint16_t D25_tem, D25_vol;
    uint16_t temperK;
    uint32_t temp;
    uint8_t sum, sumck;
    int cal;

    temperK = 64; // mV/16^C
    vol_ts = (ts_v * 1060) >> 11;
    temp = (*((uint32_t*)ROM_TMP_25C_ADDR));
    D25_tem = temp;
    D25_vol = (temp >> 16);

    if (D25_vol != 0) { // 默认系数换算
        // T = T85 + (V-V85)*16/D25
        cal = (D25_tem * temperK + vol_ts * 16 + (temperK >> 1) - D25_vol * 16) / temperK;
        return (cal);
    } else { // 内置系数换算  D25_tem
        temp = (*((uint32_t*)ROM_TMP_85C_ADDR));
        sum = (uint8_t)(temp >> 24); // 最高字节
        sumck = (uint8_t)(temp >> 16);
        sumck += (uint8_t)(temp >> 8);
        sumck += (uint8_t)temp;
        if (sum != sumck)
            return 0xff; // 校验和出错

        temperK = D25_tem; // D25_tem = temperK
        D85_tem = (uint16_t)((temp >> 16) & 0x00ff);
        D85_vol = (uint16_t)temp;

        // T = T85 + (V-V85)*16/D25
        cal = (D85_tem * temperK + vol_ts * 16 + (temperK >> 1) - D85_vol * 16) / temperK;
        return (cal);
    }
}
