//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: vip_stall.h
// author: sunnee
// actor:
// data: 2009-06-15
// last:
// brief: VIP̯λ��
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "event_mgr.h"
#include "../WorldDefine/stall_define.h"

struct tagVIPStall;
struct tagVipStall2DB;

class VIPStall : public EventMgr<VIPStall>
{
public:
	VIPStall();
	~VIPStall();

	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	// �¼�������
	VOID	OnSetStallTitleFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnSetStallStatusFunc(DWORD dwSenderID, VOID* pEventMessage);
	VOID	OnChangeStallGoodsFunc(DWORD dwSenderID, VOID* pEventMessage);

	// ��ͼ�ϲ���Ϣ����
	DWORD	ApplyVIPStall(Role* pRole, BYTE byIndex, INT32 nRent);
	DWORD	GetAllVIPStallInfo(OUT tagVIPStall* pStallInfo, DWORD& dwTime);
	DWORD	GetUpdatedStallInfo(OUT tagVIPStall* pStallInfo, INT32& nNum, DWORD& dwTime);
	DWORD	GetVIPStallGoods(OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz, DWORD& dwRequestTime, BYTE byIndex);

public:
	// ̯λ��Ϣ���
	tagVIPStall*	GetVIPStallInfo(BYTE index);
	tagVIPStall*	GetVIPStallInfo(DWORD dwRoleID);
	Role*			GetVIPStallRole(BYTE index);
	BOOL			IsInStatus(BYTE index, EVIPStallStatus eStatus);
	DWORD			GetSpecVIPStallGoodsTime(BYTE index);

	// ����̯λ״̬
	DWORD			SetVIPStallStatus(BYTE index, EVIPStallStatus eStatus);

	// ��ɫɾ��ʱ���������
	VOID			RemoveRoleVIPStall(DWORD dwRoleID);

	// ��ȡ����VIP̯λ��Ϣ
	DWORD	LoadAllVIPStallInfo(tagVipStall2DB* pVIPStallInfo);

private:
	static	VOID RegisterStallEventFunc();

private:
	vector<tagVIPStall>		m_vecVIPStall;		// VIP̯λ��Ϣ
	TMap<DWORD, BYTE>		m_mapVIPStall;		// ��ɫID��VIP̯λ��Ŷ��ձ�
	BOOL					m_bInit;

	DWORD					m_dwVIPStallChange;	// VIP̯λ���ʱ��
	tagDWORDTime			m_dwSecTimer;		// ���¼�ʱ
	INT						m_nMinuteTimer;		// ���Ӽ�ʱ
};

extern VIPStall g_VIPStall;