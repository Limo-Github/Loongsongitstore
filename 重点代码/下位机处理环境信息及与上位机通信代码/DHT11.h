#ifndef __DHT11_H__
#define __DHT11_H__

extern unsigned char rec_dat[7];   

void DHT11_start();
unsigned char DHT11_rec_byte();     
void DHT11_receive();    

#endif