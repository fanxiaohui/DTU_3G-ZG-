#ifndef __soft_i2c_H__
#define __soft_i2c_H__

#include "stm320xx8_rtu.h"
#define uchar unsigned char
#define uint  unsigned int


void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(void);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

//uchar WIN24C02_read(uint address);  //?24c02???address?????????

//void WIN24C02_write(uint address,uchar info);  //?24c02?address??????????info

void WIN24C02_init(void);   //24c02??????

void delay_nop(void);

void delay2(uint x);

//void start(void);

//void stop(void);

//void writex(uchar j);

//uchar readx(void);

//void clock(void);

#endif
