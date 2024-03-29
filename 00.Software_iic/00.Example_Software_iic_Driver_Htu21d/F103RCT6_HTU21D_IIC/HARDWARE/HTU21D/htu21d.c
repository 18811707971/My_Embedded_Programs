/**
  ******************************************************************************
  * @平台		:STM32F103RC原子精英版
  * @文件名     ： htu21d.c
  * @作者       ： DreamRoad
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2018年04月17日
  * @描述       ： 温湿度传感器HTU21D测试
  ******************************************************************************/

 //头文件
#include "software_i2c.h"
#include "delay.h"
#include <math.h>
#include "htu21d.h"
#include "lcd.h"
#include "usart.h"  

/************************************************
*函数名称 ： void Htu_Init(void)
*功    能 ： htu21d初始化
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void Htu_Init(void)
  {
	  
	I2C_Initializes();
	I2C_Start();
	I2C_WriteByte(HTU_ADDR_WR);	//写I2C器件地址
	I2C_WaitAck();
	I2C_WriteByte(HTU_SOFTWARE_RESET);		//软复位
	I2C_WaitAck();
	I2C_Stop();
	delay_ms(15);		//软复位时间最多需要15ms
  }
  
/************************************************
*函数名称 ： float Htu_Measure(uint8_t order)
*功    能 ： htu21d测量温湿度
*参    数 ： order：测量触发命令
*返 回 值 ： temp--温度值  humi--湿度值
*************************************************/
float Htu_Measure(uint8_t _ucOrder)
 {
	 uint8_t ucMsb,ucLsb;
	 volatile float f_Temp,f_Humi,f_RetVal;
	 
	 I2C_Start();
	 
	 I2C_WriteByte(HTU_ADDR_WR);		//地址+写命令
	 if(I2C_WaitAck()==I2C_NACK)
		 return 0;
	 
	 I2C_WriteByte(_ucOrder);		//触发测量命令
	 if(I2C_WaitAck()==I2C_NACK)
		 return 0;
	 
			 do{
				 delay_ms(5);
				 I2C_Start();
				 I2C_WriteByte(HTU_ADDR_RD);		//地址+读命令
			 }while(I2C_WaitAck()==I2C_NACK);
			 
			 ucMsb = I2C_ReadByte(I2C_ACK);
			 delay_ms(5);
			 ucLsb = I2C_ReadByte(I2C_ACK);
			 delay_ms(5);
			 I2C_ReadByte(I2C_NACK);
			 I2C_Stop();
			 
			 ucLsb &= 0xfc;		//设置分辨率,最低两位为0,温度:14位;湿度:12位 	
			 f_RetVal = ucMsb * 256 + ucLsb;/*MSB=(MSB<<=8)+LSB;即将MSB移位到高8位*/
			 
			 if(_ucOrder == HTU_TEMP)
			 {
				 f_Temp = (175.72)*f_RetVal/65536-46.85;//温度:T= -46.85 + 175.72 * ST/2^16
				 return f_Temp;
			 }
			 else if(_ucOrder == HTU_HUMI)
			 {
				 f_Humi = (f_RetVal*125)/65536-6;//湿度: RH%= -6 + 125 * SRH/2^16
				 return f_Humi;
			 }
			 else
				return 0;
} 
 
/************************************************
*函数名称 ： void HTU_Display(void)
*功    能 ： htu21d测量温湿度显示
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void HTU_Display(void)
{
	u16 usTemp;
	volatile float f_RetVal;
	u8 ucTest[10];
	
	f_RetVal = Htu_Measure(HTU_TEMP);//得到温度值
	printf("The htu measure temp is :%4.2fC \n",f_RetVal);
	
	sprintf((char*)ucTest,"%4.2f",f_RetVal);		//LCD显示方式1：sprintf函数将结果打印到test数组里,转换成字符串
	printf("test is %sC \n",ucTest);
	printf("\r\n");
	LCD_ShowString(100,100,200,16,16,"sprintf:");		
	LCD_ShowString(164,100,200,16,16,ucTest);
	
	usTemp = f_RetVal;			//LCD显示方式2:将得到的数值拆分成整数和小数直接显示在液晶
	LCD_ShowxNum(156,150,usTemp,2,16,0);
	f_RetVal -= usTemp;
	f_RetVal *= 100;		//保留两位小数
	LCD_ShowxNum(180,150,f_RetVal,2,16,0);
		
	f_RetVal= Htu_Measure(HTU_HUMI);		//得到湿度值
	printf("The htu measure humi is :%4.2fRH \n",f_RetVal);
	usTemp = f_RetVal;
	LCD_ShowxNum(156,170,usTemp,2,16,0);//显示湿度值
	f_RetVal -= usTemp;
	f_RetVal *= 100;
	LCD_ShowxNum(180,170,f_RetVal,2,16,0x80);
}

	
/**** Copyright (C)2018 霁风AI. All Rights Reserved **** END OF FILE ****/




