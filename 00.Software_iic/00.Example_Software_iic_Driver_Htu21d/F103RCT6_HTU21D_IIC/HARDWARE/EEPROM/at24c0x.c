/**
  ******************************************************************************
  * @Platform			:STM32F103RC原子精英版
  * @File     			： at24c0x.c
  * @Anthor       		： 霁风AI
  * @LibraryVersion    	： V3.5.0
  * @FileVersion   		： V1.0.0
  * @Date       		： 2019年08月10日
  * @Explain      	 	： AT24C02设备驱动实现
  ******************************************************************************/

/*----------------------------------------------------------------------------
  Update Explain:
				2018-08-10 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
  
#include "at24c0x.h"
#include "software_i2c.h"
#include "delay.h"
#include "lcd.h"

/************************************************
*函数名称 ： void AT24C0X_Init
*功    能 ： AT24C0X设备初始化
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void AT24C0X_Init(void)
{
	I2C_Initializes();
//	I2C_Start();
}

/************************************************
*函数名称 ： void AT24C0X_Write_Byte
*功    能 ： AT24C0X设备指定地址写入一个字节的数据
*参    数 ： WriteAddr:写入的地址;WriteData:写入的数据
*返 回 值 ： 无
*************************************************/
void AT24C0X_Write_Byte(uint16_t WriteAddr, uint8_t WriteData)
{
	I2C_Start();
	I2C_Write_Byte(AT24C0X_ADDR);
	if (I2C_Wait_Ack() == I2C_NACK)
		;
	I2C_Write_Byte(WriteAddr%256);	//写入地址设置为8位
	if (I2C_Wait_Ack() == I2C_NACK)
		;
	I2C_Write_Byte(WriteData);
	if (I2C_Wait_Ack() == I2C_NACK)
		;
	I2C_Stop();
	delay_ms(10);
}

/************************************************
*函数名称 ： uint8_t AT24C0X_Read_Byte
*功    能 ： AT24C0X设备指定地址读出一个字节的数据
*参    数 ： WriteAddr:读出数据的地址
*返 回 值 ： tmp:返回读取的数据
*************************************************/
uint8_t AT24C0X_Read_Byte(uint16_t ReadAddr)
{
	uint8_t tmp;

	I2C_Start();
	I2C_Write_Byte(AT24C0X_ADDR);
	if (I2C_Wait_Ack() == I2C_NACK)
		return 0;
	I2C_Write_Byte(ReadAddr%256);
	if (I2C_Wait_Ack() == I2C_NACK)
		return 0;
	I2C_Start();
	I2C_Write_Byte(AT24C0X_ADDR|0x01);
	if (I2C_Wait_Ack() == I2C_NACK)
		return 0;
	tmp = I2C_Read_Byte(I2C_NACK);
	I2C_Stop();

	return tmp;
}

/************************************************
*函数名称 ： void AT24C0X_WriteSomeBytes
*功    能 ： AT24C0X设备指定地址写入一连串的数据
*参    数 ： WriteAddr:写入数据的地址;WriteData:写入的数据;Len:写入数据的长度
*返 回 值 ： 无
*************************************************/
void AT24C0X_WriteSomeBytes(uint16_t WriteAddr,uint8_t *WriteData,uint16_t Len)
{
	while(Len--)
	{
		AT24C0X_Write_Byte(WriteAddr,*WriteData);
		WriteAddr++;
		WriteData++;
	}
}

/************************************************
*函数名称 ： void AT24C0X_ReadSomeBytes
*功    能 ： AT24C0X设备指定地址读取一连串的数据
*参    数 ： WriteAddr:读取数据的地址;WriteData:读取的数据;Len:读取数据的长度
*返 回 值 ： 无
*************************************************/
void AT24C0X_ReadSomeBytes(uint16_t ReadAddr,uint8_t *ReadData,uint16_t Len)
{
	while(Len--)
	{
		*ReadData++ = AT24C0X_Read_Byte(ReadAddr++);
	}
	
}

uint8_t AT_Buff = 'O';
uint8_t AT_Wbuf[] = {"AT_TEST112233"};
uint8_t AT_Rbuf[] = {"0000000000000"};

/************************************************
*函数名称 ： void AT24C0X_Check
*功    能 ： AT24C0X设备驱动功能测试:写入、读取一个/一串数据
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void AT24C0X_Check(void)
{
	uint8_t ret;
	
	AT24C0X_Write_Byte(0x01,AT_Buff);
	ret = AT24C0X_Read_Byte(0X01);
	LCD_ShowChar(80,200,AT_Buff,16,0);
	LCD_ShowChar(80,220,ret,16,0);
	
	AT24C0X_WriteSomeBytes(0x0A,AT_Wbuf,sizeof(AT_Wbuf));
	AT24C0X_ReadSomeBytes(0x0A,AT_Rbuf,sizeof(AT_Rbuf));
	LCD_ShowString(80,240,200,16,16,AT_Wbuf);
	LCD_ShowString(80,260,200,16,16,AT_Rbuf);
}


/**** Copyright (C)2018 霁风AI. All Rights Reserved **** END OF FILE ****/
