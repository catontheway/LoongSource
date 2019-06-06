//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_pet_exchange.h
// author:  hyu
// actor:   hyu
// data:    2009-08-11
// last:    2009-08-11
// brief: ��Ҽ���ｻ����Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum
{
    E_Pet_Exchange_Accept				= E_Success,	// �Է�ͬ�⽻��
    E_Pet_Exchange_Refuse				= 1,	// �Է��ܾ�		
    E_Pet_Exchange_NotEnough_Space	    = 2,	// �������ռ䲻��
    E_Pet_Exchange_RoleBusy				= 3,	// �����뷽æ	
    E_Pet_Exchange_WithMore				= 4,	// ���ܺͶ���ͬʱ����
    E_Pet_Exchange_OutOfRange			= 5,	// ���׾��뷶Χ֮��
    E_Pet_Exchange_NotInSame_Map		= 6,	// ����ͬһ����
    E_Pet_Exchange_PetCanNot_Exchange	= 7,	// ���ﲻ�ܽ���
    E_Pet_Exchange_PetCanNot_Find		= 8,	// û���ҵ�ָ���ĳ���
    E_Pet_Exchange_NotEnough_Money		= 9,	// ���׵Ľ�Ǯ����
    E_Pet_Exchange_ApplicantTgt_Change	= 10,	// ��������Ľ���Ŀ�귢���仯
    E_Pet_Exchange_NotAnswer			= 11,	// �Է�û����Ӧ
    E_Pet_Exchange_SelfBusy				= 12,	// ����Լ�æ

	E_Pet_Exchange_PetHasEquip			= 20,	// ������װ��
	E_Pet_Exchange_RoleLvlNotEnough		= 21,	// Ŀ����ҵȼ�����
	E_Pet_Exchange_PetLocked			= 22,	// ���ﱻ����
	E_Pet_Exchange_PetBinded			= 23,	// ���ﱻ��
	E_Pet_Exchange_PetStateNotAllow		= 24,	// ����״̬������

    E_Pet_Exchange_End
};




//----------------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------------

// ������󽻻�����
CMD_START(NC_PetExchangeReq)
    DWORD	dwDstID;	// ��ϢĿ��ID
CMD_END;

// ������ת����Ŀ�����
CMD_START(NS_PetExchangeReq)
    DWORD	dwSrcID;	// ��Ϣ������ID
CMD_END;

// Ŀ����ҽ���������
CMD_START(NC_PetExchangeReqRes)
    DWORD	dwErrorCode;
    DWORD	dwDstID;
CMD_END;

// ������ת����˫����ҽ���������
CMD_START(NS_PetExchangeReqRes)
    DWORD	dwErrorCode;
    DWORD	dwSrcID;    // ������id
    DWORD	dwDstID;    // Ŀ��id
CMD_END;

//----------------------------------------------------------------------------
// ���׹���
//----------------------------------------------------------------------------


// ��ӽ��׵ĳ���
CMD_START(NC_PetExchangeAdd)
    DWORD	dwPetID;
CMD_END;

// ��������ɹ��򱾵����
CMD_START(NS_PetExchangeAdd_2Src)
    DWORD	dwErrorCode;
    DWORD	dwSrcID;        // ��ӷ�����id
    DWORD	dwDstID;
    DWORD	dwPetID;
CMD_END;

// ��ɹ���֪ͨĿ�������ӳ���
CMD_START(NS_PetExchangeAdd_2Dst)
    DWORD	dwSrcID;        // �Է�������id
    DWORD	dwPetID;
CMD_END;

// ȡ�����׵ĳ���
CMD_START(NC_PetExchangeDec)
    DWORD	dwSrcID;        // ��ӷ�����id
    DWORD	dwPetID;
CMD_END;

// ��������ɹ��򱾵ؼ���
CMD_START(NS_PetExchangeDec)
    DWORD	dwErrorCode;
    DWORD	dwSrcID;        // ���ٷ�����id
    DWORD	dwDstID;
    DWORD	dwPetID;
CMD_END;

// ��ɹ���֪ͨĿ�����ȡ������
CMD_START(NS_PetExchangeDec_2Dst)
    DWORD	dwSrcID;        // ���ٷ�����id
    DWORD	dwPetID;
CMD_END;

// ��Ǯ�����ı� 
CMD_START(NC_PetExchangeMoney)
    INT64	n64Money;
CMD_END

// ֪ͨ˫��
CMD_START(NS_PetExchangeMoney)
    DWORD	dwErrorCode;	
    INT64	n64Money;
    DWORD	dwSrcID;       // �ı��Ǯ������id
CMD_END

CMD_START(NC_PetExchangeLock)		// �������׳���
CMD_END;

// ֪ͨ˫��
CMD_START(NS_PetExchangeLock)
    DWORD	dwErrorCode;
    DWORD	dwSrcID;       // �������׷�����id
CMD_END;

// ȡ������
CMD_START(NC_PetExchangeCancel)
CMD_END;

// ֪ͨ˫��
CMD_START(NS_PetExchangeCancel)
    DWORD	dwSrcID;        // ȡ�����׷�����id
CMD_END;

// ���ȷ�Ͻ���
CMD_START(NC_PetExchangeVerify)	
CMD_END;

// �������
CMD_START(NS_PetExchangeFinish)
    DWORD	dwErrorCode;
    DWORD	dwFailedRoleID;		// ����ʧ����ID
CMD_END;

//----------------------------------------------------------------------------
#pragma pack(pop)