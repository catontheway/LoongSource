#pragma once
#include "gameframe.h"
#include "PetFrame.h"

struct  tagGetPetAttrEvent;
class   PetShowWindow;
class   ItemButton;
class   PetSkillButton;
struct  tagPet;

//! \class  PetInformationFrame
//! \brief  ������Ϣ��ѯ����
//! \author hyu
//! \base   GameFrame
class PetInformationFrame :
    public GameFrame
{
public:
    PetInformationFrame(void);
    ~PetInformationFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
    virtual BOOL EscCancel();

    //! ���Ͳ�ѯ��Ϣ����
    VOID GetPetReq(DWORD dwRoleID, DWORD dwPetID);

private:
    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ���ui
    VOID ClearUI();

    //! ˢ�½�������
    VOID RefreshToUI();
    //! ˢ�µ�ǰװ��
    VOID RefreshEquip();
    //! ˢ�µ�ǰ����
    VOID RefreshSkill();
    //! ˢ�µ�ǰģ��
    VOID RefreshPetShow();

    // ��Ϸ�¼�
public:
    DWORD OnGetPetAttrEvent(tagNS_GetPetAttr* pMsg);
private:
    DWORD OnPetAttrChange( tagPetAttrChange* pEvent );

    void SetPet(tagPet* pPet);

    //! ��õȼ�����
    tstring GetLevelText(int nLevel);

    DWORD                       m_currRoleId;
    tagPet*                     m_pCurrPet;

    // GUI�ؼ�
    GUIWnd*						m_pWnd;

    GUIButton*                  m_pBtClose;                     //!< �ر�

    PetShowWindow*              m_pStcPetShow;                  //!< ����Avata

    ItemButton*                 m_pBtEquipA[MAX_PET_EQUIP];     //!< ����װ��
    PetSkillButton*             m_pBtSkillA[MAX_PET_SKILL];     //!< ���＼��

    GUIStatic*                  m_pStcNick;                     //!< �����ǳ�
    GUIStatic*                  m_pStcLevel;                    //!< ����
    GUIStatic*                  m_pStcCurrExp;                  //!< ��ǰ����
    GUIStatic*                  m_pStcLevelUpNeed;              //!< ��������

    GUIStatic*                  m_pStcQuality;                  //!< ����Ʒ��
    GUIStatic*                  m_pStcMatureTalent;             //!< �ɳ�����
    GUIStatic*                  m_pStcPotential;                //!< Ǳ��
    GUIStatic*                  m_pStcMagic;                    //!< ����֮��(ԭ������)
    GUIStatic*                  m_pStcEnergy;                   //!< ����(ԭ����)
    GUIStatic*                  m_pStcRidingSpeed;              //!< ����ٶ�

    // ����
    TSFPTrunk<PetInformationFrame>  m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    //! ֧���������ͻ���ת��
    template<typename T>
    tstring TtoString(T nSrc);
};

template<typename T>
tstring PetInformationFrame::TtoString( T nSrc )
{
    tstringstream sstream;
    sstream << nSrc;
    tstring ret;
    sstream >> ret;
    return ret;
}