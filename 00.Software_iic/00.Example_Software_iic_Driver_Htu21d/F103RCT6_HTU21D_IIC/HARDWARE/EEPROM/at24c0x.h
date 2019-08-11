/**
  ******************************************************************************
  * @Platform			:STM32F103RCԭ�Ӿ�Ӣ��
  * @File     			�� at24c0x.h
  * @Anthor       		�� ����AI
  * @LibraryVersion   	�� V3.5.0
  * @FileVersion   		�� V1.0.0
  * @Date       		�� 2019��08��10��
  * @Explain      		�� AT24C02�豸����ͷ�ļ�
  ******************************************************************************/

#ifndef __AT24C0X_H
#define __AT24C0X_H

#include "stm32f10x.h"

#define AT24C0X_ADDR	0xA0		//AT24C02 iic address 

extern void AT24C0X_Init(void);
extern void AT24C0X_Write_Byte(uint16_t WriteAddr, uint8_t WriteData);
extern uint8_t AT24C0X_Read_Byte(uint16_t ReadAddr);
extern void AT24C0X_WriteSomeBytes(uint16_t WriteAddr,uint8_t *WriteData,uint16_t Len);
extern void AT24C0X_ReadSomeBytes(uint16_t ReadAddr,uint8_t *ReadData,uint16_t Len);

extern void AT24C0X_Check(void);

#endif

/**** Copyright (C)2018 ����AI. All Rights Reserved **** END OF FILE ****/

