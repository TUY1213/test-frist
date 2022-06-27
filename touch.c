#include "touch.h"                
#include "delay.h"   
#include "lcd.h"   
#include "math.h"  //c语言数学相关头文件


float Ky,Kx;  //斜率
int   By,Bx;  //斜率

/*************************************************************************
函数功能：触摸屏引脚初始化
入口参数1：
入口参数2：
返回值：无
说明：T_SCK：PB0  T_PEN：PB1   T_MISO：PB2    T_CS：PC13   T_MOSI：PF11
*************************************************************************/
void Touch_XPT2046_Initial(void)
{
	GPIO_InitTypeDef TOUCH;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//T_PEN：PB1   T_MISO：PB2
	TOUCH.GPIO_Mode =GPIO_Mode_IN;   //输入模式
	TOUCH.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //上拉
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOB,&TOUCH);
	
	
	//T_SCK：PB0 --空闲为低
	TOUCH.GPIO_Mode =GPIO_Mode_OUT;   //输入模式
	TOUCH.GPIO_OType = GPIO_OType_PP; //推挽
	TOUCH.GPIO_Pin = GPIO_Pin_0;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //下拉
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOB,&TOUCH);
	
		
	//T_CS：PC13  空闲为高
	TOUCH.GPIO_Mode =GPIO_Mode_OUT;   //输出
	TOUCH.GPIO_OType = GPIO_OType_PP; //推挽
	TOUCH.GPIO_Pin = GPIO_Pin_13;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //上拉
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOC,&TOUCH);
	
	 //T_MOSI：PF11
	TOUCH.GPIO_Mode =GPIO_Mode_OUT;   //输出
	TOUCH.GPIO_OType = GPIO_OType_PP; //推挽
	TOUCH.GPIO_Pin = GPIO_Pin_11;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //上拉
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOF,&TOUCH);
}


//发送一个字节
void SPI_Send_Byte(u8 data) //data=0xd0  1010 0000
{	
	u8 i;
	for(i=0;i<8;i++)
	{
		
		T_SCK=1;
		T_SCK=0; //拉低，准备好数据
		if(data &(1<<(7-i)))
		{
			T_MOSI=1;  //高
		}else{		
			T_MOSI=0;  //低
		}
		delay_us(1);
		T_SCK=1; //时钟线拉高，数据才会传输出去	
	}
}

//接收一个字节
u8 SPI_Read_Byte(void)
{
	u8 data=0;
	int i;
	for(i=0;i<8;i++)
	{
		T_SCK=1;  //高电平数据通过OUT传输出去
		if(T_MISO)  //高电平   //data=0  0000  0000
		{                      //        1000 0000
			data = data|1<<(7-i);
		}else{
			data = data & (~1<<(7-i));
		}
		T_SCK=0;  //拉低，为产生下降沿做准备
	}
	return data;  //返回读到的AD值
}

//获取触摸屏的x轴A/D值
u16 Get_XPT2046_xvalueue(void)
{
	u8 X_H=0,X_L=0;
	u16 data=0;
	
	T_CS=0;  //开始通信
	SPI_Send_Byte(X_CMD);   //发送x轴命令
	T_SCK=0;
	delay_us(5);   //等待忙过去
	
	X_H=SPI_Read_Byte();  //读高字节      0x12
	X_L=SPI_Read_Byte();  //读低字节      0x56	
	T_CS=1;  //结束通信
	
	data = (X_H<<8)| X_L;	//获取16位的值
	data = data>>4;		       
	
	return data;
}

