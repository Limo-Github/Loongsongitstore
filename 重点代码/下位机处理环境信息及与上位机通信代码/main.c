#include <stdio.h>
#include <REGX52.H>
#include "lcd1602.h"
#include "DHT11.h"
#include "BH1750.h"
#include "delay.h"
#include "intrins.h"

unsigned char shidu[4];
unsigned char wendu[4];
int i;

unsigned char   ge,shi,bai,qian,wan;       //显示变量
int    dis_data;      
void UartInit(void)		//9600bps@11.0592MHz
{
	SM0 = 0;
	SM1 = 1;
	REN = 1;
	TR1 = 1;
	TMOD |= 0x20;
	TH1=0xfd;
	TL1=0xfd;
   ES=1;           //????????
	EA=1;

              
}
 

void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;   //?SBUF????
	while(!TI);//if(TI==1) {TI=0;}??????????TI?1,????,??????0
	TI=0;
}
void uart_send_string(char *str)
{
 	while(*str!='\0')
	{
	 	UART_SendByte(*str);	
		str++;
	}
}//变量

void main()
{
  float temp;

	lcd_init();
	BH1750_init();       
	lcd_showstring(2,1,"Light:"); 
	lcd_showstring(1, 1, "Hum:");
	lcd_showstring(1, 8, "Tem:");
	lcd_showstring(1,7,"%");		  
	lcd_showstring(1,15,"C"); 
 // lcd_showstring(2,12,"lx"); 

	while(1)
	{
			DHT11_receive();
		 UartInit();
		
	
			
			for(i=0;i<3;i++) 
				 shidu[i]= rec_dat[i]; 
			for(i=4;i<7;i++)
				 wendu[i-4] = rec_dat[i];
			
			lcd_showstring(1,5,shidu);
			lcd_showstring(1,12,wendu);
		
	
		 temp = Get_Sunlight_Value();
		 /* Single_Write_BH1750(0x01);   //上电
      Single_Write_BH1750(0x10);   	//高频率连续测量
		  delay(180);              		//选择模式需要延时，此处为最大值
      Multiple_read_BH1750();       	//读取数据存入BUF
      dis_data = BUF[0];
      dis_data=(dis_data<<8)+Multiple_read_BH1750();	//数据合成
			temp=(int)dis_data;		//计算数据 */
	    uart_send_string(wendu);   
      uart_send_string(shidu);	
			uart_send_string(BUF);
			
			lcd_shownum(2,7,temp,5);

}
	}



