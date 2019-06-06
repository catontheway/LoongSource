//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_marriage.h
// author: 
// actor:
// data: 2009-09-11
// last:
// brief: ��һ���ϵͳ�����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// �㲥����
//----------------------------------------------------------------------------
enum EMBroadcastType
{
	//###���з�����###��Ů������ʽ��Ϊ���ޣ�ף���ǰ�ͷЯ�ϣ�����ͬ�ģ�
	E_Broadcast_GetMarriage					= 0,	//���

	//###���������ƣ���###��Ů�����ƣ��Ļ������ڿ�ʼ��
	E_Broadcast_CelebrateWedding			= 1,	//���л���


	E_Broadcast_Pledge						= 2,	//����
};

//----------------------------------------------------------------------------
// ������ö�� �������
//----------------------------------------------------------------------------
enum
{
	E_Marriage_Accept						= E_Success,	// �Է�ͬ��
	E_Marriage_Refuse						= 1,	// �Է��ܾ�		
	E_Marriage_SameSex						= 2,	// �Ա���ͬ
	E_Marriage_NotEnough_FriendShip			= 3,	// �ѺöȲ���
	E_Marriage_Applicant_AlreadyMarried		= 4,	// �������ѻ� ���ʱ�Ĵ���
	E_Marriage_BeApplicant_AlreadyMarried	= 5,	// ���������ѻ�
	E_Marriage_OutOfRange					= 6,	// ˫��Ӧ��NPC���ϸ���[10����֮��]
	E_Marriage_NotEnough_Money				= 7,	// �����߱���ӵ��66��
	E_Marriage_NotInSame_Map				= 8,	// ����ͬһ����
	E_Marriage_NotAnswer					= 9,	// �Է�û����Ӧ
	E_Marriage_LackItem						= 10,	// ȱ�ٱ�Ҫ��Ʒ
	E_Marriage_NotMerried					= 11,	// δ�� ���ʱ�Ĵ���
	E_Marriage_NotEnough_BagSpace			= 12,   // �����ռ䲻��


    E_Marriage_ErrEnd
};


//----------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------
enum EWeddingStep
{
	ES_Wedding_None							= 0,	//��
	ES_Wedding_Pledge						= 1,	//����
	ES_Wedding_Worship						= 2,	//�԰�
	ES_Wedding_Kiss							= 3,	//����
	ES_Wedding_Gift							= 4,	//�����
	ES_Wedding_AroundTown					= 5,	//�γ�
};

enum
{
	E_Wedding_Success					= E_Success,	// ͬ�� �ɹ�
	E_Wedding_Fail						= 1,			// ���� ʧ��
	E_Wedding_BagNotEnoughSpace			= 2,			// �����ռ䲻��
	E_Wedding_GiftUseUp					= 3,			// ���������
	E_Wedding_OutOfRange				= 4,			// ���޺�������Ӧ��NPC���ϸ���[10����֮��]
	E_Wedding_AlreadyWedded				= 5,			// �Ѿ����й�������
	E_Wedding_HavingWedding				= 6,			// �����ڽ��
	E_Wedding_NotMerried				= 7,			// ���������δ��
	E_Wedding_NotInSame_Map				= 8,			// ����ͬһ����
};

//----------------------------------------------------------------------------
// ������ϵ��ͬ���������,ע: ��żid��Ϊ�����һ������
//                          ��ʼ��ʱ��μ�
//                          tagNS_GetRoleInitState_Att
//                          �� tagNS_GetRemoteRoleState
//----------------------------------------------------------------------------
// Զ����һ�����ϵ�����仯ͬ�����Ź������
CMD_START(NS_RemoteChangeSpouse)
    DWORD   dwRoleID;       // Զ�����id
    DWORD   dwLoverID;      // ���˶�Ӧ��roleid û�����ΪGT_INVALID
    BOOL    bHaveWedding;   // �Ƿ���й�����
CMD_END

//----------------------------------------------------------------------------
// �������
//----------------------------------------------------------------------------

//�������
CMD_START(NC_CelebrateWeddingReq)	
	DWORD dwOfficiatorNameCrc; //����������crc
	DWORD dwNpcID;
CMD_END;

//������ת���� ������
CMD_START(NS_CelebrateWeddingReq)	
	DWORD dwOfficiatorNameCrc; //����������crc
CMD_END;

//�����˵ķ���
CMD_START(NC_CelebrateWeddingReqRes)	
	DWORD dwErrorCode; //
CMD_END;

//�������˵ķ��� ת����������
CMD_START(NS_CelebrateWeddingReqRes)	
	DWORD	dwErrorCode; //�Ի�������ķ���
CMD_END


//�������״̬�ı�����
CMD_START(NC_UpdateWeddingStepReq)		
EWeddingStep eWeddingStep;
CMD_END;

//������ͬ������״̬��
CMD_START(NS_UpdateWeddingStepReq)		
EWeddingStep eWeddingStep;
CMD_END;

//�������״̬�ı�����ķ���
CMD_START(NS_UpdateWeddingStepReqRes)		
DWORD	dwErrorCode;
CMD_END;

//���󷢺��
CMD_START(NC_GiveGiftReq)				
DWORD	dwDstID; //��ϢĿ��id
CMD_END;

//�Է�����˵ķ���
CMD_START(NS_GiveGiftReqRes)
DWORD	dwSrcID; //��Ϣ������ID
DWORD	dwErrorCode;
CMD_END;


//----------------------------------------------------------------------------
// �����Ϣ�ṹ
//----------------------------------------------------------------------------
CMD_START(NC_GetMarriageReq)		// ���������
DWORD	dwDstID;	// ��ϢĿ��ID
DWORD	dwNpcID;
CMD_END;

CMD_START(NS_GetMarriageReq)		// ������ת����Ŀ�����
DWORD	dwSrcID;	// ��Ϣ������ID
CMD_END;

CMD_START(NC_GetMarriageReqRes)	// Ŀ����Ҷ�������ķ���
DWORD	dwErrorCode;
DWORD	dwDstID;
CMD_END;

CMD_START(NS_GetMarriageReqRes)	// ������ת����Ŀ����ҽ��������
DWORD	dwErrorCode;
DWORD	dwSrcID;
DWORD	dwDstID;
CMD_END;

//----------------------------------------------------------------------------
// �����Ϣ�ṹ
//----------------------------------------------------------------------------
CMD_START(NC_BreakMarriageReq)		// ����������
DWORD	dwDstID;	// ��ϢĿ��ID
CMD_END;

CMD_START(NS_BreakMarriageReq)		// ������ת����Ŀ�����
DWORD	dwSrcID;	// ��Ϣ������ID
CMD_END;

CMD_START(NC_BreakMarriageReqRes)	// Ŀ����Ҷ��������ķ���
DWORD	dwErrorCode;
DWORD	dwDstID;
CMD_END;

CMD_START(NS_BreakMarriageReqRes)	// ������ת����Ŀ��������������
DWORD	dwErrorCode;
DWORD	dwSrcID;
DWORD	dwDstID;
CMD_END;

//----------------------------------------------------------------------------
#pragma pack(pop)