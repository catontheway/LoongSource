#pragma once
#include "gameframe.h"

struct tagGuildWarePriChange;
struct tagRoleGetNameEvent;
struct tagNS_GetGuildWarePriList;

//! \class  GuildStorePermitFrame
//! \brief  ���ɲֿ�Ȩ�����ý���
//! \author hyu
//! \date   2009-8-3
//! \last   2009-8-3
class GuildStorePermitFrame :
    public GameFrame
{
public:
    GuildStorePermitFrame(void);
    ~GuildStorePermitFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

    void SetNpcId(DWORD id) {m_dwNpcId = id;}
private:
    //! ���ui
    void ClearUI();

    //! ˢ��
    void Refresh();

    //! ����б�
    void RefreshToList();

    //! ����б�
    void ClickList(int rowIndex);

    //! ȡ�ý�ɫ��
    tstring GetRoleName(DWORD dwRoleID);

    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ����Ȩ��
    void SetNetPrivacy(bool bVal, DWORD dwRoleId);

    //! �����Ȩ�޸ı�ʱ
    DWORD OnChangePrivacy(tagGuildWarePriChange* pGameEvent);

    //! ȡ��ɫ��
    DWORD OnGetRoleName(tagRoleGetNameEvent* pGameEvent);

    //! ��ȡȫ�����ɳ�ԱȨ���б�
    void BeginNC_GetGuildWarePriList();
    DWORD OnNS_GetGuildWarePriList(tagNS_GetGuildWarePriList* pMsg, DWORD);

    //! �õ�Ȩ���ı�
    LPCTSTR GetPrivacyText(BOOL bVal) {return bVal ? g_StrTable[_T("GuildPrivacy_yes")] : g_StrTable[_T("GuildPrivacy_no")];}
    DWORD GetPrivacyColor(BOOL bVal) {return bVal ? 0xFF00FF00 : 0xFFFF0000;}

    // ע�⣬���²��Һ����ķ���ֵΪ�Ƿ�鵽�д˶��󣬽�����������ò�����
    bool FindPrivacyByRoleID(DWORD dwRoleID, BOOL &ret);
    bool FindRoleIDByRow(int rowIndex, DWORD &ret);
    bool FindRowByRoleID(DWORD dwRoleID, int &ret);
    bool FindPrivacyByRow(int rowIndex, DWORD &dwRoleID, BOOL &ret);
    bool SetPrivacy(DWORD roleId, BOOL bVal);

    map<DWORD, BOOL>                    m_mapRole2Permit;
    map<int, DWORD>                     m_mapRow2Role;
    map<DWORD, int>                     m_mapRole2Row;

    DWORD                               m_dwNpcId;

    TSFPTrunk<GuildStorePermitFrame>    m_Trunk;
    TObjRef<GUISystem>			        m_pGUI;
    TObjRef<Util>				        m_pUtil;
    TObjRef<NetCmdMgr>			        m_pCmdMgr;
    TObjRef<GameFrameMgr>		        m_pFrameMgr;
    TObjRef<NetSession>			        m_pSession;

private:
    //GUI�ؼ�
    GUIWnd*         m_pWnd;
    GUIButton*      m_pBtClose;     //!< �رմ���
    GUIButton*      m_pBtActive;    //!< ����Ȩ��
    GUIButton*      m_pBtDiscard;   //!< ����Ȩ��
    GUIListBox*     m_pLBList;      //!< Ȩ���б�

    DWORD           m_updateTimeDistance;
};
