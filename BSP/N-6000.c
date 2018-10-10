#include "N_6000.h"
#include "main.h"
extern void SD_ProTest(u8* pBuffer,u8* Woffset_num,u8* Roffset_num);
void SD_RBuffer(u8* pBuffer,u16 NumByteToRead,u8* offset_num); 
void SD_WBuffer(u8* pBuffer,u16 NumByteToRead,u8* offset_num);
#if HW_N6000
const char N_6000Message[40][20]={"FIRE ALARM","PREALARM OFF","FIRE ALARM DELAY","FIRE ALARM DELAY OFF",//0-3
           "SUPERVISORY","SUPERVISORY OFF","FEEDBACK","FEEDBACK OFF","DISABLED",//4-8
           "ENABLED","ACTIVE","DEACTIVE","NORMAL","CRC FAIL","OFFLINE","TROUBLE","GROUND NE",//9-16
           "GROUND PE","OPEN NE","OPEN PE","SHORT CIRCUIT","OPEN CIRCUIT","LOW VOLT","HIGH VOLT",//17-23
           "ADDRESS CONFLICT","MODEL ERROR","LOW THRESHOLD","REMOTE TEST","EVT_PS_FAULT",//24-28
	         "EVT_PS_FAULT_OFF","EVT_BAT_FAULT","EVT_BAT_FAULT_OFF","BUS_FAULT","BUS_FAULT_OFF",//29-33
	         "TO MANUAL","TO AUTO","CONFIG","CONFIG","RESET","SYSTEM POWER ON"};//34-39

const char N_6000Device[37][8]={"���Ӹ���","������","�������","����","���¸���","�������","����",//0-6
															"�ֶ�����","ˮ��ָʾ","ѹ������","����","����˨","��ͨ̽��",//7-12
                              "����̽��","��ȼ����","���̷�","������","��ŷ�","���̷��","������",//13-19
                              "���ܱ�","���ݹ��","�յ�����","������","��ѹ�ͷ�","��ѹ�ͷ�","����",//20-26
                            	"����ָʾ","��Դ�ж�","Ӧ���㲥","Ӧ������","���ⱨ��","������ʾ","(����)̽","(����)��","(����)��",//27-31
                              "3.0.19-N"};
HWDeviceStatus N_6000DS;
															
uint8_t Status_Decode(uint8_t *tmpecR) 
{
	 uint8_t i,j;
    for(i=1;i<21;i++)
		{
				if(tmpecR[i]==0x20&&tmpecR[i-1]==0x20)
				{
					  N_6000DS.ucDev_Status=0x01;
						for(j=0;j<28;j++)
						{
								if(0 == strncmp((char*)tmpecR+1,N_6000Message[j], (uint16_t)i-2))
								{
										switch(j)
										{
											 case 0: N_6000DS.ucDev_Status|=0x0002; break;//��
											 case 1: N_6000DS.ucDev_Status&=0xfffd;break;//�޻�
											 case 2: N_6000DS.ucDev_Status|=0x0080;break;//�ӳ�
											 case 3: N_6000DS.ucDev_Status&=0xff7f;break;//���ӳ�
											 case 4: N_6000DS.ucDev_Status|=0x0010;break;//���
											 case 5: N_6000DS.ucDev_Status&=0xffef;break;//�����ʧ
											 case 6: N_6000DS.ucDev_Status|=0x0040;break;//����
											 case 7: N_6000DS.ucDev_Status&=0xffbf;break;//������ʧ
											 case 8: N_6000DS.ucDev_Status|=0x0008;break;//����
											 case 9: N_6000DS.ucDev_Status&=0xfff7;break;//������
											 case 10: N_6000DS.ucDev_Status|=0x0020;break;//����
											 case 11: N_6000DS.ucDev_Status&=0xffdf;break;//ֹͣ
											 case 12: N_6000DS.ucDev_Status&=0xfffb;break;//�޹���
											 default: N_6000DS.ucDev_Status|=0x0004;break;//����
										}	
									  return 1;		
										break;																		
								}				
						}									
				}	
		}			
		return 0;
}

