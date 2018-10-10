/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395.C���ݽṹ����
*                      
**********************************************************************************/

#ifndef __CH395_H__
#define __CH395_H__

#include "stdint.h"

/* TCPģʽ��0Ϊ�ͻ��ˣ�1Ϊ������ */
#define TCP_CLIENT_MODE                                      0
#define TCP_SERVER_MODE                                      1

struct _CH395_SYS
{
    uint8_t   IPAddr[4];                                           /* CH395IP��ַ 32bit*/
    uint8_t   GWIPAddr[4];                                         /* CH395���ص�ַ 32bit*/
    uint8_t   MASKAddr[4];                                         /* CH395�������� 32bit*/
    uint8_t   MacAddr[6];                                          /* CH395MAC��ַ 48bit*/
    uint8_t   PHYStat;                                             /* CH395 PHY״̬�� 8bit*/
    uint8_t   MackFilt;                                            /* CH395 MAC���ˣ�Ĭ��Ϊ���չ㲥�����ձ���MAC 8bit*/
    uint32_t  RetranCount;                                         /* ���Դ��� Ĭ��Ϊ10��*/
    uint32_t  RetranPeriod;                                        /* ��������,��λMS,Ĭ��200MS */
    uint8_t   IntfMode;                                            /* �ӿ�ģʽ */
    uint8_t   UnreachIPAddr[4];                                    /* ���ɵ���IP */
    uint16_t  UnreachPort;                                         /* ���ɵ���˿� */
};

struct _SOCK_INF
{
    uint8_t  IPAddr[4];                                           /* socketĿ��IP��ַ 32bit*/
    uint8_t  MacAddr[6];                                          /* socketĿ���ַ 48bit*/
    uint8_t  ProtoType;                                           /* Э������ */
    uint8_t  ScokStatus;                                          /* socket״̬���ο�scoket״̬���� */
    uint8_t  TcpMode;                                             /* TCPģʽ */
    uint32_t IPRAWProtoType;                                      /* IPRAW Э������ */
    uint16_t DesPort;                                             /* Ŀ�Ķ˿� */
    uint16_t SourPort;                                            /* Ŀ�Ķ˿� */
    uint16_t SendLen;                                             /* �������ݳ��� */
    uint16_t RemLen;                                              /* ʣ�೤�� */
    uint8_t  *pSend;                                              /* ����ָ�� */                                               
}; 

extern void CH395_PRO(void);

#endif


/************************** endfile **************************************/