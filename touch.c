#include "touch.h"                
#include "delay.h"   
#include "lcd.h"   
#include "math.h"  //c������ѧ���ͷ�ļ�


float Ky,Kx;  //б��
int   By,Bx;  //б��

/*************************************************************************
�������ܣ����������ų�ʼ��
��ڲ���1��
��ڲ���2��
����ֵ����
˵����T_SCK��PB0  T_PEN��PB1   T_MISO��PB2    T_CS��PC13   T_MOSI��PF11
*************************************************************************/
void Touch_XPT2046_Initial(void)
{
	GPIO_InitTypeDef TOUCH;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//T_PEN��PB1   T_MISO��PB2
	TOUCH.GPIO_Mode =GPIO_Mode_IN;   //����ģʽ
	TOUCH.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //����
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOB,&TOUCH);
	
	
	//T_SCK��PB0 --����Ϊ��
	TOUCH.GPIO_Mode =GPIO_Mode_OUT;   //����ģʽ
	TOUCH.GPIO_OType = GPIO_OType_PP; //����
	TOUCH.GPIO_Pin = GPIO_Pin_0;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //����
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOB,&TOUCH);
	
		
	//T_CS��PC13  ����Ϊ��
	TOUCH.GPIO_Mode =GPIO_Mode_OUT;   //���
	TOUCH.GPIO_OType = GPIO_OType_PP; //����
	TOUCH.GPIO_Pin = GPIO_Pin_13;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //����
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOC,&TOUCH);
	
	 //T_MOSI��PF11
	TOUCH.GPIO_Mode =GPIO_Mode_OUT;   //���
	TOUCH.GPIO_OType = GPIO_OType_PP; //����
	TOUCH.GPIO_Pin = GPIO_Pin_11;
	TOUCH.GPIO_PuPd = GPIO_PuPd_UP;  //����
	TOUCH.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOF,&TOUCH);
}


//����һ���ֽ�
void SPI_Send_Byte(u8 data) //data=0xd0  1010 0000
{	
	u8 i;
	for(i=0;i<8;i++)
	{
		
		T_SCK=1;
		T_SCK=0; //���ͣ�׼��������
		if(data &(1<<(7-i)))
		{
			T_MOSI=1;  //��
		}else{		
			T_MOSI=0;  //��
		}
		delay_us(1);
		T_SCK=1; //ʱ�������ߣ����ݲŻᴫ���ȥ	
	}
}

//����һ���ֽ�
u8 SPI_Read_Byte(void)
{
	u8 data=0;
	int i;
	for(i=0;i<8;i++)
	{
		T_SCK=1;  //�ߵ�ƽ����ͨ��OUT�����ȥ
		if(T_MISO)  //�ߵ�ƽ   //data=0  0000  0000
		{                      //        1000 0000
			data = data|1<<(7-i);
		}else{
			data = data & (~1<<(7-i));
		}
		T_SCK=0;  //���ͣ�Ϊ�����½�����׼��
	}
	return data;  //���ض�����ADֵ
}

//��ȡ��������x��A/Dֵ
u16 Get_XPT2046_xvalueue(void)
{
	u8 X_H=0,X_L=0;
	u16 data=0;
	
	T_CS=0;  //��ʼͨ��
	SPI_Send_Byte(X_CMD);   //����x������
	T_SCK=0;
	delay_us(5);   //�ȴ�æ��ȥ
	
	X_H=SPI_Read_Byte();  //�����ֽ�      0x12
	X_L=SPI_Read_Byte();  //�����ֽ�      0x56	
	T_CS=1;  //����ͨ��
	
	data = (X_H<<8)| X_L;	//��ȡ16λ��ֵ
	data = data>>4;		       
	
	return data;
}

