/*! \file RoleTitleFrame.h
	\brief ��ɫ�ƺŽ�����
	\author hyu
	\date 2009-2-2
*/
#pragma once
#include "RoleTitleProtoData.h"
#include "TreeEx.h"
#include "..\WorldDefine\msg_role_title.h"

/*!
	\struct tagTitleList
	\brief ����Ŀؼ��б����title list��Ϣ
*/
struct tagTitleList
{
	bool		bOwend;		/*!< �Ƿ��� */
    DWORD       dwTypeID;   /*!< ��������id */
	DWORD		dwItemID;	/*!< tree��item��id */
};

struct tagTitleTypeActivedCount
{
    tstring     strName;    /*!< ����� */
    INT         bCount;     /*!< ������ */
    INT         nTotal;     /*!< ���� */
    DWORD       dwTreeItemID;   /*!< �ؼ�item id*/
};

/*! 
	\class RoleTitleFrame
	\brief ��ɫ�ƺſ��
	\base GameFrame
	\sa RoleTitleFrame, CharacterFrame
*/
class RoleTitleFrame : public GameFrame
{
public:
	/*! ���캯�� */
	RoleTitleFrame(void);

	/*! �������� */
	~RoleTitleFrame(void);

	//! ��д��ʼ��
	/*!
		\param pMgr ��Ϸ��ܹ�����
		\param pFather �����
		\param dwParam ���Ӳ���
		\return ��ʼ���Ƿ�ɹ�
	*/
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);

	/*! ��д���� */
	virtual BOOL Destroy();

	/*! ��д����UI */
	virtual BOOL ReloadUI();

	/*! ��дˢ�� */
	virtual VOID Update();

	//! ˢ�³ƺŽ���
	/*! ��CharactarFrame��ʾ��������ʱ���µ�ǰ��ɫ�ƺ� */
	VOID UpdateRoleTitle();

private:
	/*! GUI�¼��ص� */
	DWORD EventHandler(tagGUIEvent* pEvent);

	/*! ��Ϸ�¼��ص� */
	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	//! ������Ϣ�ص�
    /*! ���ý�ɫʹ��ĳ�ƺ� */
    DWORD OnNS_ActiveRoleTitle(tagNS_ActiveRoleTitle* pMsg, DWORD dwParam);
	/*! �õ���ɫ�ѻ�õ����гƺ� */
	DWORD OnNS_GetRoleTitles(tagNS_GetRoleTitles* pMsg, DWORD dwParam);
	/*! ��ɫ���һ���³ƺţ�����֪ͨ�ͻ��� */
	DWORD OnNS_NewTitles(tagNS_NewTitles* pMsg, DWORD dwParam);
    /*! ��ɫɾ��һ���ƺţ�����֪ͨ�ͻ��� */
    DWORD OnNS_DelTitles(tagNS_DelTitles* pMsg, DWORD dwParam);

	//! ����������

	/*! ѡ��ĳһ�ƺ� */
	VOID SelectRoleTitle();
	/*! ���ʹ��ĳһ�ƺ� */
	VOID ActiveRoleTitle();
	/*! ������ĳһ�ƺ� */
	VOID ClearRoleTitle();
	/*! ��ʼ�����гƺ�xml���ݵ��ؼ� */
	VOID InitTree();
	/*! ���ݳƺű仯ˢ�¿ؼ� */
	VOID UpdateTree();

	//! ˽�г�Ա
	TSFPTrunk<RoleTitleFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	UINT16							m_un16ActivedTitleID;	/*!< ��ɫ����ʹ�õĳƺ�,��ΪGT_INVALID */
	map<DWORD, tagTitleList>		m_mapOwnedTitles;		/*!< �����ɫ�ѻ�óƺŵ����� */
    map<DWORD, tagTitleTypeActivedCount> m_mapActivedCount; /*!< ����title��õ����� */

	//! GUI�ؼ�
	GUIWnd*							m_pWnd;					/*!< ������ */
	GUIWnd*							m_pWndFather;			/*!< ������ */

	TreeEx*							m_pTreeTitleList;		/*!< �ƺ��б� */
	GUIScrollBar*					m_pScrBarTitleList;		/*!< ������ */
	GUIButton*						m_pBtTitleListUp;		/*!< �� */
	GUIButton*						m_pBtTitleListDown;		/*!< �� */

	GUIStaticEx*					m_pStcexDescription;	/*!< ��ʾ�ƺ����� */
	GUIScrollBar*					m_pScrBarDescription;	/*!< ������ */
	GUIButton*						m_pBtDescriptionUp;		/*!< �� */
	GUIButton*						m_pBtDescriptionDown;	/*!< �� */

	GUIButton*						m_pBtClear;				/*!< ��� */
	GUIButton*						m_pBtUse;				/*!< ʹ�� */

	GUIStatic*						m_pStcPercentage;		/*!< ����� */

};
