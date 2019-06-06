#pragma once

#include "Singleton.h"
#include "Mutex.h"
#include "../ServerDefine/fatigue_define.h"

//-------------------------------------------------------------------------
// �����Թ�����
//-------------------------------------------------------------------------
class FatigueMgr : public Singleton<FatigueMgr>
{
public:
	// ��ʼ������ȡ���״̬�ȵ�
	BOOL	Init();

	// ��ȡ�������߷��������
	BOOL	LoadAllOLAccounts();

	// ֪ͨ�����Է����������߳�����
	VOID	NotifyFagitueSvr();

	// ���ĳ����Ϸ���������ʺ�
	VOID	ResetWorldAccounts(DWORD dwWorldNameCrcReset, DWORD *pOLAccountIDs, INT nOLAccountIDNum);

	// ����֪ͨ�����߳�����
	VOID	LoginNotify(DWORD dwAccount, DWORD dwWorldNameCrc, BOOL bGuard);

	// �ǳ�֪ͨ�� ���߳�����
	VOID	LogoutNotify(DWORD dwAccount);

	// ֪ͨ����ۼ�����ʱ�䣬���߳�������
	VOID	NotifyPlayer(const tagFatigueNotify* pNotify);

	// ��world������Ϣ�����߳�����
	VOID	SendWorldNotifys();

	// ���̸߳���
	VOID	Update();

private:
	//accountid, worldnamecrc
	TMap<DWORD, DWORD>						m_mapAccountID2WorldNameCrc;
	Mutex									m_setLock;

	// worldnamecrc, list
	TMap<DWORD, TList<tagFatigueNotify*>* > m_mapNotifyLists;
	Mutex									m_mapLock;
	
	// ���Ӽ�ʱ��
	INT										m_nMinCounter;
	INT										m_nCircleUnit;

	BOOL									m_bEnable;
};

#define sFatigueMgr FatigueMgr::getSingleton()