uint8_t Sys_Status_Decode(uint8_t *tmpecR) 
{
	 uint8_t i,j;
    for(i=1;i<21;i++)
		{
				if(tmpecR[i]==0x20&&tmpecR[i-1]==0x20||tmpecR[20]!=0x20)
				{
				  	N_6000DS.ucDev_Status=0x01;
						for(j=0;j<40;j++)
						{
								if(0 == strncmp((char*)tmpecR+1,N_6000Message[j], (uint16_t)i-2))
								{
										switch(j)
										{
											 case 0: N_6000DS.ucDev_Status|=0x0002; break;//��
											 case 1: N_6000DS.ucDev_Status&=0xfffd;break;//�޻�
											 case 2: N_6000DS.ucDev_Status|=0x0080;break;//�ӳ�
											 case 3: N_6000DS.ucDev_Status&=0xff7f;break;//���ӳ�
											 case 4: N_6000DS.ucDev_Status|=0x0010;break;//���
											 case 5: N_6000DS.ucDev_Status&=0xffef;break;//�����ʧ
											 case 6: N_6000DS.ucDev_Status|=0x0040;break;//����
											 case 7: N_6000DS.ucDev_Status&=0xffbf;break;//������ʧ
											 case 8: N_6000DS.ucDev_Status|=0x0008;break;//����
											 case 9: N_6000DS.ucDev_Status&=0xfff7;break;//������
											 case 10: N_6000DS.ucDev_Status|=0x0020;break;//����
											 case 11: N_6000DS.ucDev_Status&=0xffdf;break;//ֹͣ
											 case 12: N_6000DS.ucDev_Status&=0xfffb;break;//�޹���
											 case 28: N_6000DS.ucDev_Status|=0x0100;break;//�������
											 case 29: N_6000DS.ucDev_Status&=0xfeff;break;//��������
											 case 30: N_6000DS.ucDev_Status|=0x0200;break;//�������
											 case 31: N_6000DS.ucDev_Status&=0xfdff;break;//��������
											 case 32: N_6000DS.ucDev_Status|=0x0400;break;//���߹���
											 case 33: N_6000DS.ucDev_Status&=0xfbff;break;//��������
											 case 34: N_6000DS.ucDev_Status|=0x0800;break;//�ֶ�״̬
											 case 35: N_6000DS.ucDev_Status&=0xf7ff;break;//�Զ�״̬
											 case 36: N_6000DS.ucDev_Status|=0x1000;break;//���øı�
											 case 37: N_6000DS.ucDev_Status&=0xefff;break;//�����øı�
											 case 38: N_6000DS.ucDev_Status|=0x2000;break;//��λ
											 case 39: N_6000DS.ucDev_Status&=0xdfff;break;//����
											 default: N_6000DS.ucDev_Status|=0x0004;break;//����
										}	
									  return 1;		
										break;																		
								}				
						}									
				}	
		}			
		return 0;
}

uint8_t Type_Decode(uint8_t *tmpecR)
{
	uint8_t i,j,tempnum=8;
	
	for(i=22;i<40;i++)
	{
			if(tmpecR[i]==0x20)
			{
					for(j=0;j<36;j++)
					{
							if(i-22<7)
							{
								 tempnum=i-22;
							}	
							else if(tmpecR[28]==0x28)
							{
									tempnum=6;
							}	
							else if(tmpecR[22]==0x28)
							{
							    tempnum=8;
							}	
							if(0 == strncmp((char*)tmpecR+22,N_6000Device[j], (uint16_t)tempnum))
							{
										switch(j)
									{
										 case 0: N_6000DS.ucDev_Type=41;break;//���Ӹ���41
										 case 1: N_6000DS.ucDev_Type=42;break;//������42
										 case 2: N_6000DS.ucDev_Type=40;break;//�������43
										 case 3: N_6000DS.ucDev_Type=30;break;//����  128 ����
										 case 4: N_6000DS.ucDev_Type=51;break;//���¸���51
										 case 5: N_6000DS.ucDev_Type=43;break;//�������61 �̸�
										 case 6: N_6000DS.ucDev_Type=30;break;//����  129 ����
										 case 7: N_6000DS.ucDev_Type=23;break;//�ֶ�����23
										 case 8: N_6000DS.ucDev_Type=96;break;//ˮ��ָʾ96
										 case 9: N_6000DS.ucDev_Type=99;break;//ѹ������99
										 case 10: N_6000DS.ucDev_Type=103;break;//����103
										 case 11: N_6000DS.ucDev_Type=24;break;//����˨24
										 case 12: N_6000DS.ucDev_Type=21;break;//��ͨ̽����21
										 case 13: N_6000DS.ucDev_Type=136;break;//����̽��25
										 case 14: N_6000DS.ucDev_Type=10;break;//��ȼ����13
										 case 15: N_6000DS.ucDev_Type=113;break;//���̷�"113
										 case 16: N_6000DS.ucDev_Type=102;break;//������102
										 case 17: N_6000DS.ucDev_Type=106;break;//��ŷ�106
										 case 18: N_6000DS.ucDev_Type=111;break;//���̷��"111
										 case 19: N_6000DS.ucDev_Type=91;break;//������"91
										 case 20: N_6000DS.ucDev_Type=95;break;//���ܱ�95
										 case 21: N_6000DS.ucDev_Type=130;break;//���ݹ��  130
										 //case 22: N_6000DS.ucDev_Type=104;break;//�յ�����104
										 case 23: N_6000DS.ucDev_Type=117;break;//������"117
										 case 24: N_6000DS.ucDev_Type=128;break;//��ѹ�ͷ�
										 case 25: N_6000DS.ucDev_Type=129;break;//��ѹ�ͷ�   
										 case 26: N_6000DS.ucDev_Type=121;break;//����121
										 case 27: N_6000DS.ucDev_Type=131;break;//����ָʾ  132
										 case 28: N_6000DS.ucDev_Type=132;break;//��Դ�ж�  133
										 case 29: N_6000DS.ucDev_Type=133; break;//Ӧ���㲥  134
										 case 30: N_6000DS.ucDev_Type=134;break;//Ӧ������"  135
										 case 31: N_6000DS.ucDev_Type=135;break;//���ⱨ��   136
										 case 32: N_6000DS.ucDev_Type=22;break;//������ʾ   22
										 case 33: N_6000DS.ucDev_Type=25;break;//����̽����
										 case 34: N_6000DS.ucDev_Type=85;break;//����ģ��  22
										 case 35: N_6000DS.ucDev_Type=86;break;//���ģ��  22
									}	
									return 1;		
									break;																		
							}				
					}				
			}
	 }		
  return 0;	
}

