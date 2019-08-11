/**
  ******************************************************************************
  * @Platform			:STM32F103RCԭ�Ӿ�Ӣ��
  * @File     			�� at24c0x.c
  * @Anthor       		�� ����AI
  * @LibraryVersion    	�� V3.5.0
  * @FileVersion   		�� V1.0.0
  * @Date       		�� 2019��08��10��
  * @Explain      	 	�� AT24C02�豸����ʵ��
  ******************************************************************************/

/*----------------------------------------------------------------------------
  Update Explain:
				2018-08-10 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
  
#include "at24c0x.h"
#include "software_i2c.h"
#include "delay.h"
#include "lcd.h"

/************************************************
*�������� �� void AT24C0X_Init
*��    �� �� AT24C0X�豸��ʼ��
*��    �� �� ��
*�� �� ֵ �� ��
*************************************************/
void AT24C0X_Init(void)
{
	I2C_Initializes();
//	I2C_Start();
}

/************************************************
*�������� �� void AT24C0X_Write_Byte
*��    �� �� AT24C0X�豸ָ����ַд��һ���ֽڵ�����
*��    �� �� WriteAddr:д��ĵ�ַ;WriteData:д�������
*�� �� ֵ �� ��
*************************************************/
void AT24C0X_Write_Byte(uint16_t WriteAddr, uint8_t WriteData)
{
	I2C_Start();
	I2C_Write_Byte(AT24C0X_ADDR);
	if (I2C_Wait_Ack() == I2C_NACK)
		;
	I2C_Write_Byte(WriteAddr%256);	//д���ַ����Ϊ8λ
	if (I2C_Wait_Ack() == I2C_NACK)
		;
	I2C_Write_Byte(WriteData);
	if (I2C_Wait_Ack() == I2C_NACK)
		;
	I2C_Stop();
	delay_ms(10);
}

/************************************************
*�������� �� uint8_t AT24C0X_Read_Byte
*��    �� �� AT24C0X�豸ָ����ַ����һ���ֽڵ�����
*��    �� �� WriteAddr:�������ݵĵ�ַ
*�� �� ֵ �� tmp:���ض�ȡ������
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
*�������� �� void AT24C0X_WriteSomeBytes
*��    �� �� AT24C0X�豸ָ����ַд��һ����������
*��    �� �� WriteAddr:д�����ݵĵ�ַ;WriteData:д�������;Len:д�����ݵĳ���
*�� �� ֵ �� ��
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
*�������� �� void AT24C0X_ReadSomeBytes
*��    �� �� AT24C0X�豸ָ����ַ��ȡһ����������
*��    �� �� WriteAddr:��ȡ���ݵĵ�ַ;WriteData:��ȡ������;Len:��ȡ���ݵĳ���
*�� �� ֵ �� ��
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
*�������� �� void AT24C0X_Check
*��    �� �� AT24C0X�豸�������ܲ���:д�롢��ȡһ��/һ������
*��    �� �� ��
*�� �� ֵ �� ��
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


/**** Copyright (C)2018 ����AI. All Rights Reserved **** END OF FILE ****/
