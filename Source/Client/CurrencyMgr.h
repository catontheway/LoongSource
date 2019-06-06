#pragma once
#include "..\WorldDefine\reputation.h"

struct tagNS_GetRoleInitState_Money;
struct tagNS_BagSilver;
struct tagNS_BagYuanBao;
struct tagNS_WareSilver;
struct tagNS_BaiBaoYuanBao;
struct tagNS_ClanCon;
struct tagNS_ExchangeVolume;

struct tagRoleSilverChange : public tagGameEvent
{
	INT64 n64Silver;
	INT64 n64Inc;

	tagRoleSilverChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

struct tagRoleYuanbaoChange : public tagGameEvent
{
	INT nYuanbao;
	INT nInc;

	tagRoleYuanbaoChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

struct tagRoleClanContributeChange : public tagGameEvent
{
	ECLanType eClanType;
	INT nCurClanCon;
	INT nChangeClanCon;

	tagRoleClanContributeChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

// by add xtian 2009-6-17
struct tagExchangeVolumeChange : public tagGameEvent
{
	INT32		nCurExVolume;		// ��ǰ�ܵ�����
	INT32		nChangeExVolume;	// �仯������
	tagExchangeVolumeChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

/** \class CurrencyMgr
	\brief ���ҹ����� ����������Ԫ�������幱��ֵ
*/
class CurrencyMgr
{
public:
	CurrencyMgr(void);
	~CurrencyMgr(void);

	VOID Init();
	VOID Destroy();

	//----------------------------------------------------
	//��ȡ
	//----------------------------------------------------
	//��ȡ�����е�����
	INT64 GetBagSilver(){return m_n64BagSilver;}
	//��ȡ�����е�Ԫ��
	INT	GetBagYuanbao(){return m_nBagYuanbao;}
	//��ȡ�ֿ��е�����
	INT64 GetWareSilver(){return m_n64WareSilver;}
	//��ȡ�ֿ��е�Ԫ��
	INT	GetBaibaoYuanbao(){return m_nBaibaoYuanbao;}
	//��ȡȡ���幱��
	INT GetClanContribute(ECLanType eCtype){return JDG_VALID(ECLT, eCtype) ? m_nClanContribute[eCtype] : -1;}
    INT GetClanContributeEx(ERContributionType eRCtype){return JDG_VALID(ERCT, eRCtype) ? m_nClanContribute[eRCtype - ERCT_BEGIN] : -1;}
	//��öһ���
	INT GetExchangeVolume() { return m_nExchangeVolume; }
	
	// ��Ǯ������ʾ
	tstring GetSilverDisplay(INT64 n64Silver);
	// Ԫ��������ʾ
	tstring GetYuanbaoDisplay(INT nYuanbao);

	static CurrencyMgr* Inst();

private:

	//----------------------------------------------------
	//��ֵ
	//----------------------------------------------------
	//���ñ����е�����
	VOID SetBagSilver(INT64 n64Value, INT64 n64Inc = 0);
	//���ñ����е�Ԫ��
	VOID SetBagYuanbao(INT nValue, INT nInc = 0);
	//���òֿ��е�����
	VOID SetWareSilver(INT64 n64Value, INT64 n64Inc = 0);
	//���òֿ��е�Ԫ��
	VOID SetBaibaoYuanbao(INT nValue, INT nInc = 0);
	//�������幱��
	VOID SetClanContribute(ECLanType eCtype, INT nValue, INT nInc = 0);
	//���öһ��� by add xtian 2009-6-17
	VOID SetExchangeVolume(INT nValue, INT nInc = 0);

	//----------------------------------------------------
	//������Ϣ����
	//----------------------------------------------------
	//������һ��ҳ�ʼ��
	DWORD NetInitRoleCurrency(tagNS_GetRoleInitState_Money* pNetCmd, DWORD);
	//������ұ����е������仯
	DWORD NetGetRoleBagSilver(tagNS_BagSilver* pNetCmd, DWORD);
	//������ұ����е�Ԫ���仯
	DWORD NetGetRoleBagYuanbao(tagNS_BagYuanBao* pNetCmd, DWORD);
	//������Ҳֿ��е������仯
	DWORD NetGetRoleWareSilver(tagNS_WareSilver* pNetCmd, DWORD);
	//������Ҳֿ��е�Ԫ���仯
	DWORD NetGetRoleBaibaoYuanbao(tagNS_BaiBaoYuanBao* pNetCmd, DWORD);
	//����������幱�ױ仯
	DWORD NetGetRoleClanContribute(tagNS_ClanCon* pNetCmd, DWORD);
	//������Ҷһ���仯 by add xtian 2009-6-17
	DWORD NetGetRoleExchangeVolume(tagNS_ExchangeVolume* pNetCmd, DWORD);

private:
	TSFPTrunk<CurrencyMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	INT64	m_n64BagSilver;
	INT64	m_n64WareSilver;
	INT		m_nBagYuanbao;
	INT		m_nBaibaoYuanbao;
	INT		m_nClanContribute[ECLT_NUM];
	INT		m_nExchangeVolume;
};
