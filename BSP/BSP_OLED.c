//=============================================================================
//�ļ����ƣ�bsp_oled.c
//���ܸ�Ҫ��OLED�����ļ�
//��Ȩ���У�Դ�ع�����www.vcc-gnd.com
//�Ա����꣺http://vcc-gnd.taobao.com
//����ʱ�䣺2013-11-20
//���Է�ʽ��J-Link OB ARM SWD
//=============================================================================
#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>
#include "BSP_OLED.h"
#include "fonts.h"

FONT_T tFont12, tFont16;

#define TRUN_180_EN			/* ��ת180�� */

void  OLEDDelay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/*
*********************************************************************************************************
*   VCC ----------- 3.3V
*   GND ----------- GND
*   CS ----------- TP_NCS   (PB12)
*   RST ----------- NRESET (Ҳ���Բ�����)(PB0)
*   D0/SCK ----------- TP_SCK   (PB13)
*   D1/SDIN ----------- TP_MOSI  (PB15)
*********************************************************************************************************
*/
#define SSD_CS_1()	GPIOB->BSRR = GPIO_Pin_12
#define SSD_CS_0()	GPIOB->BRR = GPIO_Pin_12

#define SSD_SDIN_1() GPIOB->BSRR = GPIO_Pin_15
#define SSD_SDIN_0() GPIOB->BRR = GPIO_Pin_15

#define SSD_SCK_1() GPIOB->BSRR = GPIO_Pin_13
#define SSD_SCK_0() GPIOB->BRR = GPIO_Pin_13
/* 12864 OLED���Դ澵��ռ��1K�ֽ�. ��8�У�ÿ��128���� */
static uint8_t s_ucGRAM[8][128];

/* Ϊ�˱���ˢ����Ļ��̫ǿ������ˢ����־��
0 ��ʾ��ʾ����ֻ��д����������д����1 ��ʾֱ��д����ͬʱд�������� */
static uint8_t s_ucUpdateEn = 1;

static void OLED_ConfigGPIO(void);
static void OLED_WriteCmd(uint8_t _ucCmd);
static void OLED_WriteData(uint8_t _ucData);
static void OLED_BufToPanel(void);