uint8_t Notice_Decode(uint8_t *tmpecR)
{
	 uint8_t i,j=0;

	 if(0==strncmp((char*)tmpecR+41,"N-6000", 6))
	 {
			return 1;	 
	 }
	 else	 
   {		 
			 for(i=41;i<61;i++)
			 {	
				  if(i>41&&tmpecR[i-1]==0x20&&tmpecR[i]==0x20)
					{	
						 N_6000DS.ucDev_Notice[j-1]=0;
						 j--;
						 break;
					}	
					else
					{	
						 N_6000DS.ucDev_Notice[j]=tmpecR[i];
						 j++;
					}	
			 }	
			 for(i=61;i<81;i++)
			 {	
				  if(i>61&&tmpecR[i-1]==0x20&&tmpecR[i]==0x20)
					{	
						 N_6000DS.ucDev_Notice[j-1]=0;
						 break;
					}	
					else
					{	
						 N_6000DS.ucDev_Notice[j]=tmpecR[i];
						 j++;
					}	
			 } 
		}		
    return 0;	 
}	

uint8_t TimeDay_Decode(uint8_t *tmpecR)
{
	 uint8_t i,j=0;
   N_6000DS.ucDev_TimeDay[0]=(tmpecR[130]-0x30)*10+tmpecR[131]-0x30;
	 N_6000DS.ucDev_TimeDay[1]=(tmpecR[127]-0x30)*10+tmpecR[128]-0x30;
	 N_6000DS.ucDev_TimeDay[2]=(tmpecR[124]-0x30)*10+tmpecR[125]-0x30;
	 N_6000DS.ucDev_TimeDay[3]=(tmpecR[142]-0x30)*10+tmpecR[143]-0x30;
	 N_6000DS.ucDev_TimeDay[4]=(tmpecR[139]-0x30)*10+tmpecR[140]-0x30;
	 N_6000DS.ucDev_TimeDay[5]=(tmpecR[136]-0x30)*10+tmpecR[137]-0x30;
}

uint8_t Address_Decode(uint8_t *tmpecR)
{
	 uint8_t i,j=0;
	 uint32_t taddress;
   taddress=(tmpecR[157]-0x30)*10000+(tmpecR[158]-0x30)*1000+\
	                        (tmpecR[160]-0x30)*100+(tmpecR[161]-0x30)*10+(tmpecR[162]-0x30);
   N_6000DS.ucDev_Address[0]=taddress;
	 N_6000DS.ucDev_Address[1]=taddress>>8;
	 N_6000DS.ucDev_Address[2]=taddress>>16;
	 N_6000DS.ucDev_Address[3]=taddress>>24;
}

