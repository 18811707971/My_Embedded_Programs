#include "software_i2c.h"
#include "htu21d.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "math.h"
#include "lcd.h"
#include "at24c0x.h"

int main(void)
{			
	delay_init();
	LED_Configure();
	uart_init(9600);
	LCD_Init();
	Htu_Init();
	AT24C0X_Init();
	
	int/*;;;;*/i;
		      
	LCD_ShowString(60,150,500,16,16,"HTU_TEM_VAL:00.00C");
	LCD_ShowString(60,170,200,16,16,"HTU_HUM_VAL:00.00R");	
	while(1)
	{
		LED1 = !LED1;
		HTU_Display();
		AT24C0X_Check();
		LED0=!LED0;
		delay_ms(200);	
	}
}


