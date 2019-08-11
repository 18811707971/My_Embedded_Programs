/**
  ******************************************************************************
  * @平台		:STM32F103RC原子精英版
  * @文件名     ： software_i2c.h
  * @作者       ： DreamRoad
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年04月16日
  * @描述       ： LED灯驱动实现
  ******************************************************************************/
  
#include "led.h"

void LED_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);//开启GPIOA/D时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}


