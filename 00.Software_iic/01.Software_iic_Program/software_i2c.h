/**
  ******************************************************************************
  * @平台		:STM32F103RC原子精英版
  * @文件名     ： software_i2c.h
  * @作者       ： DreamRoad
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年04月16日
  * @描述       ： 软件模拟I2C头文件
  ******************************************************************************/


#ifndef __SOFTWARE_I2C__H__
#define __SOFTWARE_I2C__H__

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/* 宏定义 --------------------------------------------------------------------*/
#define RCC_PCLK_I2C			RCC_APB2Periph_GPIOC

#define PORT_I2C_SCL			GPIOC
#define PORT_I2C_SDA			GPIOC

#define I2C_SCL					GPIO_Pin_12
#define I2C_SDA					GPIO_Pin_11

#if 1
	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
#define I2C_SCL_LOW				GPIO_ResetBits(PORT_I2C_SCL, I2C_SCL)	
#define I2C_SCL_HIGH			GPIO_SetBits(PORT_I2C_SCL, I2C_SCL)
#define I2C_SDA_LOW				GPIO_ResetBits(PORT_I2C_SDA, I2C_SDA)
#define I2C_SDA_HIGH			GPIO_SetBits(PORT_I2C_SDA, I2C_SDA)

#define I2C_SDA_READ  GPIO_ReadInputDataBit(PORT_I2C_SCL, I2C_SDA)
	
#else
	/* 选择直接寄存器操作实现IO读写 */
#define I2C_SCL_LOW				(PORT_I2C_SCL->BRR  = I2C_SCL)
#define I2C_SCL_HIGH			(PORT_I2C_SCL->BSRR = I2C_SCL )
#define I2C_SDA_LOW				(PORT_I2C_SDA->BRR  = I2C_SDA)
#define I2C_SDA_HIGH			(PORT_I2C_SDA->BSRR = I2C_SDA)

#define I2C_SDA_READ			(PORT_I2C_SDA->IDR & I2C_SDA)//读取输入电平状态,判断IO的电平

#endif

#define I2C_ACK					0				//应答
#define I2C_NACK				1				//非应答

//**************函数声明***************************************//
extern void I2C_Delay(uint16_t cnt);
extern void I2C_GPIO_Configure(void);
extern void I2C_Initializes(void);
extern void I2C_SDA_SetInput(void);
extern void I2C_SDA_SetOutput(void);
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Write_Byte(uint8_t dat);
extern uint8_t I2C_Read_Byte(uint8_t ack);

extern uint8_t I2C_Wait_Ack(void);
extern void I2C_Get_Ack(void);
extern void I2C_Get_NAck(void);

#endif

/**** Copyright (C)2018 霁风AI. All Rights Reserved **** END OF FILE ****/


