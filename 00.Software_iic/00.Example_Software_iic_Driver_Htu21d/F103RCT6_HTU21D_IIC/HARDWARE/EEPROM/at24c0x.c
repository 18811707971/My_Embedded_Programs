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
*�������� �� void AT24C0X_WriteByte
*��    �� �� AT24C0X�豸ָ����ַд��һ���ֽڵ�����
*��    �� �� WriteAddr:д��ĵ�ַ;WriteData:д�������
*�� �� ֵ �� ��
*************************************************/
void AT24C0X_WriteByte(uint16_t _usWriteAddr, uint8_t _ucWriteData)
{
	I2C_Start();
	I2C_WriteByte(AT24C0X_ADDR);
	if (I2C_WaitAck() == I2C_NACK)
		;
	I2C_WriteByte(_usWriteAddr%256);	//д���ַ����Ϊ8λ
	if (I2C_WaitAck() == I2C_NACK)
		;
	I2C_WriteByte(_ucWriteData);
	if (I2C_WaitAck() == I2C_NACK)
		;
	I2C_Stop();
	delay_ms(10);
}

/************************************************
*�������� �� uint8_t AT24C0X_ReadByte
*��    �� �� AT24C0X�豸ָ����ַ����һ���ֽڵ�����
*��    �� �� WriteAddr:�������ݵĵ�ַ
*�� �� ֵ �� tmp:���ض�ȡ������
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
*�������� �� void AT24C0X_WriteSomeBytes
*��    �� �� AT24C0X�豸ָ����ַд��һ����������
*��    �� �� WriteAddr:д�����ݵĵ�ַ;WriteData:д�������;Len:д�����ݵĳ���
*�� �� ֵ �� ��
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
*�������� �� void AT24C0X_ReadSomeBytes
*��    �� �� AT24C0X�豸ָ����ַ��ȡһ����������
*��    �� �� WriteAddr:��ȡ���ݵĵ�ַ;WriteData:��ȡ������;Len:��ȡ���ݵĳ���
*�� �� ֵ �� ��
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
*�������� �� void AT24C0X_Check
*��    �� �� AT24C0X�豸�������ܲ���:д�롢��ȡһ��/һ������
*��    �� �� ��
*�� �� ֵ �� ��
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


/**** Copyright (C)2018 ����AI. All Rights Reserved **** END OF FILE ****/
