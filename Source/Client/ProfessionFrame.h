#pragma once
#include "gameframe.h"
#include "ProfessionFrame.h"

const int MAX_CLASS_BUTTON_COUNT = 8;

enum ESetClassType
{
    ESClassType_NEW,    //!< �¾�ְ
    ESClassType_Change  //!< ���
};

//! \class  ProfessionFrame
//! \brief  תְ�������
//! \author hyu
//! \base   GameFrame
//! \sa     ProfessionMgr
class ProfessionFrame :
    public GameFrame
{
public:
    ProfessionFrame(void);
    ~ProfessionFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
    virtual BOOL EscCancel();

    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

    // ���õ�ǰ��������
    void SetOperaType(ESetClassType eType) { m_eType = eType; }

    // ���õ�ǰnpcId
    void SetNPCID(DWORD dwNpcId) { m_dwNpcId = dwNpcId; }

private:
    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ����Ի��򷵻���Ϣ
    DWORD EventSetClass(tagMsgBoxEvent* pEvent);
    DWORD EventChangeClass(tagMsgBoxEvent* pEvent);


    // �ؼ�
    GUIWnd*						m_pWnd;
    GUIButton*                  m_pBtClose;     //!< �ر�
    GUIButton*                  m_pBtChange;    //!< תְ
    GUIButton*                  m_pBtCancel;    //!< ȡ��
    GUIPushButton*              m_pPbClass[MAX_CLASS_BUTTON_COUNT]; //!< ְҵѡ��ť

    // ����
    TSFPTrunk<ProfessionFrame>  m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    // ��ǰ��������
    ESetClassType               m_eType;

    // ��ǰnpcId
    DWORD                       m_dwNpcId;

    // ��ǰ���µ�ְҵ
    EClassType                  m_eCurrClassType;
};
