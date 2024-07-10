#include <REGX52.H>

sbit RS=P0^7;
sbit RW=P0^6;
sbit E=P0^5;
#define lcd_DatePort P2

void lcd_delay(void)	//1ms
{
	unsigned char data i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

void lcd_writecmd(unsigned char Command)	//写指令
{
	RS=0;
	RW=0;
	lcd_DatePort=Command;
	E=1;
	lcd_delay();
	E=0;
	lcd_delay();
}

void lcd_writedat(unsigned char Date)  //写数据
{
	RS=1;
	RW=0;
	lcd_DatePort=Date;
	E=1;
	lcd_delay();
	E=0;
	lcd_delay();
}

void lcd_init()	//lcd1602初始化
{
	lcd_writecmd(0x38);
	lcd_writecmd(0x0C);
	lcd_writecmd(0x06);
	lcd_writecmd(0x01);
}

int lcd_pow(int X,int Y)	//X的Y次方
{
	unsigned char i;
	int result=1;
	for(i=0;i<Y;i++)
		result*=X;
	return result;
}

void lcd_setcursor(unsigned char Line,unsigned char Column)	//设置光标位置
{
	if(Line==1)
		lcd_writecmd(0x80|(Column-1));
	else
		lcd_writecmd(0x80|(Column-1)+0x40);
}

void lcd_showchar(unsigned char Line,unsigned char Column,unsigned char Char)
{
	lcd_setcursor(Line,Column);
	lcd_writedat(Char);
}

void lcd_showstring(unsigned char Line,unsigned char Column,unsigned char String[])		//显示字符串
{
	unsigned char i;
	lcd_setcursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
		lcd_writedat(String[i]);
}

void lcd_shownum(unsigned char Line,unsigned char Column,int Number,unsigned int Length)
{
	unsigned char i;
	lcd_setcursor(Line,Column);
	for(i=Length;i>0;i--)
		lcd_writedat('0'+Number/lcd_pow(10,i-1)%10);	//加0即加了0x30，因为实际数字和ASC码表的数字差0x30
}

void lcd_showsignednum(unsigned char Line,unsigned char Column,int Number,unsigned int Length)
{
	unsigned char i;
	unsigned int Number1;
	lcd_setcursor(Line,Column);
	if(Number>=0)
	{
		lcd_writedat('+');
		Number1=Number;
	}
	else
	{
		lcd_writedat('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	lcd_writedat('0'+Number1/lcd_pow(10,i-1)%10);	
	//加0即加了0x30，因为实际数字和ASC码表的数字差0x30,即将想要显示的数字在表中找到对应的位置
}

void lcd_showhexnum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned int Length)	//显示16进制数
{
	unsigned char i,singlenum;	//singlenum中间量，因为16进制大于9为abc
	lcd_setcursor(Line,Column);
	for(i=Length;i>0;i--)
	{
	singlenum=Number/lcd_pow(16,i-1)%16;
	if(singlenum<10)
		lcd_writedat('0'+singlenum);
	else
		lcd_writedat('A'+singlenum-10);	
	//singlenum大于10，再加上0x41，超了10，所以减掉
	}
}

void lcd_showbinnum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned int Length)	//显示2进制数
{
		unsigned char i;
	lcd_setcursor(Line,Column);
	for(i=Length;i>0;i--)
		lcd_writedat('0'+Number/lcd_pow(2,i-1)%2);	//加0即加了0x30，因为实际数字和ASC码表的数字差0x30
}