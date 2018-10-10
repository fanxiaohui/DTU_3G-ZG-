#include "myiic.h"
//#include "delay.h"
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

extern void delay_us(uint16_t ms);
//��ʼ��IIC

void AT24C02_SDA_IO_SET(unsigned char io_set) //SDA????????
{
	 	GPIO_InitTypeDef GPIO_InitStructure;
    if(io_set==0)
  {
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else if(io_set==1)
  {
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  //????
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else
  {;}
}
void delay_nop(void)
{
  uint8_t i=10; //i=10??1.5us//???????? ,??????5????
   while(i--);

}


void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB10,PB11 �����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT;     //sda�����
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT;//sda�����
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL(1); 
	IIC_SDA(1);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN;      //SDA����Ϊ����  
	IIC_SDA(1);delay_us(1);	   
	IIC_SCL(1);delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT;
	IIC_SDA(0);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT;
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{         
   u8 i,temp,temp1;

   temp=txd;

   for (i=0;i<8;i++)
   {
      temp1=temp & 0x80;
      temp=temp<<1;

    IIC_SCL(0);
      delay_nop();


  if(temp1==0x80)
   {IIC_SDA(1);}
  else
   {IIC_SDA(0);}
           delay_nop();
    	IIC_SCL(1);
      delay_nop();
   }
     IIC_SCL(0);
   delay_nop();
   IIC_SDA(1);
   delay_nop();
} 	    

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	u8 i,j,k=0;

   //SCL=0;
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    delay_nop(); 
 //SDA=1;
 GPIO_SetBits(GPIOB,GPIO_Pin_7);

 AT24C02_SDA_IO_SET(1);
   for (i=0;i<8;i++)
   {
      delay_nop();
      //SCL=1;
   GPIO_SetBits(GPIOB,GPIO_Pin_6);
      delay_nop();
      //if (SDA==1) j=1;
  if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1 ) 
   {j=1;}
      else 
   {j=0;}
      k=(k<<1)|j;
      //SCL=0;
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
   }
    AT24C02_SDA_IO_SET(0);
   delay_nop();
   return(k);
}



