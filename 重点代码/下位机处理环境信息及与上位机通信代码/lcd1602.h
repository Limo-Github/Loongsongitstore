#ifndef __LCD1602_H__
#define __LCD1602_H__

void lcd_init();
void lcd_showchar(unsigned char Line,unsigned char Column,unsigned char Char);
void lcd_showstring(unsigned char Line,unsigned char Column,unsigned char String[]);		//显示字符串
void lcd_shownum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned int Length);
void lcd_showsignednum(unsigned char Line,unsigned char Column,int Number,unsigned int Length);
void lcd_showhexnum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned int Length);	//显示16进制数
void lcd_showbinnum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned int Length); //显示16进制数
void lcd_setcursor(unsigned char Line,unsigned char Column);	//设置光标位置
void lcd_writedat(unsigned char Date);  //写数据
void lcd_writecmd(unsigned char Commond);

#endif