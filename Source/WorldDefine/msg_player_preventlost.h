#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"

CONST	UINT16		DOUBLE_EXP_QUEST_ID		=	10848;		// ˫����������ID
CONST	UINT16		FISHING_QUEST_ID		=	79;			// ��������ID

enum ELoongBenediction
{
	ELBD_Success			= 0,			// �ɹ�
	ELBD_Bag_NotEnoughSpace	= 1,			// �����Ѿ�����
	ELBD_Time_NotArrive		= 2,			// ʱ�仹δ��
	ELBD_No_LessingProto	= 3,			// û���ҵ��͸�ԭ��������Ϣ
	ELBD_Item_Create_Failed	= 4,			// ��Ʒ����ʧ��
	ELBD_No_Benediction		= 5,			// û����
};


//-----------------------------------------------------------------------------
// ��ֹ�����ʧ������������ʾ�������͸�
//-----------------------------------------------------------------------------

CMD_START(NC_ExitGamePrompt)
CMD_END

CMD_START(NS_ExitGamePrompt)
	BOOL		bDualExpQuestDo;				//�����Ƿ����,˫��ɱ�־�������� id = 10848
	BOOL		bFishQuestDo;					//�����Ƿ���ɣ��������� id = 79
	INT32		nExprence;						//����һСʱ��õľ���
CMD_END


//-----------------------------------------------------------------------------
// �����͸�
//-----------------------------------------------------------------------------
CMD_START(NC_GetLoongBenediction)			//��ȡ����
CMD_END

CMD_START(NS_GetLoongBenediction)
	DWORD		dwErrorCode;
	DWORD		nItemTypeID;				//��������Ʒ��ID
	INT			nItemCount;					//��Ʒ�����ĸ���
	INT32		nExprence;					//�����ľ����ID
CMD_END

CMD_START(NS_SetNextBenediction)
	BOOL			isEnd;					//�����͸��Ƿ�����ı�־
	DWORD			nextServerTime;			//��һ�������͸���ʱ�䣬ʣ������
	DWORD			nItemTypeID;			//��õ���Ʒ����
	INT				nItemCount;				//��Ʒ�����ĸ���
	INT32			n32Exprence;			//���齱��
CMD_END

CMD_START(NS_LoongBenedictionInit)			//���߿ͻ�����Ҫ��ʼ�����ģ��
CMD_END

#pragma pack(pop)