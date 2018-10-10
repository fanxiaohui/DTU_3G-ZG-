

#ifndef _BSP_OLED_H
#define _BSP_OLED_H

#include "stm32f0xx.h"



/* ������� */
enum
{
	FC_ST_16 = 0,		/* ����15x16���� ����x�ߣ� */
	FC_ST_12 = 1		/* ����12x12���� ����x�ߣ� */
};

/* �������Խṹ, ����LCD_DispStr() */
typedef struct
{
	uint16_t usFontCode;	/* ������� 0 ��ʾ16���� */
	uint16_t usTextColor;	/* ������ɫ 0 �� 1 */
	uint16_t usBackColor;	/* ���ֱ�����ɫ 0 �� 1 */
	uint16_t usSpace;		/* ���ּ�࣬��λ = ���� */
}FONT_T;


extern  FONT_T tFont12;
extern  FONT_T tFont16;


/* �ɹ��ⲿģ����õĺ��� */
void OLED_Init(void);
void OLED_InitHard(void);
void OLED_DispOn(void);
void OLED_DispOff(void);
void OLED_StartDraw(void);
void OLED_EndDraw(void);
void OLED_ClrScr(uint8_t _ucMode);
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY);
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr);

uint32_t OLED_shuji(uint32_t x,uint32_t y,uint32_t shuji,uint32_t stlye);

#endif


