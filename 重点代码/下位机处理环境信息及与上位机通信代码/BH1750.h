typedef unsigned char BYTE;
extern BYTE BUF[8];

#ifndef __BH1750_H__
#define __BH1750_H__

void BH1750_Start();
void BH1750_Stop();
void BH1750_SendACK(bit ack);
bit BH1750_RecvACK();
void BH1750_SendByte(BYTE dat);
BYTE BH1750_RecvByte();

void Single_Write_BH1750(unsigned char REG_Address);
void Multiple_read_BH1750(void);
float Get_Sunlight_Value();
void BH1750_init();


#endif