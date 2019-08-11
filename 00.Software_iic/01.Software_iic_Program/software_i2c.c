/**
  ******************************************************************************
  * @Platform			:STM32F103RC原子精英版
  * @File     			： software_i2c.c
  * @Anthor       		： 霁风AI
  * @LibraryVersion    	： V3.5.0
  * @FileVersion   		： V1.0.0
  * @Date       		： 2019年08月10日
  * @Explain      	 	： IIC软件模拟驱动实现
  ******************************************************************************/

/*----------------------------------------------------------------------------
  Update Explain:
				2018-08-10 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/


 //头文件
#include "software_i2c.h"	//iic driver of software imitation
#include "delay.h"  

/************************************************
函数名称 ： I2C_Delay
功    能 ： I2C延时(非标准延时,需根据MCU速度 调节大小)
参    数 ： 无
返 回 值 ： 无
*************************************************/
void I2C_Delay(uint16_t cnt)
{
	while(cnt--);
}

/************************************************
*函数名称 ： I2C_GPIO_Configure
*功    能 ： I2C对应管脚配置,开漏输出,外接上拉
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_GPIO_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_I2C,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
	
	GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(PORT_I2C_SCL, &GPIO_InitStructure);
}

/************************************************
*函数名称 ： void I2C_Init
*功    能 ： I2C初始化,时钟,数据均为高电平
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_Initializes(void)
{
	I2C_GPIO_Configure();
	I2C_SCL_HIGH;
	I2C_SDA_HIGH;
}

/************************************************
*函数名称 ： void I2C_SDA_SetInput
*功    能 ： I2C读取数据方向设置为输入
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_SDA_SetInput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(PORT_I2C_SDA,&GPIO_InitStructure);
}

/************************************************
*函数名称 ： void I2C_SDA_SetOutput
*功    能 ： I2C读取数据方向设置为输出
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_SDA_SetOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//开漏输出
	GPIO_Init(PORT_I2C_SDA,&GPIO_InitStructure);
}

/************************************************
*函数名称 ： void I2C_Start
*功    能 ： I2C开始:SCL高电平期间,SDA从高变为低
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_Start(void)
{
	I2C_SCL_HIGH;
	delay_us(4);
	
	I2C_SDA_HIGH;
	delay_us(4);
	I2C_SDA_LOW;
	delay_us(4);
	
	I2C_SCL_LOW;
	delay_us(4);
}

/************************************************
*函数名称 ： void I2C_Stop
*功    能 ： I2C停止:SCL高电平期间,SDA从低变为高
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_Stop(void)
{
	I2C_SCL_LOW;
	I2C_SDA_LOW;
	delay_us(4);
	
	I2C_SCL_HIGH;
	delay_us(4);
	
	I2C_SDA_HIGH;
	delay_us(4);
}

/************************************************
*函数名称 ： void I2C_Write_Byte
*功    能 ： I2C写一个字节
*参    数 ： dat：传输数据
*返 回 值 ： 无
*************************************************/
void I2C_Write_Byte(uint8_t dat)
{
	uint8_t cnt;
	
	for(cnt=0;cnt<8;cnt++)
	{
		I2C_SCL_LOW;		//SCL低电平,允许数据改变
		delay_us(4);
		
		if(dat&0x80)		//从高位开始传输
			I2C_SDA_HIGH;
		else
			I2C_SDA_LOW;
		dat <<= 1;
		delay_us(4);
		
		I2C_SCL_HIGH;		//数据稳定,发送给从机
		delay_us(4);
	}
	I2C_SCL_LOW;		//第9个时钟,SCL低电平,等待应答信号来到
	delay_us(10);
}

/******* *****************************************
*函数名称 ： uint8_t I2C_Read_Byte
*功    能 ： I2C读一个字节
*参    数 ： ack：I2C_ACK-应答(0)：I2C_NACK-非应答(1)
*返 回 值 ： dat:读取的数据
*************************************************/
uint8_t I2C_Read_Byte(uint8_t ack)
{
	uint8_t cnt,dat = 0;
	
	I2C_SCL_LOW;
	delay_us(4);
	
	I2C_SDA_HIGH;
	
	I2C_SDA_SetInput();		//切换SDA传输方向
	
	for(cnt=0;cnt<8;cnt++)
	{
		I2C_SCL_HIGH;		//SCL高电平时SDA上的数据达到稳定
		delay_us(4);		//延时等待信号稳定
		
		dat <<= 1;
		if(I2C_SDA_READ)
			dat |= 0x01;
		else
			dat &= 0xfe;
		I2C_SCL_LOW;		//允许数据改变
		delay_us(4);
	}
	I2C_SDA_SetOutput();
	if(ack)
		I2C_Get_NAck();
	else
		I2C_Get_Ack();
	
	return dat;
}

/************************************************
*函数名称 ： uint8_t I2C_Wait_Ack
*功    能 ： I2C等待应答
*参    数 ： 无
*返 回 值 ： ack:I2C_ACK or I2C_NACK
*************************************************/
#if 1

uint8_t I2C_Wait_Ack(void)
{
	uint8_t ack;
	
	I2C_SDA_HIGH;
	delay_us(4);
	I2C_SCL_HIGH;		//此时判断是否有应答
	delay_us(4);
	
	if(I2C_SDA_READ)
		ack = I2C_NACK;
	else
		ack = I2C_ACK;
	
	I2C_SCL_LOW;
	delay_us(4);
	
	return ack;
}

/************************************************
*函数名称 ： uint8_t I2C_Wait_Ack
*功    能 ： I2C等待应答,等待一定时间等待应答信号到来
*参    数 ： 无
*返 回 值 ： 0:I2C_ACK or 1:I2C_NACK
*************************************************/
#else

uint8_t I2C_Wait_Ack(void)
{
	uint8_t WaitTime = 0;
	
	I2C_SDA_HIGH;
	delay_us(2);
	I2C_SCL_HIGH;
	delay_us(2);
	while(I2C_SDA_READ)
	{
		WaitTime++;
		if(WaitTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_LOW;
	return 0;
}

#endif

/************************************************
*函数名称 ： void I2C_Get_Ack
*功    能 ： I2C得到应答
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_Get_Ack(void)
{
	I2C_SCL_LOW;
	delay_us(4);
	I2C_SDA_LOW;
	delay_us(4);
	I2C_SCL_HIGH;		//SCL高电平
	delay_us(4);
	I2C_SCL_LOW;
	delay_us(4);
	I2C_SDA_HIGH;		//释放SDA
}

/************************************************
*函数名称 ： void I2C_Get_NAck
*功    能 ： I2C无应答
*参    数 ： 无
*返 回 值 ： 无
*************************************************/
void I2C_Get_NAck(void)
{
	I2C_SCL_LOW;
	delay_us(4);
	I2C_SDA_HIGH;
	delay_us(4);
	I2C_SCL_HIGH;		//SCL高电平
	delay_us(4);
	I2C_SCL_LOW;
	delay_us(4);
}

/**** Copyright (C)2018 霁风AI. All Rights Reserved **** END OF FILE ****/



