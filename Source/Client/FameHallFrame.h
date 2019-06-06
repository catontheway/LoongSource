#pragma once

class ClanTreasureFrame;

#include "ProgressEx.h"
#include "ReputeFrame.h"
#include "PlayerNameTab.h"
#include "..\WorldDefine\msg_famehall.h"

const INT MAX_RANK_PLACE			=	17;		/*!< ����ÿҳ���ɵ��������������Ŀ�� */
const INT MAX_FAMOUS_TOP_PLACE		=	12;		/*!< ������ҳ���ɵ���������� */
const INT MAX_FAMOUS_COMMON_PLACE	=	19;		/*!< ������������ҳ������������� */

/*!
	\struct tagOpenClanTreasure
	\brief ֪ͨ�������䱦�б�
	\base tagGameEvent
	\sa tagGameEvent
*/
struct tagOpenClanTreasure : public tagGameEvent
{
	ECLanType			eClanTypeValue;			/*!< �������� */
	tagOpenClanTreasure(LPCTSTR szEventName, ECLanType eType, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame), eClanTypeValue(eType){}
};

/*! �������ݽṹ */
struct tagRankData
{
	DWORD		dwRoleID;		/*!< ��ɫID */
	tstring		strRoleName;	/*!< ��ɫ�� */
	INT			nReputeValue;	/*!< ����ֵ */
};

/*! �������ݽṹ */
struct tagFameData
{
	DWORD		dwNameID;		/*!< ����ID */
	tstring		strName;		/*!< ���� */
};

/*!
	\class FameHallFrame
	\brief �����ý���
	\author hyu
	\base GameFrame
	\remark
*/
class FameHallFrame : public GameFrame
{
public:
	FameHallFrame(void);
	~FameHallFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

    /*! ˢ������ */
    VOID UpdateFameHall();

private:


	/*! �����/�رձ��������Ϣ */
	DWORD OnOpenFamousEvent(tagGameEvent* pGameEvent);
	DWORD OnCloseFamousEvent(tagGameEvent* pGameEvent);

	/*! ���������䱦�������Ϣ */
	DWORD OnClanTreaOpen(tagGameEvent* pGameEvent);
	DWORD OnClanTreaClose(tagGameEvent* pGameEvent);

	/*! ��ʾ���� */
	VOID ShowWnd(bool bVal);

	/*! ����GUI�¼� */
	DWORD EventHandler(tagGUIEvent* pEvent);

	/*! ���õ�ǰѡ�������ҳ */
	VOID SetClan(ECLanType eVal);

	/*! ���������б���� */

	/*! ���ͻ�������б����� */
	VOID BeginGetReputRank(ECLanType eVal);
	/*! �������������б� */
	DWORD OnNS_GetReputeTop(tagNS_GetReputeTop* pMsg, DWORD param);
	/*! ������������������� */
	VOID FillReputRank(ECLanType eVal);
	/*! ������NameTable��ɫ�����¼����ش��� */
	DWORD OnGetRankName(tagRoleGetNameEvent* pGameEvent);
	/*! �������������������ݸ��µ����� */
	VOID RefreshRankToUI(ECLanType eVal);
	/*! ѡ����һҳ */
	VOID PrevRankPage();
	/*! ѡ����һҳ */
	VOID NextRankPage();
	/*! ��������е��������� */
	VOID ClearRankData(ECLanType eVal);
	/*! ��������������������� */
	VOID SafeDeleteRankData();

	/*! �������б���� */

	/*! ���ͻ���������б����� */
	VOID BeginGetFame(ECLanType eVal);
	/*! �����������б� */
	DWORD OnNS_GetFameHallRoles(tagNS_GetFameHallRoles* pMsg, DWORD param);
	/*! ������������������� */
	VOID FillFameData(ECLanType eVal);
	/*! ������NameTablebyNameid�����¼����ش��� */
	DWORD OnGetNameByNameID(tagGetNameByNameID* pGameEvent);
	/*! ���������������б���µ����� */
	VOID RefreshFameToUI(ECLanType eVal);
	/*! ѡ����һҳ */
	VOID PrevFamePage();
	/*! ѡ����һҳ */
	VOID NextFamePage();
	/*! ��������е��������� */
	VOID ClearFameData(ECLanType eVal);
	/*! ��������������������� */
	VOID SafeDeleteFameData();

	TSFPTrunk<FameHallFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	/*! GUI�ؼ� */
	GUIWnd*						m_pWnd;										/*!< ������ */

	GUIPushButton*				m_pBtClans[ECLT_NUM];						/*!< �����ҳ��ť */

	GUIStatic*					m_pStcRoleName[MAX_RANK_PLACE];				/*!< ����� */
	ProgressEx*					m_pPrgrss[MAX_RANK_PLACE];					/*!< ����ֵ */
	GUIStatic*					m_pStcReputeLevel[MAX_RANK_PLACE];			/*!< �������� */
	GUIStatic*					m_pStcRankPage;								/*!< ҳ�� */
	GUIButton*					m_pBtRankPrevPage;							/*!< ��һҳ */
	GUIButton*					m_pBtRankNextPage;							/*!< ��һҳ */

	GUIStatic*					m_pStcFameTopPage;							/*!< ������ҳ */
	GUIStatic*					m_pStcFamousTop[MAX_FAMOUS_TOP_PLACE];		/*!< ������ҳ��λ */
	GUIStatic*					m_pStcFameCommonPage;						/*!< ���˵�����ҳ */
	GUIStatic*					m_pStcFamousCommon[MAX_FAMOUS_COMMON_PLACE];/*!< ��������ҳ��λ */
	GUIStatic*					m_pStcFamousPage;							/*!< ҳ�� */
	GUIButton*					m_pBtFamousPrevPage;						/*!< ��һҳ */
	GUIButton*					m_pBtFamousNextPage;						/*!< ��һҳ */

	GUIButton*					m_pBtEnterClanTreasure;						/*!< ���������䱦��ť */
	GUIButton*					m_pBtClose;									/*!< ����رհ�ť */

	/*! �����䱦��� */
	ClanTreasureFrame*			m_pClanTreasure;							/*!< �����䱦Frame */
	bool						m_bisClanTreasureOpen;						/*!< �����䱦����״̬ */

	/*! ���� */
	ECLanType					m_eSelectedClan;							/*!< ��ǰѡ������� */

	vector<tagRankData*>		m_vecRankArray[ECLT_NUM];					/*!< ��������������� */
	map<DWORD, tagRankData*>	m_mapRoleIDtoRank[ECLT_NUM];				/*!< ͨ��roleid���ٲ����������� */
	INT							m_nRankPageCount[ECLT_NUM];					/*!< �����������ҳ�� */
	INT							m_nRankCurrPage[ECLT_NUM];					/*!< ����������ǰҳ */
	DWORD						m_dwRankUpdateTime[ECLT_NUM];				/*!< ����������������ʱ��� */

	vector<tagFameData*>		m_vecFameNameIDs[ECLT_NUM];					/*!< ������������� */
	map<DWORD, tagFameData*>	m_mapNameIDtoFame[ECLT_NUM];				/*!< ͨ��nameid���ٲ����������� */
	INT							m_nFamePageCount[ECLT_NUM];					/*!< �������ҳ�� */
	INT							m_nFameCurrPage[ECLT_NUM];					/*!< ���˵�ǰҳ */
	DWORD						m_dwFameUpdateTime[ECLT_NUM];				/*!< ����������������ʱ��� */

};
