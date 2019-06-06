#pragma once
struct tagNS_SetClass;
struct tagNS_ChangeClass;
struct tagNS_RoleClassChangeBroadcast;

//! \class  ProfessionMgr
//! \brief  ְҵ������
//! \author hyu
//! \date   2009-8-28
class ProfessionMgr
{
public:
    ProfessionMgr(void);
    ~ProfessionMgr(void);

    void Init();
    void Destroy();
    void Update();

    //! Singleton
    static ProfessionMgr& Inst();

    //! �õ�ר��ְҵ����
    LPCTSTR GetClassName(EClassType eClass);

    //! �õ�Ӣ��ְҵ����
    LPCTSTR GetClassExName(EClassTypeEx eClassEx);

    //! ��"�������ְҵר��"����
    DWORD OnSetClass(tagGameEvent* pEvent);

    //! ��"�������ר��ְҵ"����
    DWORD OnChangeClass(tagGameEvent* pEvent);

    //! ��"�������Ӣ��ְҵ"����
    DWORD OnChangeClassEx(tagGameEvent* pEvent);

    //! �����������ְҵר��
    void BeginNC_SetClass(DWORD dwNpcId, EClassType classType);
    DWORD OnNS_SetClass(tagNS_SetClass* pMsg, DWORD);

    //! �����������ר��ְҵ
    void BeginNC_ChangeClass(DWORD dwNpcId, EClassType classType);
    DWORD OnNS_ChangeClass(tagNS_ChangeClass* pMsg, DWORD);

    //! Զ�����ר��ְҵ�����ı䣬����Χ��ҹ㲥
    DWORD OnNS_RoleClassChangeBroadcast(tagNS_RoleClassChangeBroadcast* pMsg, DWORD);

    void ShowErrorMsg( DWORD dwErrCode );

private:
    //------------------------------------------------------------------------
    // ����
    //------------------------------------------------------------------------
    TSFPTrunk<ProfessionMgr>    m_Trunk;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
};
