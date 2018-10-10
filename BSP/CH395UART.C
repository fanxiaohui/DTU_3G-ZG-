/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395UART.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ UART�������ӵ�Ӳ������� V1.0
*                      
*******************************************************************************/

/* Ӳ����غ궨�� */
/* �����е�Ӳ�����ӷ�ʽ����(ʵ��Ӧ�õ�·���Բ����޸��������弰�ӳ���) */
/* ��Ƭ��������    CH395оƬ������
      TXD                  RXD
      RXD                  TXD       */
			
#include "stm32f0xx.h"		
#include "CH395INC.h"	

//extern UART_HandleTypeDef huart3;
extern uint8_t RxBuffer1[];
extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];
extern uint16_t TxCount;
extern uint16_t NbrOfDataToRead;
extern uint16_t NbrOfDataToTransfer;
extern uint8_t runningFlag;
#define	CH395_INT_PIN_WIRE	   GPIOA->IDR & GPIO_Pin_5 
//#define CH395_INT_WIRE             INT1                          /* �ٶ�CH395��INT#����,���δ������ôҲ����ͨ����ѯ�����ж�״̬��ʵ�� */

#define UART_INIT_BAUDRATE         9600                          /* Ĭ��ͨѶ������9600bps,����ͨ��Ӳ�������趨ֱ��ѡ����ߵ�CH395��Ĭ��ͨѶ������ */
#define UART_WORK_BAUDRATE         115200                         /* ��ʽͨѶ������57600bps */
#define xEndCH395Cmd()     {}                                        /* �������������SPIģʽ����Ч */

/*******************************************************************************
* Function Name  : mDelayuS
* Description    : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : us---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS(unsigned char us)
{
	  us*=2;
    while(us --);                                                    /* MCS51@24MHz */
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : ms---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS(unsigned char ms)
{
    while(ms --) 
    {
        mDelayuS(250);
        mDelayuS(250);
        mDelayuS(250);
        mDelayuS(250);
    }
}

/*******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : Ӳ����ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
    /* �����Ƭ��ֻ��һ������,��ô�����ֹͨ��������������Ϣ */
   	GPIO_InitTypeDef GPIO_InitStructure;

	 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		GPIO_SetBits(GPIOA, GPIO_Pin_4, GPIO_Pin_SET);
//		//HAL_GPIO_WritePin(GPIOA, GPIO_Pin_4, GPIO_PIN_SET);
}

/*******************************************************************************
* Function Name  : Set_MCU_BaudRate
* Description    : ���õ�Ƭ��������
*                  ����Ƭ���л�����ʽͨѶ������ 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetMCUBaudRate(void)                                
{
//  TL2 = RCAP2L = 0 - 18432000/32/UART_WORK_BAUDRATE;               /* 18.432MHz���� */
	
	USART_InitTypeDef USART_InitStructure;

   USART_InitStructure.USART_BaudRate = 115200;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
	 USART_Init(USART3, &USART_InitStructure);
}

void USART3_Send_Byte( UINT8 dat )
{
	USART3->TDR = ( dat & (UINT16)0x01FF );								
	while( !( USART3->ISR & USART_FLAG_TXE ) ); 		
}

/********************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395д����
* Input          : cmd 8λ��������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(unsigned char cmd)                                      
{
	  USART_Cmd(USART3, ENABLE);	
	  USART3_Send_Byte( SER_SYNC_CODE1 );							 /* ??????1?????? */
	  USART3_Send_Byte( SER_SYNC_CODE2 );							 /* ??????2?????? */
	  USART3_Send_Byte( cmd );							 		 /* ????? */
	//  mDelayuS( 2 );  	                                                        
}

/********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mdata 8λ����
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(unsigned char mdata)
{                                                                    /* ��CH395д���� */
    USART_Cmd(USART3, ENABLE);			 
    USART3_Send_Byte(mdata);
}

/********************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395������
* Input          : None
* Output         : None
* Return         : 8λ����
*******************************************************************************/
unsigned char xReadCH395Data(void)                                          /* ��CH395������ */
{
	UINT32 i;
	for( i = 0; i < 0xFFFFF0; i ++ ) 
	{  
		/* ?????? */		
		if( USART3->ISR & USART_FLAG_RXNE ) 
		{  
			/* ????? */
			return( ( UINT8 )( USART3->RDR & (UINT16)0x01FF ) );/* ???? */
		}
	}
	return( 0 );  												 /* ???????? */
}

/*******************************************************************************
* Function Name  : Query395Interrupt
* Description    : ??CH395??(INT#???)
* Input          : None
* Output         : None
* Return         : ??????
*******************************************************************************/
UINT8 Query395Interrupt( void )
{
	return( CH395_INT_PIN_WIRE ? FALSE : TRUE );  
}
/**************************** endfile *************************************/