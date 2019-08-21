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
*函数名称 ： void AT24C0X_WriteByte
*功    能 ： AT24C0X设备指定地址写入一个字节的数据
*参    数 ： WriteAddr:写入的地址;WriteData:写入的数据
*返 回 值 ： 无
*************************************************/
void AT24C0X_WriteByte(uint16_t _usWriteAddr, uint8_t _ucWriteData)
{
	I2C_Start();
	I2C_WriteByte(AT24C0X_ADDR);
	if (I2C_WaitAck() == I2C_NACK)
		;
	I2C_WriteByte(_usWriteAddr%256);	//写入地址设置为8位
	if (I2C_WaitAck() == I2C_NACK)
		;
	I2C_WriteByte(_ucWriteData);
	if (I2C_WaitAck() == I2C_NACK)
		;
	I2C_Stop();
	delay_ms(10);
}

/************************************************
*函数名称 ： uint8_t AT24C0X_ReadByte
*功    能 ： AT24C0X设备指定地址读出一个字节的数据
*参    数 ： WriteAddr:读出数据的地址
*返 回 值 ： tmp:返回读取的数据
*************************************************/
uint8_t AT24C0X_ReadByte(uint16_t _usReadAddr)
{
	uint8_t ucTmp;

	I2C_Start();
	I2C_WriteByte(AT24C0X_ADDR);
	if (I2C_WaitAck() == I2C_NACK)
		return 0;
	I2C_WriteByte(_usReadAddr%256);
	if (I2C_WaitAck() == I2C_NACK)
		return 0;
	I2C_Start();
	I2C_WriteByte(AT24C0X_ADDR|0x01);
	if (I2C_WaitAck() == I2C_NACK)
		return 0;
	ucTmp = I2C_ReadByte(I2C_NACK);
	I2C_Stop();

	return ucTmp;
}

/************************************************
*函数名称 ： void AT24C0X_WriteSomeBytes
*功    能 ： AT24C0X设备指定地址写入一连串的数据
*参    数 ： WriteAddr:写入数据的地址;WriteData:写入的数据;Len:写入数据的长度
*返 回 值 ： 无
*************************************************/
void AT24C0X_WriteSomeBytes(uint16_t _usWriteAddr,uint8_t *_pWriteData,uint16_t _usLen)
{
	while(_usLen--)
	{
		AT24C0X_WriteByte(_usWriteAddr,*_pWriteData);
		_usWriteAddr++;
		_pWriteData++;
	}
}

/************************************************
*函数名称 ： void AT24C0X_ReadSomeBytes
*功    能 ： AT24C0X设备指定地址读取一连串的数据
*参    数 ： WriteAddr:读取数据的地址;WriteData:读取的数据;Len:读取数据的长度
*返 回 值 ： 无
*************************************************/
void AT24C0X_ReadSomeBytes(uint16_t _usReadAddr,uint8_t *_pReadData,uint16_t _usLen)
{
	while(_usLen--)
	{
		*_pReadData++ = AT24C0X_ReadByte(_usReadAddr++);
	}
	
}

uint8_t g_ATBuff = 'O';
uint8_t g_ATWbuf[] = {"AT_TEST112233"};
uint8_t g_ATRbuf[] = {"0000000000000"};

/************************************************
*函数名称 ： void AT24C0X_Check
*功    能 ： AT24C0X设备驱动功能测试:写入、读取一个/一串数据
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void AT24C0X_Check(void)
{
	uint8_t ucRetVal;
	
	AT24C0X_WriteByte(0x01,g_ATBuff);
	ucRetVal = AT24C0X_ReadByte(0X01);
	LCD_ShowChar(80,200,g_ATBuff,16,0);
	LCD_ShowChar(80,220,ucRetVal,16,0);
	
	AT24C0X_WriteSomeBytes(0x0A,g_ATWbuf,sizeof(g_ATWbuf));
	AT24C0X_ReadSomeBytes(0x0A,g_ATRbuf,sizeof(g_ATRbuf));
	LCD_ShowString(80,240,200,16,16,g_ATWbuf);
	LCD_ShowString(80,260,200,16,16,g_ATRbuf);
}


/**** Copyright (C)2018 霁风AI. All Rights Reserved **** END OF FILE ****/
