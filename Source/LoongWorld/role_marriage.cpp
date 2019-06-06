//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_marriage.cpp
// author: songg
// actor:
// data: 2009-09-10
// last:
// brief: ��Ҽ���ͻ���Ĵ���
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "role.h"
#include "map.h"
#include "marriage.h"
#include "role_mgr.h"
#include "../WorldDefine/msg_marriage.h"

//------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------
DWORD Role::ProcGetMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID, DWORD dwNpcID)
{
	// �Ƿ��Լ����Լ����
	if(GetID() == dwTgtRoleID)
	{
		IMSG(_T("Player<id: %u> is GetMarriage with himself!!!!!\r\n"), dwTgtRoleID);
		return GT_INVALID;
	}
	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		IMSG(_T("Cant't find the map when tackling the GetMarriageReq message\r\n"));
		return GT_INVALID;
	}
	// �������߼��
	pTarget = pMap->FindRole(dwTgtRoleID);
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Marriage_NotInSame_Map;
	}
	
	//��һ�½��check�ű�
	DWORD dwErrorCode = g_ScriptMgr.GetRoleScript()->OnGetMarriageCheck(this, this->GetID(), dwTgtRoleID);
	if(dwErrorCode != E_Marriage_Accept)
	{
		return dwErrorCode;
	}

	//���Դ��Һ�Ŀ����Ҿ������ϵľ����Ƿ�����������
	Unit *pYueLao = pMap->FindUnit(dwNpcID);
	if(!P_VALID(pYueLao))
	{
		IMSG(_T("Cant't find the the npc yuelao in this map\r\n"));
		return GT_INVALID;
	}
	//���������Ҿ������� 10��������
	if(!IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Marriage_OutOfRange;
	}
	//������������ҲҪ�������� 10��������
	if(!pTarget->IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Marriage_OutOfRange;
	}

	return E_Marriage_Accept;
}
//------------------------------------------------------------------------
// ��Ҷ�������ķ���
//------------------------------------------------------------------------
DWORD Role::ProcGetMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode)
{
	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		IMSG(_T("Cant't find the map when tackling the GetMarriageReqRes message.\r\n"));
		return GT_INVALID;
	}

	// ��������߼��
	pApplicant = pMap->FindRole(dwApplicantRoleID);
	if(!P_VALID(pApplicant))
	{
		// ���������ߣ����߸�����ͼ
		return E_Marriage_NotInSame_Map;
	}
	//��һ�½��check�ű�
	DWORD dwScriptErrCode = g_ScriptMgr.GetRoleScript()->OnGetMarriageCheck(this, pApplicant->GetID(), this->GetID());
	if(dwScriptErrCode != E_Marriage_Accept)
	{
		return dwScriptErrCode;
	}

	return dwErrorCode;
}
//------------------------------------------------------------------------
// �������
//------------------------------------------------------------------------
DWORD Role::ProcBreakMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID)
{
	// �Ƿ��Լ����Լ����
	if(GetID() == dwTgtRoleID)
	{
		IMSG(_T("Player<id: %u> is BreakMarriage with himself!!!!!\r\n"), dwTgtRoleID);
		return GT_INVALID;
	}
	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		IMSG(_T("Cant't find the map when tackling the BreakMarriageReq message\r\n"));
		return GT_INVALID;
	}
	// �������߼��
	pTarget = pMap->FindRole(dwTgtRoleID);
	if(!P_VALID(pTarget))
	{
		// �������߲�����,���������߲���ͬһ�ŵ�ͼ��
		return E_Marriage_NotInSame_Map;
	}

	//��һ�����check�ű�
	DWORD dwErrCode = g_ScriptMgr.GetRoleScript()->OnBreakMarriageCheck(this, this->GetID(), dwTgtRoleID);
	if(dwErrCode != E_Marriage_Accept)
	{
		return dwErrCode;
	}
	
	return E_Marriage_Accept;
}
//------------------------------------------------------------------------
// ��Ҷ��������ķ���
//------------------------------------------------------------------------
DWORD Role::ProcBreakMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode)
{
	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		IMSG(_T("Cant't find the map when tackling the BreakMarriageReqRes message.\r\n"));
		return GT_INVALID;
	}
	//��������߼��
	pApplicant = pMap->FindRole(dwApplicantRoleID);
	if(!P_VALID(pApplicant))
	{
		// ���������ߣ����߸�����ͼ
		return E_Marriage_NotInSame_Map;
	}
	// �Ƿ��Լ����
	if(pApplicant->GetID() == this->GetID())
	{
		IMSG(_T("Player<id: %u> is BreakMarriage with himself!!!!!\r\n"), dwApplicantRoleID);
		return GT_INVALID;
	}
	//��һ�����check�ű�
	DWORD dwScriptErrCode = g_ScriptMgr.GetRoleScript()->OnBreakMarriageCheck(this, pApplicant->GetID(), this->GetID());
	if(dwScriptErrCode != E_Marriage_Accept)
	{
		return dwScriptErrCode;
	}
	return dwErrorCode;
}
//------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------
DWORD Role::ProcCelebrateWeddingReq(OUT Role* &pTarget,OUT Role* &pOfficiatorRole,DWORD dwOfficiatorNameCrc,DWORD dwNpcID)
{
	pTarget = NULL;
	pOfficiatorRole = NULL;

	// ����Ƿ��л����ڽ���
	if(g_MarriageMgr.IsHavingWedding())
	{
		return E_Wedding_HavingWedding;
	}

	// ���������Ƿ��ѻ�
	if(IsRoleMarried() == FALSE)
	{
		return E_Wedding_NotMerried;
	}
	// ���������Ƿ��Ѿ����й�����
	if(IsRoleWedded() == TRUE)
	{
		return E_Wedding_AlreadyWedded;
	}

	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		IMSG(_T("Cant't find the map when tackling the CelebrateWeddingReq message.\r\n"));
		return GT_INVALID;
	}
	//�Ұ���
	pTarget = pMap->FindRole(GetLoverID());
	if(!P_VALID(pTarget))
	{
		// �������ߣ����߸�����ͼ
		return E_Wedding_NotInSame_Map;
	}

	//�������������ǲ��ǿ�
	if(dwOfficiatorNameCrc == -1)
	{
		//����ϵͳ����ģʽ
		g_MarriageMgr.EnableSysOfficiator();
	}
	else
	{
		//��������
		pOfficiatorRole = pMap->FindRole(g_roleMgr.GetRoleIDByNameCrc(dwOfficiatorNameCrc));
		if(!P_VALID(pOfficiatorRole))
		{
			// ���������ߣ����߸�����ͼ
			return E_Wedding_NotInSame_Map;
		}
	}

	//�����3���˵�npc���ϵľ���
	Unit *pYueLao = pMap->FindUnit(dwNpcID);
	if(!P_VALID(pYueLao))
	{
		IMSG(_T("Cant't find the the npc yuelao in this map\r\n"));
		return GT_INVALID;
	}
	//���������Ҿ������� 10��������
	if(!IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Marriage_OutOfRange;
	}
	//��ż
	if(!pTarget->IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Marriage_OutOfRange;
	}
	//�����������Ч
	if(dwOfficiatorNameCrc != -1  && P_VALID(pOfficiatorRole))
	{
		//��������˾���
		if(!pOfficiatorRole->IsInDistance(*pYueLao, 10*TILE_SCALE))
		{
			return E_Marriage_OutOfRange;
		}
	}
	return E_Wedding_Success;
}
//------------------------------------------------------------------------
// �����˵ķ���
//------------------------------------------------------------------------
DWORD Role::ProcCelebrateWeddingReqRes(OUT Role* &pApplicant, DWORD dwErrorCode)
{
	DWORD dwApplicantID = g_MarriageMgr.GetApplicantRoleID();
	// ��ȡ��ͼ
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		IMSG(_T("Cant't find the map when tackling the CelebrateWeddingReqRes message.\r\n"));
		return GT_INVALID;
	}
	pApplicant = pMap->FindRole(dwApplicantID);
	if(!P_VALID(pApplicant))
	{
		return E_Wedding_NotInSame_Map;
	}

	if(dwErrorCode != E_Wedding_Success)
	{
		return dwErrorCode;
	}
	/*
	����˫�����������Ƿ����ߡ�
	����˫�����������Ƿ���NPC����10���ӷ�Χ֮�ڡ�
	��ǰ�Ƿ����л������ھ���
	�������ֽ��Ƿ��㹻XXX
	*/
	//ֻ������ߡ����롢��Ǯ�Ϳ�����
	DWORD dwTargetID = g_MarriageMgr.GetTargetRoleID();
	Role* pTarget = pMap->FindRole(dwTargetID);
	if(!P_VALID(pApplicant))
	{
		return E_Wedding_NotInSame_Map;
	}

/*
	//�����3���˵�npc���ϵľ���
	Unit *pYueLao = pMap->FindUnit(g_MarriageMgr.GetYueLaoNPCID());
	if(!P_VALID(pYueLao))
	{
		IMSG(_T("Cant't find the the npc yuelao in this map\r\n"));
		return GT_INVALID;
	}
	//�����˾������� 10��������
	if(!IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Wedding_OutOfRange;
	}
	//��������
	if(!pTarget->IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Wedding_OutOfRange;
	}
	//������
	if(!pApplicant->IsInDistance(*pYueLao, 10*TILE_SCALE))
	{
		return E_Wedding_OutOfRange;
	}

	//��������˽�Ǯ
	if(dwErrorCode == E_Wedding_Success)
	{

		if(pApplicant->GetCurMgr().GetBagSilver() < g_MarriageMgr.GetBreakMarriageDecreaseSilver())
		{
			return E_Marriage_NotEnough_Money;		
		}
	}
*/
	return E_Wedding_Success;
}