uint8_t SYS_Address_Decode(uint8_t *tmpecR)
{
   return (tmpecR[153]-0x30)*100+(tmpecR[154]-0x30)*10+(tmpecR[155]-0x30);	
}

void N_6000Pro_Decode(uint16_t *N_rxcnt,uint8_t *RxBuffer)
{
	  if(*N_rxcnt)
		{
				if(USART_ReceiveData(RTU_COM_2)==0x02)
				{
						RxBuffer[0]=0x02;
						*N_rxcnt=1;
				}		
				else
				{	
						RxBuffer[(*N_rxcnt)++]=USART_ReceiveData(RTU_COM_2);
						if(*N_rxcnt==166)
						{
							  USART_Cmd(USART2, DISABLE);
						  	N_6000DS.ucMSG_Type=Notice_Decode(RxBuffer);	
							  if(!N_6000DS.ucMSG_Type)
								{	
										N_6000DS.ucSend_FLAG=Status_Decode(RxBuffer);
										N_6000DS.ucSend_FLAG=Type_Decode(RxBuffer);
								}
                else
                {
								    N_6000DS.ucSend_FLAG=Sys_Status_Decode(RxBuffer);
								}										
								N_6000DS.ucSYS_Address=SYS_Address_Decode(RxBuffer);	
								TimeDay_Decode(RxBuffer);		
								Address_Decode(RxBuffer);									
								*N_rxcnt=0;
							  if(!N_6000DS.ucSend_FLAG)
								{
										USART_Cmd(USART2, ENABLE);
								}	
							  if( GPIO_ReadOutputDataBit(LED2_GPIO_PORT,LED2_PIN))
								{
										STM_RTU_LEDOff(LED2);
								}
								else
								{
										STM_RTU_LEDOn(LED2);
										 
								}
						}
				}		
		}		
		else if(USART_ReceiveData(RTU_COM_2)==0x02)
		{
				RxBuffer[0]=0x02;
				(*N_rxcnt)++;
		}				
}	
#endif
		 
#if HW_N3030		

