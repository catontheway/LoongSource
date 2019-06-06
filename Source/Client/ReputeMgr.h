#pragma once
#include "ProgressEx.h"
#include "..\WorldDefine\reputation.h"
#include "..\WorldDefine\role_att.h"

struct tagNS_GetRoleInitState_Reputation;
struct tagNS_RoleReputeChange;

//! �����׶���ɫ
enum EPrgrssColor
{
	EPC_RED			=	0,
	EPC_YELLOW		=	1,
	EPC_GREEN		=	2,
	EPC_BLUE		=	3,
	EPC_ORANGE		=	4,

	EPC_END			=	5
};

//! \struct tagReputeLevel
//! \brief �����ȼ�����
struct tagReputeLevel
{
	EReputationLevel		eLevel;			//!< �����׶�
	EPrgrssColor			eColor;			//!< �����׶���ɫ
	LPCTSTR					szLevel;		//!< �����׶��ı�
	INT						nFrom;			//!< ����ֵ [nFrom, nTo)
	INT						nTo;
	INT						nDiffValue;		//!< �����׶�ֵ = nTo - nFrom
	INT						nMaxContribute;	//!< ����ֵ����
};

//! ����ֵ�б仯
struct tagReputeChange : public tagGameEvent
{
	ECLanType				eClanType;		//!< ��������
	INT						nValue;			//!< ����ֵ
	INT						nChange;		//!< �仯ֵ

	tagReputeChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame){}
};

//! ��Ϊ����
struct tagBecomeEvent : public tagGameEvent
{
    ECLanType               eClanType;      //!< ��������

    tagBecomeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! �����䱦��������ı�
struct tagTreasureActCountChange : public tagGameEvent
{
    ECLanType               eClanType;      //!< ��������
    INT                     nCount;         //!< ʣ�༤�����
    tagTreasureActCountChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame) {}
};

//! ���������ȼ��仯 by add xtian 2009-6-15
struct tagReputeLvlChange : public tagGameEvent
{
	 ECLanType					eClanType;      //!< ��������
	 EReputationLevel			eLevel;			//!< �����׶�
	 tagReputeLvlChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
		 : tagGameEvent(szEventName, pSenderFrame) {}
};

//! \class ReputeMgr
//! \brief ����ֵ������
//! \author hyu
class ReputeMgr
{
public:
	ReputeMgr(void);
	~ReputeMgr(void);

	VOID Init();
	VOID Destroy();

	static ReputeMgr* Inst();

	//! ��ȡ��ǰ����ֵ
	INT GetReputeValue(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? m_nReputeValue[eCtype] : 0;}
    //! ��GetReputeValue����һ�������������ͬ����߻���������ص����������Ǵ�20��ʼ����Ӧʹ�ô˺���
    INT GetReputeValueEx(EReputationType eRtype) {return JDG_VALID(ERT, eRtype) ? m_nReputeValue[eRtype - ERT_BEGIN] : 0;}

	//! ��ȡ��ǰ�����ȼ�
	const tagReputeLevel* GetReputeLevel(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? &m_ReputeLevel[eCtype] : NULL;}

    //! ��õ�ǰ�Ƿ�������
    BOOL IsFame(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? m_bisFame[eCtype] : FALSE;}

    //! ��õ�ǰ�����䱦�������
    INT GetActCount(ECLanType eCtype) {return JDG_VALID(ECLT, eCtype) ? m_nActCount[eCtype] : 0;}

	//! ��������ֵ���������ȼ�
	BOOL UlityCalReputeLevel(tagReputeLevel& ReputeSrc, INT nReputeValue);

	//! ������������ɫ
	VOID UlitySetPrgrssColor( ProgressEx* pProgress, EPrgrssColor eColor );

private:

	//! ��������ֵ
	VOID SetReputeValue(ECLanType eCtype, INT nValue, INT nChange);

	//------------------------------------------------------------------------
	//! ������Ϣ
	//------------------------------------------------------------------------
	
	//! ��ʼ��
	DWORD OnInitReputation(tagNS_GetRoleInitState_Reputation* pNetCmd, DWORD);

	//! ��ʼ�����������������ȼ� by add xtian 2009-6-15
	VOID OnInitRepuLvl(ECLanType eType, INT nReputeValue);

	//! ��ǰ��ɫ���������ȼ������仯ʱ������Ϸ�¼� by add xtian 2009-6-15
	void UlityLocalPlayerRepuLvl( ECLanType eType, INT nReputeValue, INT nChange );

	//! ����ֵ�ı� 
	DWORD OnReputeChange(tagNS_RoleReputeChange* pNetCmd, DWORD);

    //! ����״̬�ı�
    DWORD OnBecomeFame(tagNS_BecomeFame* pNetCmd, DWORD);

    //! �����䱦��������ı�
    DWORD OnActCountChange(tagNS_TreasureActCountChange* pNetCmd, DWORD);

    //! ���幱�״�����ʱ����ʾ
    DWORD OnNS_ClanConUpperLimit(tagNS_ClanConUpperLimit* pNetCmd, DWORD);

	TSFPTrunk<ReputeMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	INT							m_nReputeValue[ECLT_NUM];		//!< ��ŵ�ǰ��ɫ����ֵ
	tagReputeLevel				m_ReputeLevel[ECLT_NUM];		//!< ��ŵ�ǰ��ɫ�����׶�
	EReputationLevel			m_eRepuLvl[ECLT_NUM];			//!< ��ŵ�ǰ��ɫ������������ȼ� by add xtian 2009-6-15
    BOOL                        m_bisFame[ECLT_NUM];            //!< �Ƿ�������
    INT                         m_nActCount[ECLT_NUM];          //!< �䱦�������

	//! ������ǰ����ɫ
	tstring						m_strPrgssForePic_Red;
	tstring						m_strPrgssForePic_Yellow;
	tstring						m_strPrgssForePic_Green;
	tstring						m_strPrgssForePic_Blue;
	tstring						m_strPrgssForePic_Orange;
};
