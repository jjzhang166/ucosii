/****************************************Copyright (c)*********************************************
**                                     ��������ʢ�����Ƽ����޹�˾
**									       ��   ��   ��
**--------------�ļ���Ϣ---------------------------------------------------------------------------
**��   ��   ��: FRAM.C
**��   ��   ��: ��Сӱ
**�� �� �� �� ��2009��03��06��
**��        ��: FM25H**��������������
**--------------��ǰ�汾�޶�-----------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**************************************************************************************************/
#define IN_FRAM_GLOBALS
#include "fram.h"
#include "FM1702.h"

/**************************************************************************************************
**	��������:	FramState()
**	��	��	:	��ѯFRAM��״̬
**	��ڲ���:	��
**	���ڲ���:	��
**  ����ֵ  ��  statetemp		:	�ڼ��״̬
**	ȫ�ֱ���:	��
**	����ģ��:	FMSendByte��FMReadByte
**	��	��	:	��Сӱ
**	��	��	:	2009��03��06��
**************************************************************************************************/
uint8_t FramState(void)
{	
	uint8_t statetemp;
	CLR_SSPCSFR() ;
	ddelay(5) ;
	FMSendByte(COMD_RD_STATUS);				//������״̬
	statetemp	= FMReadByte();
	SET_SSPCSFR() ;
	ddelay(5) ;
	return(statetemp);
}
/**************************************************************************************************
**	��������:	FramWrite()
**	��	��	:	��FRAMд����
**	��ڲ���:	*dataptr	:	ָ����Ҫ���͵����ݵ�ָ��
**				addr		:	FRAM����ʼ�洢��ַ
**				length		:	���ݳ���
**	���ڲ���:	��
**  ����ֵ  ��  ��
**	ȫ�ֱ���:	��
**	����ģ��:	FMSendByte��FramState
**	��	��	:	��Сӱ
**	��	��	:	2009��03��06��
**************************************************************************************************/
void FRAMWrite(uint8_t *dataptr,uint16_t addr,uint16_t length)
{	
	uint16_t i;
	uint8_t	err;
	
//	OSSemPend(FRAMSem,0,&err);
	
	err	= FramState();
	
	if((err & 0x71) == 0)
	{
		CLR_SSPCSFR() ;
		ddelay(5) ;
		FMSendByte(COMD_WR_ENABLE);
		SET_SSPCSFR();
		ddelay(5) ;
		
		while((FramState() & 0x02 )== 0)
        {
            OSTimeDly(OS_TICKS_PER_SEC/10);
        }

		CLR_SSPCSFR() ;
		ddelay(5) ;
		FMSendByte(COMD_WR_MEMORY);
		FMSendByte((addr >> 8) & 0xff);
		FMSendByte(addr & 0xff);
		for (i = 0; i < length; i++)
		{	
			FMSendByte(*dataptr);
			dataptr	+= 1;
		}
		SET_SSPCSFR();
		ddelay(5) ;
	}
}
/**************************************************************************************************
**	��������:	FRAMRead()
**	��	��	:	��FRAM
**	��ڲ���:	*dataptr	:	ָ����Ҫ��ŵ����ݵ�ָ��
**				addr		:	FRAM����ʼ�洢��ַ
**				length		:	���ݳ���
**	���ڲ���:	��
**  ����ֵ  ��  ��
**	ȫ�ֱ���:	��
**	����ģ��:	FMSendByte��FMReadByte��FramState
**	��	��	:	��Сӱ
**	��	��	:	2009��03��06��
**************************************************************************************************/
void FRAMRead(uint8_t *dataptr,uint16_t addr,uint16_t length)
{	
	uint16_t i;
	uint8_t	err;
	
	CLR_SSPCSFR();
	ddelay(5) ;
	FMSendByte(COMD_RD_MEMORY);
	FMSendByte((addr >> 8) & 0xff);
	FMSendByte(addr & 0xff);	
	for (i = 0; i < length; i++)
	{
		*dataptr	= FMReadByte();
		dataptr++;
	}
	SET_SSPCSFR();
	ddelay(5) ;

}
/**************************************************************************************************
**	��������:	FRAMCheck()
**	��	��	:	FRAM��飬�����ϵ���FRAM����
**	��ڲ���:	�޴���ַ
**	���ڲ���:	��
**  ����ֵ  ��  =0 �ɹ�	= 1 ʧ��
**	ȫ�ֱ���:	��
**	����ģ��:	
**	��	��	:	��Сӱ
**	��	��	:	2009��03��06��
**************************************************************************************************/
uint8_t	FRAMCheck(void)
{	
	uint8_t	i;
	uint8_t	nn;
	uint8_t	err;
	
	err	= 1;

//	OSSemPend(FRAMSem,0,&err);
//	CLR_SSPCSFR() ;
//	ddelay(5) ;
	for(i = 0;i < 50; i++)
	{
		nn	= FramState();
		
		if((nn & 0x71) == 0)
		{
			err	= 0;
			break;
		}
	}
//	SET_SSPCSFR();
//	ddelay(5) ;
//	OSSemPost(FRAMSem);

	return err;
}

/*************************************************************************************************
**                            End Of File
**************************************************************************************************/
