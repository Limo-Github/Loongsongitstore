#ifndef __XPT2046H__
#define __XPT2046H__

#define XPT2046_XP_8 		0x9c		 	//0x8c 第二位到第四位为多路选择器
#define XPT2046_YP_8 		0xdc
#define XPT2046_VBAT_8 	0xac
#define XPT2046_AUX_8 	0xec

#define XPT2046_XP_12 		0x94		//0x84 第二位到第四位为多路选择器
#define XPT2046_YP_12 		0xd4
#define XPT2046_VBAT_12 	0xa4
#define XPT2046_AUX_12 		0xe4

unsigned int XPT2046_ReadAD(unsigned char Command);

#endif