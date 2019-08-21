/**
  ******************************************************************************
  * @平台		:STM32F103RC原子精英版
  * @文件名     ： htu21d.h
  * @作者       ： DreamRoad
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年04月16日
  * @摘要       ： htu21d温湿度测量头文件
  ******************************************************************************/


#ifndef __HTU21D__H__
#define __HTU21D__H__

#include "stm32f10x.h"

#define HTU_ADDR		0x80
#define HTU_ADDR_WR (HTU_ADDR & 0xfe)
#define HTU_ADDR_RD (HTU_ADDR | 0x01)

//非主机模式
#define HTU_TEMP    0xf3
#define HTU_HUMI    0Xf5

#define HTU_SOFTWARE_RESET  0xfe

extern void Htu_Init(void);
extern float Htu_Measure(uint8_t _ucOrder);
extern void HTU_Display(void);

#endif


