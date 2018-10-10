//=============================================================================
//�ļ����ƣ�bsp_led.c
//���ܸ�Ҫ��LED�����ļ�
//��Ȩ���У�Դ�ع�����www.vcc-gnd.com
//�Ա����꣺http://vcc-gnd.taobao.com
//����ʱ�䣺2013-11-20
//���Է�ʽ��J-Link OB ARM SWD
//=============================================================================

#include "BSP_LED.h"

void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
}






