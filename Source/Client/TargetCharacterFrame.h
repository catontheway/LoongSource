#pragma once
#include "gameframe.h"
#include "SocialEvent.h"
#include "ReputeMgr.h"
#include "RoleTitleProtoData.h"
#include "RoleTitleFrame.h"
#include "TreeEx.h"
class RemoteRoleStateFrame;
#include "..\WorldDefine\msg_role_title.h"
#include "..\WorldDefine\msg_reputation.h"

class TargetCharacterFrame :
    public GameFrame
{
public:
    TargetCharacterFrame(void);
    ~TargetCharacterFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
	virtual BOOL EscCancel();

    tstring GetGUIFullName()
    {
        if(P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else 
            return _T("");
    };

private:
    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ��ʾĿ�����װ��,����,�ƺŽ���
    DWORD OnDispPlayerState(tagDispPlayerState* pMsg);

    //! ��ʾ����
    VOID ShowWnd(BOOL bVal);

    //! ����������ʾ
    VOID ResetRepute();
    //! ���ͻ����������
    VOID BeginNC_GetReputation(DWORD dwRoleID);
    //! �����������
    DWORD OnNS_GetReputation(tagNS_GetReputation* pMsg, DWORD);

    //! ���óƺ���ʾ
    VOID ResetTitle();
    //! ���ͳƺŻ������
    VOID BeginNC_GetRoleTitles(DWORD dwRoleID);
    //! ���سƺŽ��
    DWORD OnNS_GetRoleTitles(tagNS_GetRoleTitles* pMsg, DWORD);
    //! ��ʼ�����гƺ�xml���ݵ��ؼ�
    VOID InitTree();
    //! ���ݳƺű仯ˢ�¿ؼ�
    VOID UpdateTree();
    //! ѡ��ĳһ�ƺ�
    VOID SelectRoleTitle();

	// ����װ��
	VOID ResetRemoteRoleEquip();

    map<DWORD, tagTitleList>		m_mapOwnedTitles;		/*!< �����ɫ�ѻ�óƺŵ����� */
    map<DWORD, tagTitleTypeActivedCount> m_mapActivedCount; /*!< ����title��õ����� */

    TSFPTrunk<TargetCharacterFrame> m_Trunk;
    TObjRef<GUISystem>			    m_pGUI;
    TObjRef<Util>				    m_pUtil;
    TObjRef<GameFrameMgr>		    m_pFrameMgr;
    TObjRef<NetCmdMgr>				m_pCmdMgr;
    TObjRef<NetSession>				m_pSession;

    // ������

    GUIWnd*						    m_pWnd;             //!< ������
    GUIWnd*                         m_pEquip;           //!< װ���Ӵ���
    GUIWnd*                         m_pRepute;          //!< ��������
    GUIWnd*                         m_pTitle;           //!< �ƺŴ���
    
    GUIPushButton*                  m_pBtEquip;         //!< ����װ��
    GUIPushButton*                  m_pBtReput;         //!< �������� 
    GUIPushButton*                  m_pBtTitle;         //!< ����ƺ�

    GUIButton*                      m_pBtClose;         //!< �ر�

    // �������
    ProgressEx*	    	            m_pPrgrssReputeValue[ECLT_NUM];
    GUIStatic*                      m_pStcReputeLevel[ECLT_NUM];
    GUIStatic*                      m_pStcContrib[ECLT_NUM];
    GUIStatic*                      m_pStcHero[ECLT_NUM];

    // �ƺ����
    TreeEx*                         m_pTreeTitleList;
    GUIStaticEx*                    m_pStcexDescription;

    DWORD                           m_dwCurrPlayerID;    //!< ��ǰ�鿴���ID
	RemoteRoleStateFrame*			m_pRRSF;

    bool                            m_bneedRefreshRepute;
    DWORD                           m_timeFirst;
};