/*
*********************************************************************************************************
*	�� �� ��: OLED_InitHard
*	����˵��: ��ʼ��OLED��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_InitHard(void)
{
	OLED_ConfigGPIO();

	/* �ϵ��ӳ� */

		OLEDDelay(0xfffff);

	 /* ģ�鳧���ṩ��ʼ������ */
	OLED_WriteCmd(0xAE);	/* �ر�OLED�����ʾ(����) */
	OLED_WriteCmd(0x00);	/* �����е�ַ��4bit */
	OLED_WriteCmd(0x10);	/* �����е�ַ��4bit */
	OLED_WriteCmd(0x40);	/* ������ʼ�е�ַ����5bit 0-63���� Ӳ�����*/

	OLED_WriteCmd(0x81);	/* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
	OLED_WriteCmd(0xCF);	/* ���öԱȶȲ��� */

	OLED_WriteCmd(0xA1);	/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
	OLED_WriteCmd(0xA6);	/* A6 : ����������ʾģʽ; A7 : ����Ϊ����ģʽ */

	OLED_WriteCmd(0xA8);	/* ����COM·�� */
	OLED_WriteCmd(0x3F);	/* 1 ->��63+1��· */

	OLED_WriteCmd(0xD3);	/* ������ʾƫ�ƣ�˫�ֽ����*/
	OLED_WriteCmd(0x00);	/* ��ƫ�� */

	OLED_WriteCmd(0xD5);	/* ������ʾʱ�ӷ�Ƶϵ��/��Ƶ�� */
	OLED_WriteCmd(0x80);	/* ���÷�Ƶϵ��,��4bit�Ƿ�Ƶϵ������4bit����Ƶ�� */

	OLED_WriteCmd(0xD9);	/* ����Ԥ������� */
	OLED_WriteCmd(0xF1);	/* [3:0],PHASE 1; [7:4],PHASE 2; */

	OLED_WriteCmd(0xDA);	/* ����COM��Ӳ�����߷�ʽ */
	OLED_WriteCmd(0x12);

	OLED_WriteCmd(0xDB);	/* ���� vcomh ��ѹ���� */
	OLED_WriteCmd(0x40);	/* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */

	OLED_WriteCmd(0x8D);	/* ���ó��ã����¸�������ʹ�ã� */
	OLED_WriteCmd(0x14);	/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
	OLED_WriteCmd(0xAF);	/* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispOn
*	����˵��: ����ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispOn(void)
{
	OLED_WriteCmd(0x8D);	/* ���ó��ã����¸�������ʹ�ã� */
	OLED_WriteCmd(0x14);	/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
	OLED_WriteCmd(0xAF);	/* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispOff
*	����˵��: �ر���ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispOff(void)
{
	OLED_WriteCmd(0x8D);	/* ���ó��ã����¸�������ʹ�ã�*/
	OLED_WriteCmd(0x10);	/* 0x14 ʹ�ܳ��ã�0x10 �ǹر� */
	OLED_WriteCmd(0xAE);	/* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_StartDraw
*	����˵��: ��ʼ��ͼ���Ժ��ͼ����ֻ��д������������д����Դ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_StartDraw(void)
{
	s_ucUpdateEn = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_EndDraw
*	����˵��: ������ͼ��������������ˢ�µ�����Դ档 OLED_StartDraw() �� OLED_EndDraw() ����ɶ�ʹ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_EndDraw(void)
{
	s_ucUpdateEn = 1;
	OLED_BufToPanel();
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ClrScr
*	����˵��: ����
*	��    �Σ�_ucMode : 0 ��ʾȫ�ڣ� 0xFF��ʾȫ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_ClrScr(uint8_t _ucMode)
{
	uint8_t i,j;

	for (i = 0 ; i < 8; i++)
	{
		for (j = 0 ; j < 128; j++)
		{
			s_ucGRAM[i][j] = _ucMode;
		}
	}

	if (s_ucUpdateEn == 1)
	{
		OLED_BufToPanel();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_BufToPanel
*	����˵��: ���������еĵ�������д�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_BufToPanel(void)
{
	uint8_t i,j;

	for (i = 0 ; i< 8; i++)
	{
		OLED_WriteCmd (0xB0 + i);	/* ����ҳ��ַ��0~7�� */
		OLED_WriteCmd (0x00);		/* �����е�ַ�ĵ͵�ַ */
		OLED_WriteCmd (0x10);		/* �����е�ַ�ĸߵ�ַ */

		for (j = 0 ; j < 128; j++)
		{
			OLED_WriteData(s_ucGRAM[i][j]);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispStr
*	����˵��: ����Ļָ�����꣨���Ͻ�Ϊ0��0����ʾһ���ַ���
*	��    �Σ�
*		_usX : X���꣬����12864������ΧΪ��0 - 127��
*		_usY : Y���꣬����12864������ΧΪ��0 - 63��
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[24 * 24 / 8];	/* ���֧��24������ */
	uint8_t m, width;
	uint8_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;

	/* �������ṹΪ��ָ�룬��ȱʡ��16���� */	
	if (_tFont->usFontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		pAscDot = g_Ascii12;
		pHzDot = g_Hz12;
	}
	else
	{
		/* ȱʡ��16���� */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		pHzDot = g_Hz16;
	}

	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
		if (code1 < 0x80)
		{
			/* ��ascii�ַ������Ƶ�buf */
			memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			width = font_width / 2;
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* ����16�����ֵ����ַ
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* ��ȫ�ֿ� */
				/* �˴���Ҫ�����ֿ��ļ����λ�ý����޸� */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}

		y = _usY;
		/* ��ʼˢLCD */
		for (m = 0; m < font_height; m++)	/* �ַ��߶� */
		{
			x = _usX;															 
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					OLED_PutPixel(x, y, _tFont->usTextColor);	/* ����������ɫΪ����ɫ */
				}
				else
				{
					OLED_PutPixel(x, y, _tFont->usBackColor);	/* ����������ɫΪ���ֱ���ɫ */
				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
		}
		_usX += width + _tFont->usSpace;	/* �е�ַ���� */
		_ptr++;			/* ָ����һ���ַ� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_PutPixel
*	����˵��: ��1������
*	��    �Σ�
*			_usX,_usY : ��������
*			_ucColor  ��������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;
#ifdef TRUN_180_EN
	const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
	const uint8_t aAndTab[8] = {0xFE, 0xFD, 0xFB, 0xF7,0xEF,0xDF,0xBF,0x7F};

	ucPageAddr = _usY / 8;
	ucColAddr = 127 - _usX;
#else
	const uint8_t aOrTab[8]  = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	const uint8_t aAndTab[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

	ucPageAddr = 7 - _usY / 8;
	ucColAddr = _usX;
#endif

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (_ucColor == 0)
	{
		ucValue &= aAndTab[_usY % 8];
	}
	else
	{
		ucValue |= aOrTab[_usY % 8];
	}
	s_ucGRAM[ucPageAddr][ucColAddr] = ucValue;

	if (s_ucUpdateEn == 1)
	{
		OLED_WriteCmd (0xB0 + ucPageAddr);					/* ����ҳ��ַ��0~7�� */
		OLED_WriteCmd (0x00 + (ucColAddr & 0x0F));			/* �����е�ַ�ĵ͵�ַ */
		OLED_WriteCmd (0x10 + ((ucColAddr >> 4) & 0x0F));	/* �����е�ַ�ĸߵ�ַ */
		OLED_WriteData(ucValue);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_GetPixel
*	����˵��: ��ȡ1������
*	��    �Σ�
*			_usX,_usY : ��������
*	�� �� ֵ: ��ɫֵ (0, 1)
*********************************************************************************************************
*/
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;
#ifdef TRUN_180_EN
	ucPageAddr = _usY / 8;
	ucColAddr = 127 - _usX;
#else
	ucPageAddr = 7 - _usY / 8;
	ucColAddr = _usX;
#endif

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (ucValue & (_usY % 8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawLine
*	����˵��: ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
*	��    �Σ�
*			_usX1, _usY1 ����ʼ������
*			_usX2, _usY2 ����ֹ��Y����
*			_ucColor     ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* ���� Bresenham �㷨����2��仭һ��ֱ�� */

	OLED_PutPixel(_usX1 , _usY1 , _ucColor);

	/* ��������غϣ���������Ķ�����*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* ȷ������1���Ǽ�1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* ѭ������ */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			OLED_PutPixel ( y , x , _ucColor) ;
		}
		else
		{
			OLED_PutPixel ( x , y , _ucColor) ;
		}
		x += tx ;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawPoints
*	����˵��: ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ��
*	��    �Σ�
*			x, y     ����������
*			_ucColor ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		OLED_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _ucColor);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawRect
*	����˵��: ���ƾ��Ρ�
*	��    �Σ�
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	OLED_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* �� */
	OLED_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* �� */

	OLED_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* �� */
	OLED_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* �� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    �Σ�
*			_usX,_usY  ��Բ�ĵ�����
*			_usRadius  ��Բ�İ뾶
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* ��ǰ X ֵ */
	uint32_t  CurY;		/* ��ǰ Y ֵ */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		OLED_PutPixel(_usX + CurX, _usY + CurY, _ucColor);
		OLED_PutPixel(_usX + CurX, _usY - CurY, _ucColor);
		OLED_PutPixel(_usX - CurX, _usY + CurY, _ucColor);
		OLED_PutPixel(_usX - CurX, _usY - CurY, _ucColor);
		OLED_PutPixel(_usX + CurY, _usY + CurX, _ucColor);
		OLED_PutPixel(_usX + CurY, _usY - CurX, _ucColor);
		OLED_PutPixel(_usX - CurY, _usY + CurX, _ucColor);
		OLED_PutPixel(_usX - CurY, _usY - CurX, _ucColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
*	��    �Σ�
*			_usX, _usY : ͼƬ������
*			_usHeight  ��ͼƬ�߶�
*			_usWidth   ��ͼƬ���
*			_ptr       ����ɫͼƬ����ָ�룬ÿ������ռ��1���ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr)
{
	uint16_t x, y;

	for (x = 0; x < _usWidth; x++)
	{
		for (y = 0; y < _usHeight; y++)
		{
			OLED_PutPixel(_usX + x, _usY + y, *_ptr);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ConfigGPIO
*	����˵��: ����OLED���ƿ��ߣ�����Ϊ8λ80XX���߿���ģʽ��SPIģʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_ConfigGPIO(void)
{


		GPIO_InitTypeDef GPIO_InitStructure;

	/* ���߷�ʽ��
	D1/SDIN ----------- TP_MOSI  (PB15)
	     CS ----------- TP_NCS   PB12
	 D0/SCK ----------- TP_SCK   (PB13)
	 */

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);


	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		SSD_CS_1();
		GPIO_Init(GPIOB, &GPIO_InitStructure);



	

}

/*
*********************************************************************************************************
*	�� �� ��: OLED_WriteCmd
*	����˵��: ��SSD1306����һ�ֽ�����
*	��    �Σ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_WriteCmd(uint8_t _ucCmd)
{

	uint8_t i;

	SSD_CS_0();

	SSD_SCK_0();
	SSD_SDIN_0();	/* 0 ��ʾ���洫�͵������� 1��ʾ���洫�͵����� */
	SSD_SCK_1();

	for (i = 0; i < 8; i++)
	{
		if (_ucCmd & 0x80)
		{
			SSD_SDIN_1();
		}
		else
		{
			SSD_SDIN_0();
		}
		SSD_SCK_0();
		_ucCmd <<= 1;
		SSD_SCK_1();
	}

	SSD_CS_1();



}

/*
*********************************************************************************************************
*	�� �� ��: OLED_WriteData
*	����˵��: ��SSD1306����һ�ֽ�����
*	��    �Σ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_WriteData(uint8_t _ucData)
{

	uint8_t i;

	SSD_CS_0();

	SSD_SCK_0();
	SSD_SDIN_1();	/* 0 ��ʾ���洫�͵������� 1��ʾ���洫�͵����� */
	SSD_SCK_1();

	for (i = 0; i < 8; i++)
	{
		if (_ucData & 0x80)
		{
			SSD_SDIN_1();
		}
		else
		{
			SSD_SDIN_0();
		}
		SSD_SCK_0();
		_ucData <<= 1;
		SSD_SCK_1();
	}

	SSD_CS_1();

}

void OLED_Init(void)
		{
			OLED_InitHard();	 /* ��ʼ��OLEDӲ�� */
			OLED_ClrScr(0x00);	 /* ������0x00��ʾ�ڵף� 0xFF ��ʾ�׵� */

			/* ����������� */

			tFont16.usFontCode = FC_ST_16;	/* ������� 16���� */
			tFont16.usTextColor = 1;		/* ������ɫ 0 �� 1 */
			tFont16.usBackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
			tFont16.usSpace = 0;			/* ���ּ�࣬��λ = ���� */

			tFont12.usFontCode = FC_ST_12;	/* ������� 12���� */
			tFont12.usTextColor = 1;		/* ������ɫ 0 �� 1 */
			tFont12.usBackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
			tFont12.usSpace = 1;			/* ���ּ�࣬��λ = ���� */

		}
			uint32_t OLED_shuji(uint32_t x,uint32_t y,uint32_t shuji,uint32_t stlye)
	{
		
		char chuandi[4]={0};
		chuandi[0]=0x30+shuji/1000;
		chuandi[1]=0x30+(shuji%1000)/100;
		chuandi[2]=0x30+(shuji%100)/10;
		chuandi[3]=0x30+shuji%10;
		OLED_DispStr(x,y,chuandi, &tFont12);
		return shuji;
		
		}

