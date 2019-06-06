//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: NPCTeam_mgr.h
// author: wjiang
// actor:
// data: 2009-6-15
// last:
// brief: NPC���������
//-------------------------------------------------------------------------------------------------------
#pragma once

class NPCTeam;
class CVector2D;
struct tagNPCTeamOrder;

class NPCTeamMgr
{
public:
	NPCTeamMgr(Map* pMap);
	~NPCTeamMgr();

	// ���С�Ӷ��νṹ
	const tagNPCTeamOrder*	GetNPCTeamOrder(ENPCTeamOrder eTeamOrder)	{ return m_mapNPCOrder.Peek(eTeamOrder); }
	// ���С��ָ��
	NPCTeam*	GetNPCTeam(DWORD dwID) { return m_mapNPCTeam.Peek(dwID); }

	// ����С�Ӷ���
	static BOOL	LoadNPCOrder();
	// ˢ��ʱ����С�Ӷ�Աλ��
	Vector3		CalTeamMemPos(Creature *pLeader, POINT point, Vector3 vFace, const tagNestProto* pNestProto);
	// ��������С��
	NPCTeam*	CreateTeam(Creature* pLeader);

private:
	
private:
	TMap<DWORD, NPCTeam*>								m_mapNPCTeam;
	static TMap<ENPCTeamOrder, tagNPCTeamOrder*>		m_mapNPCOrder;
	Map*												m_pMap;
	DWORD												m_dwTeamIndex;		// С��ID����
};