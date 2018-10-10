/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��SPI.c
 * ����    ��SPI��ʼ��
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-08-20
 * Ӳ������  :SCLK--PA5;MISO---PA6;MOSI--PA7
 * ���Է�ʽ��J-Link-OB
********************************************************************************/
#include "spi.h"

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
SPI_InitTypeDef  SPI_InitStructure;


#define TM7706_SPI                           SPI2
#define TM7706_SPI_CLK                       RCC_APB1Periph_SPI2

#define TM7706_SPI_SCK_PIN                   GPIO_Pin_13                /* PA.05 */
#define TM7706_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOA */
#define TM7706_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define TM7706_SPI_SCK_SOURCE                GPIO_PinSource3
#define TM7706_SPI_SCK_AF                    GPIO_AF_0

#define TM7706_SPI_MISO_PIN                 GPIO_Pin_13                  /* PA.06 */
#define TM7706_SPI_MISO_GPIO_PORT           GPIOB                      /* GPIOA */
#define TM7706_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define TM7706_SPI_MISO_SOURCE              GPIO_PinSource4
#define TM7706_SPI_MISO_AF                  GPIO_AF_0

#define TM7706_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PA.07 */
#define TM7706_SPI_MOSI_GPIO_PORT            GPIOB                     /* GPIOA */
#define TM7706_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define TM7706_SPI_MOSI_SOURCE               GPIO_PinSource5
#define TM7706_SPI_MOSI_AF                   GPIO_AF_0

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(TM7706_SPI_SCK_GPIO_CLK | TM7706_SPI_MISO_GPIO_CLK |TM7706_SPI_MOSI_GPIO_CLK , ENABLE);	
  RCC_APB1PeriphClockCmd(TM7706_SPI_CLK, ENABLE);
 
	
	GPIO_PinAFConfig(TM7706_SPI_SCK_GPIO_PORT,TM7706_SPI_SCK_SOURCE,TM7706_SPI_SCK_AF);
	GPIO_PinAFConfig(TM7706_SPI_MISO_GPIO_PORT,TM7706_SPI_MISO_SOURCE,TM7706_SPI_MISO_AF);
	GPIO_PinAFConfig(TM7706_SPI_MOSI_GPIO_PORT,TM7706_SPI_MOSI_SOURCE,TM7706_SPI_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = TM7706_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;/*!< I/O output speed: High 50 MHz */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//����
	GPIO_Init(TM7706_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	/*!< ���� SD_SPI �ܽ�: MISO */
	GPIO_InitStructure.GPIO_Pin = TM7706_SPI_MISO_PIN;
	GPIO_Init(TM7706_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	/*!< ���� SD_SPI �ܽ�: MOSI */
	GPIO_InitStructure.GPIO_Pin = TM7706_SPI_MOSI_PIN;
	GPIO_Init(TM7706_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
		
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(TM7706_SPI, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
  SPI_RxFIFOThresholdConfig(TM7706_SPI, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(TM7706_SPI, ENABLE); //ʹ��SPI����
	
	SPIx_ReadWriteByte(0xff);//��������

}   

void SPIx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(SD_SPI_MOSI_GPIO_CLK | SD_SPI_MISO_GPIO_CLK |SD_SPI_SCK_GPIO_CLK , ENABLE);	
  RCC_APB2PeriphClockCmd(SD_SPI_CLK, ENABLE);
 
	
	GPIO_PinAFConfig(SD_SPI_SCK_GPIO_PORT,SD_SPI_SCK_SOURCE,SD_SPI_SCK_AF);
	GPIO_PinAFConfig(SD_SPI_MISO_GPIO_PORT,SD_SPI_MISO_SOURCE,SD_SPI_MISO_AF);
	GPIO_PinAFConfig(SD_SPI_MOSI_GPIO_PORT,SD_SPI_MOSI_SOURCE,SD_SPI_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;/*!< I/O output speed: High 50 MHz */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//����
	GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	/*!< ���� SD_SPI �ܽ�: MISO */
	GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
	GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	/*!< ���� SD_SPI �ܽ�: MOSI */
	GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
	GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
		
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SD_SPI, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
  SPI_RxFIFOThresholdConfig(SD_SPI, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SD_SPI, ENABLE); //ʹ��SPI����
	
	SPIx_ReadWriteByte(0xff);//��������

}   

 /**
  * @file   SPIx_SetSpeed
  * @brief  �ٶ�����
  * @param  SpeedSet:�ٶȲ���
  * @retval ��
  */  
void SPIx_SetSpeed(uint8_t SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
	SPI_Init(SD_SPI, &SPI_InitStructure);
	SPI_Cmd(SD_SPI,ENABLE);
} 

void SPI2_SetSpeed(uint8_t SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
	SPI_Init(TM7706_SPI, &SPI_InitStructure);
	SPI_Cmd(TM7706_SPI,ENABLE);
} 

 /**
  * @file   SPIx_ReadWriteByte
  * @brief  ��дһ���ֽ�
  * @param  TxData:Ҫд����ֽ�
  * @retval ��ȡ�����ֽ�
  */

uint8_t SPI2_WriteByte(uint8_t TxData)
{		
		uint8_t retry=0;				 
		while (SPI_I2S_GetFlagStatus(TM7706_SPI, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  	
		SPI_SendData8(TM7706_SPI, TxData); //ͨ������SPIx����һ������
}

uint8_t SPI2_ReadByte(uint8_t *RxData)
{		
		uint8_t retry=0;				 
		
	
		retry++;
		if(retry>200)*RxData=0;
		  						    
		*RxData=SPI_ReceiveData8(TM7706_SPI); //����ͨ��SPIx������յ�����	

}

uint8_t SPIx_ReadWriteByte(uint8_t TxData)
{		
		uint8_t retry=0;				 
		while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
   	
		SPI_SendData8(SD_SPI, TxData); //ͨ������SPIx����һ������
		retry=0;
		while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
		{
		;
		} //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
		return SPI_ReceiveData8(SD_SPI); //����ͨ��SPIx������յ�����	

}