extern int16_t ff_convert (	/* Converted code, 0 means conversion error */
	unsigned short	src,	/* Character code to be converted */
	unsigned int	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
const char N_3030Message[6][12]=\
      {{0x70,0x6b,0x70,0x7e,0x62,0xa5,0x8b,0x66,0x00,0x00,0x00,0x00},//���ֱ���
			 {0x70,0x6b,0x8b,0x66,0x5d,0xf2,0x6d,0x88,0x96,0x64,0x00,0x00},//��������
			 {0x70,0x6b,0x8b,0x66,0x65,0x45,0x96,0x9c,0x5d,0xf2,0x6d,0x88},//�𾯹���������
			 {0x6d,0x88,0x96,0x32,0x65,0x45,0x96,0x9c,0x00,0x00,0x00,0x00},//��������
			 {0x78,0x6e,0x8b,0xa4,0x70,0x6b,0x70,0x7e,0x65,0x45,0x96,0x9c},//ȷ�ϻ��ֱ���
		   {0x78,0x6e,0x8b,0xa4,0x70,0x6b,0x70,0x7e,0x62,0xa5,0x8b,0x66}//ȷ�ϻ��ֹ���
       

};//24-28

const char N_3030Device[9][4]=\
            {{0x6e,0x29,0x61,0x1f},//�¸�
						 {0x70,0xdf,0x61,0x1f},//�̸�
						 {0x98,0xce,0x7b,0xa1},//��� �̸�
						 {0x95,0x01,0x5b,0x9a},//���� �̸�
						 {0x76,0xd1,0x7b,0xa1},//��� �̸�
						 {0x76,0xd1,0x89,0xc6},//���� �̸�
						 {0x81,0xea,0x90,0x02},//���� �̸�
						 {0x59,0x0d,0x54,0x08},//����
						 {0x61,0x1f,0x6e,0x29}//����
};//27-31
						
typedef struct
{
    uint8_t           ubFlag;
    uint16_t          usNTimeOutMS;
    uint16_t          usNTimeLeft;
} xTimerInternalHandle;

#define MBM_DEFAULT_RESPONSE_TIMEOUT            ( 150 )
#define DRufLarryNum  20
uint8_t cnt=0;
xTimerInternalHandle N_6000TimerHdls;
HWDeviceStatus N_3030DS;
N_3030DecodeMEM N_3030DM;
uint8_t N_RxBuffer[DRufLarryNum][332];
extern uint8_t runningFlag;
extern uint8_t HW_N3030_SendFlag;
extern uint8_t TxBuffer[];
extern uint16_t NbrOfDataToTransfer;
extern void makeN_CRT_DataPack(char* databuf, HWDeviceStatus N_CRTDS);
extern BOOL mqtt_pub(char* payload,uint16_t TxLen);
extern void TIM2_ClearTim(void);
uint8_t TimeDay_Decode(uint8_t *tmpecR)
{
	 uint8_t i,j=0;
   N_3030DS.ucDev_TimeDay[0]=(tmpecR[257]-0x30)*10+tmpecR[259]-0x30;//��
	 N_3030DS.ucDev_TimeDay[1]=(tmpecR[251]-0x30)*10+tmpecR[253]-0x30;//��
	 if(tmpecR[261]=='P')
	 { 
			N_3030DS.ucDev_TimeDay[2]=(tmpecR[245]-0x30)*10+tmpecR[247]-0x30;//ʱ
		  N_3030DS.ucDev_TimeDay[2]+=12;
	 }
	 else
	 {
	    N_3030DS.ucDev_TimeDay[2]=(tmpecR[245]-0x30)*10+tmpecR[247]-0x30;
	 }	 
	 N_3030DS.ucDev_TimeDay[3]=(tmpecR[281]-0x30)*10+tmpecR[283]-0x30;//��
	 switch(tmpecR[273])
	 {
		  case 'J': 
				        if(tmpecR[275]=='A')  N_3030DS.ucDev_TimeDay[4]=1;
                else
                {									
									  if(tmpecR[277]=='N') N_3030DS.ucDev_TimeDay[4]=6;	
                    else  N_3030DS.ucDev_TimeDay[4]=7;									
                }									
                break;			
			case 'F': N_3030DS.ucDev_TimeDay[4]=2;break;	
      case 'M': N_3030DS.ucDev_TimeDay[4]=3;break;	
      case 'A':	if(tmpecR[275]=='P')  N_3030DS.ucDev_TimeDay[4]=4;
                else
                {
									  N_3030DS.ucDev_TimeDay[4]=8;
								}	  
				        break;		
      case 'S': N_3030DS.ucDev_TimeDay[4]=9;break;	
      case 'O': N_3030DS.ucDev_TimeDay[4]=10;break;	
      case 'N':	N_3030DS.ucDev_TimeDay[4]=11;break;				
      case 'D':	N_3030DS.ucDev_TimeDay[4]=12;break;			
	 }	 
	 N_3030DS.ucDev_TimeDay[5]=(tmpecR[293]-0x30)*10+tmpecR[295]-0x30;//��
}

uint8_t Address_Decode(uint8_t *tmpecR)
{
	 uint8_t i,j=0;
	 uint32_t taddress;
	 taddress=(tmpecR[309]-0x30)*10000+(tmpecR[311]-0x30)*1000+\
	                        (tmpecR[315]-0x30)*100+(tmpecR[317]-0x30)*10+(tmpecR[319]-0x30);
   N_3030DS.ucDev_Address[0]=taddress;
	 N_3030DS.ucDev_Address[1]=taddress>>8;
	 N_3030DS.ucDev_Address[2]=taddress>>16;
	 N_3030DS.ucDev_Address[3]=taddress>>24;
}

uint8_t Notice_Decode(uint8_t *tmpecR)
{
	 uint8_t i,j=0;
	 uint16_t temp16;
	 uint8_t *temp[2];
	 for(i=80;i<111;i+=2)
   {	
		  temp16=(tmpecR[i]<<8)+tmpecR[i+1];
		  temp16=ff_convert(temp16,0); 
			N_3030DS.ucDev_Notice[j++]=temp16>>8;
		  N_3030DS.ucDev_Notice[j++]=temp16;
	 }		  
}	

uint8_t Status_Decode(uint8_t *tmpecR) 
{
	 uint8_t i,j;
	 uint8_t tempnum=12;
 
		for(j=0;j<6;j++)
		{
				if(0 == strncmp((char*)tmpecR,N_3030Message[j], 8))
				{
					  N_3030DS.ucDev_Status=0x01;
						switch(j)
						{
							 case 0: N_3030DS.ucDev_Status|=0x0002; break;//��
							 case 1: 
							 case 2:
											 if(tmpecR[10]==0x6d)
													 N_3030DS.ucDev_Status&=0xfffb;										 
											 else
													 N_3030DS.ucDev_Status&=0xfffd;
											 break;//�޻�		
							 case 3: N_3030DS.ucDev_Status|=0x0004;break;//����
							 case 4: 
											 if(tmpecR[8]==0x65)
													 N_3030DS.ucDev_Status|=0x0002;//�� 
											 else														 
													N_3030DS.ucDev_Status|=0x0004;
											 break;//����				 
						}	 
						return 1;																			
				}				
		}									
		return 0;
}

uint8_t Type_Decode(uint8_t *tmpecR)
{
	uint16_t i;
	uint8_t j,tempnum=4;
	
	if(tmpecR[313]==0x4D)
	{
	    N_3030DS.ucDev_Type=84;//ģ��
		  return 1;	
	}
  else
  {
		 for(j=0;j<9;j++)
		{
			if(0 == strncmp((char*)tmpecR+218,N_3030Device[j], (uint16_t)tempnum))
			{
					switch(j)
					{
						 case 0: 
						 case 8: 	 
										 N_3030DS.ucDev_Type=30;break;//���Ӹ���41
						 case 7: N_3030DS.ucDev_Type=50;break;//������42
						 default: N_3030DS.ucDev_Type=40;break;//�������43
					}	
					return 1;																				
			}			
		}			
	}				
  
  return 0;	
}

void Overtime_Init(void)
{	
		N_6000TimerHdls.ubFlag=0;
		N_6000TimerHdls.usNTimeOutMS = MBM_DEFAULT_RESPONSE_TIMEOUT;
		N_6000TimerHdls.usNTimeLeft = MBM_DEFAULT_RESPONSE_TIMEOUT;
}

void Overtime_Pro(uint16_t *N_rxcnt)
{	
	 if(N_6000TimerHdls.ubFlag) 
	 {
		  N_6000TimerHdls.usNTimeLeft--;
	    if(!N_6000TimerHdls.usNTimeLeft)
			{
				   N_6000TimerHdls.usNTimeLeft=MBM_DEFAULT_RESPONSE_TIMEOUT;
				   N_6000TimerHdls.ubFlag=0;
           *N_rxcnt=0;
			}	
	 }
}

void Decode_CTR(void)
{
	  
	 uint8_t i;
	 static  uint8_t Status3G=0; 
	  char tempdata[100];
    //USART_Cmd(USART2, DISABLE);
	  if(N_3030DM.ucDEcodeSTA)
		{	
				for(i=0;i<N_3030DM.ucDecodeNum;i++)
				{
						if(N_RxBuffer[i][0])
						{	
								N_3030DM.ucDEcodeSTA=0;
								N_3030DS.ucSend_FLAG=Status_Decode(N_RxBuffer[i]);
								N_3030DS.ucSend_FLAG=Type_Decode(N_RxBuffer[i]);
								//Notice_Decode(N_RxBuffer[i]);		
								TimeDay_Decode(N_RxBuffer[i]);		
								Address_Decode(N_RxBuffer[i]);
								if(N_3030DS.ucSend_FLAG)
								{		
										makeN_CRT_DataPack(tempdata,N_3030DS);						
								  	Status3G=mqtt_pub(tempdata,78);	
										if(Status3G==1)
										{										
												
												bsp_DelayMS(50);
										}
										else
										{
												bsp_DelayMS(10000);
										}	
										
								}
						}		
				}		
				N_6000TimerHdls.ubFlag=0;
				N_3030DM.ucDecodeNum=0;
				USART_Cmd(USART2, ENABLE);						
		}		
		
}	

uint8_t N_3030Pro(void)
{ 
		TIM2_ClearTim();
		N_6000TimerHdls.ubFlag=1;
		N_6000TimerHdls.usNTimeLeft=MBM_DEFAULT_RESPONSE_TIMEOUT;
		N_3030DM.ucDEcodeTIM=1;						
}	

void N_3030ProCtr(void)
{	
		if(HW_N3030_SendFlag)
		{
			  if(!N_6000TimerHdls.ubFlag)
			  {	 
						if(runningFlag!=1)
						{	
								runningFlag =1;
								HW_N3030_SendFlag=0;
							  
								TxBuffer[0]=0x00;
								TxBuffer[1]=0x1B;
								TxBuffer[2]=0x00;
								TxBuffer[3]=0x06;
								NbrOfDataToTransfer = 4;
							  
							  if( GPIO_ReadOutputDataBit(LED2_GPIO_PORT,LED2_PIN))
								{
										STM_RTU_LEDOff(LED2);
								}
								else
								{
										STM_RTU_LEDOn(LED2);
										 
								}
								USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
								 USART_Cmd(USART2, ENABLE);
				  	}	
				} 
		}	
}															
#endif															
