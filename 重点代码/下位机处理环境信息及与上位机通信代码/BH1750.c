#include <REGX52.H>
#include  <math.h>     
#include  <stdio.h>   
#include <intrins.h>
#include "delay.h"


typedef unsigned char BYTE;
#define uchar unsigned char
#define uint unsigned int
#define  SlaveAddress 0x46

sbit SCL=P1^2;
sbit SDA=P1^3;

BYTE BUF[2];

void Delay5ms(void)	
{
	unsigned char data i, j;

	i = 10;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}

void Delay5us(void)	//@12.000MHz
{
		_nop_();
}

//起始信号
void BH1750_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}

//停止信号
void BH1750_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/*************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

//接收应答信号
bit BH1750_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

//向IIC总线发送一个字节数据
void BH1750_SendByte(BYTE dat)
{
    BYTE i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
		BH1750_RecvACK();
}

//从IIC总线接收一个字节数据
BYTE BH1750_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

void Single_Write_BH1750(uchar REG_Address)    //BH1750写入指令
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址
    BH1750_Stop();                   //停止信号
}

void Multiple_read_BH1750(void)     					//BH1750读取指令
{   
		
    BH1750_Start();                          
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	BUF[0] = BH1750_RecvByte();
	BH1750_SendACK(0);
	BUF[1] = BH1750_RecvByte();
	BH1750_SendACK(1);
	 
        
    BH1750_Stop();                     
    Delay5ms();
}
float Get_Sunlight_Value()
{
	int dis_data=0;
	float temp;
	int i=0;
	Single_Write_BH1750(0x01);// power on
	Single_Write_BH1750(0x10);// H- resolution mode
	delay(180);//延时180ms
	Multiple_Read_BH1750();//连续读出数据，存储在BUF中  
	for(i=0;i<3;i++)
	//printf("0X%X ",BUF[i]);
	dis_data=BUF[0];
	dis_data=(dis_data<<8)+BUF[1];//合成数据 
	temp=(float)dis_data/1.2;
//	UsartPrintf(USART_DEBUG,"Sunlight=%0.2f lx\n",temp);
	return temp;
}

void BH1750_init()
{
		Single_Write_BH1750(0x01);
}
