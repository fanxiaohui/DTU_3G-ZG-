#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm320xx8_rtu.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//STM32������
//IIC���� ����	   
//
//
//�޸�����:2012/9/9
//�汾��V1.0
//
// 
//									  
//////////////////////////////////////////////////////////////////////////////////
extern void AT24C02_SDA_IO_SET(unsigned char io_set);
//IO��������
#define SDA_IN   AT24C02_SDA_IO_SET(1)
#define SDA_OUT  AT24C02_SDA_IO_SET(0)

//IO��������	 
#define IIC_SCL(x)    GPIO_WriteBit(GPIOB,GPIO_Pin_6,x) //SCL
#define IIC_SDA(x)    GPIO_WriteBit(GPIOB,GPIO_Pin_7,x) //SDA	 
#define READ_SDA      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















