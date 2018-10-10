//=============================================================================
//�ļ����ƣ�bsp_key.c
//���ܸ�Ҫ��KEY�����ļ�
//��Ȩ���У�Դ�ع�����www.vcc-gnd.com
//�Ա����꣺http://vcc-gnd.taobao.com
//����ʱ�䣺2013-11-20
//���Է�ʽ��J-Link OB ARM SWD
//=============================================================================

#include "BSP_KEY.h"
#include "BSP_OLED.h"    	// OLED��ʾ������ͷ�ļ�
#include "BSP_USART1.h"   // USASRT1����ģ��ͷ

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
          
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
}


void KEY_EXTI_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStruct; 
     EXTI_InitTypeDef EXTI_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
              
              
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12; 
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
     GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
     GPIO_Init(GPIOC, &GPIO_InitStruct);

     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12);
              
     EXTI_ClearITPendingBit(EXTI_Line6);
     EXTI_ClearITPendingBit(EXTI_Line7);
     EXTI_ClearITPendingBit(EXTI_Line8);
     EXTI_ClearITPendingBit(EXTI_Line9);
     EXTI_ClearITPendingBit(EXTI_Line12);
              
     EXTI_InitStruct.EXTI_Line = EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9|EXTI_Line12;
     EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
     EXTI_InitStruct.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStruct); 

     NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
     NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStruct);
}



//=============================================================================
//�ļ����ƣ�main
//���ܸ�Ҫ��USART1�жϺ���
//����˵������
//�������أ�int
//=============================================================================
void EXTI4_15_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line6)!= RESET ) 
	{		
		printf("KEY1 ����\r\n");
		OLED_DispStr(64, 1, "KEY1 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line6);
	}		
	
		if ( EXTI_GetITStatus(EXTI_Line7)!= RESET ) 
	{		
		printf("KEY2 ����\r\n");
		OLED_DispStr(64, 1, "KEY2 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}		
		if ( EXTI_GetITStatus(EXTI_Line8)!= RESET ) 
	{		
		printf("KEY3 ����\r\n");
		OLED_DispStr(64, 1, "KEY3 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line8);
	}		
		if ( EXTI_GetITStatus(EXTI_Line9)!= RESET ) 
	{		
		printf("KEY4 ����\r\n");
		OLED_DispStr(64, 1, "KEY4 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line9);
	}	
			if ( EXTI_GetITStatus(EXTI_Line12)!= RESET ) 
	{		
	  printf("JOY ����\r\n");
		OLED_DispStr(64, 1, "JOY  OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line12);
	}	
	
}
