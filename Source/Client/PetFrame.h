#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "PetSkillButton.h"
#include "StaticEx.h"
#include "../WorldDefine/msg_pet.h"
#include "PetEvent.h"

const INT MAX_PET_EQUIP     =   4;
const INT MAX_PET_SKILL     =   12;

class PetShowWindow;
class PetBagFrame;

//! \class  PetFrame
//! \brief  ������ҳ���������
//! \author hyu
//! \base   GameFrame
//! \sa     PetManager
class PetFrame :
    public GameFrame
{
public:
    PetFrame(void);
    ~PetFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
	virtual BOOL EscCancel();

    bool IsWindowVisible();

private:
    //! ����GUI��Ϣ
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! ��/�رճ��������Ϸ�¼�
    DWORD OnOpen_Pet(tagGameEvent* pMsg);
	DWORD OnClose_Pet(tagGameEvent* pMsg);

    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

    //! ˢ�³����б�
    VOID RefreshPetList();
    //! ˢ�½�������
    VOID RefreshToUI();
    //! ˢ�µ�ǰװ��
    VOID RefreshEquip();
    //! ˢ�µ�ǰ����
    VOID RefreshSkill();
    //! ˢ�µ�ǰģ��
    VOID RefreshPetShow();

    //! ��õȼ�����
    tstring GetLevelText(int nLevel);
    //! ���Ʒ������
    LPCTSTR GetQualityText(int nQuality);

    //! ���ý���
    VOID ClearUI();

    //! ȷ��ɾ������
    DWORD OnCommitDeletePet(tagMsgBoxEvent* pEvent);

    //! ȷ����ע����
    DWORD OnCommitPourExp(tagMsgBoxEvent* pEvent);

    //! ������ﴩװ���¼�
    DWORD OnUsePetEquipHandler(tagUsePetEquip* pEvent);

    //! �������Է����仯
    DWORD OnPetAttrChange(tagPetAttrChange* pEvent);

    //! ֧���������ͻ���ת��
    template<typename T>
    tstring TtoString(T nSrc);

    DWORD                       m_dwCurrSelectPetID;    //!< ��ǰѡ�еĳ���ID(����ѡ����ΪGT_INVALID)

    map<INT, DWORD>             m_mapRow2Pet;           //!< �б�ؼ��кͳ���id���ձ�

    TSFPTrunk<PetFrame>         m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    //GUI�ؼ�
    GUIWnd*						m_pWnd;

    GUIStatic*                  m_pStcState;                    //!< ״̬
    PetShowWindow*              m_pStcPetShow;                  //!< ����Avata

    GUIListBox*                 m_pLbPetList;                   //!< �����б�

    GUIButton*                  m_pBtCall;                      //!< �ٻ�
    GUIButton*                  m_pBtSetFree;                   //!< ����
    GUIButton*                  m_pBtPerfuse;                   //!< ��ע
    GUIButton*                  m_pBtRide;                      //!< ���
    GUIButton*                  m_pBtTrade;                     //!< ����

    ItemButton*                 m_pBtEquipA[MAX_PET_EQUIP];     //!< ����װ��
    PetSkillButton*             m_pBtSkillA[MAX_PET_SKILL];     //!< ���＼��

    GUIStatic*                  m_pStcQuality;                  //!< ����Ʒ��
    GUIStatic*                  m_pStcMatureTalent;             //!< �ɳ�����
    GUIStatic*                  m_pStcPotential;                //!< Ǳ��
    GUIStatic*                  m_pStcMagic;                    //!< ����֮��(ԭ������)
    GUIStatic*                  m_pStcEnergy;                   //!< ����(ԭ����)
    GUIStatic*                  m_pStcRidingSpeed;              //!< ����ٶ�

    GUIStatic*                  m_pStcNick;                     //!< �����ǳ�
    GUIStatic*                  m_pStcLevel;                    //!< ����
    GUIStatic*                  m_pStcCurrExp;                  //!< ��ǰ����
    GUIStatic*                  m_pStcLevelUpNeed;              //!< ��������

    GUIButton*                  m_pBtClose;                     //!< �ر�

    PetBagFrame*                m_pPetBagFrame;                 //!< pet bag frame
};

template<typename T>
tstring PetFrame::TtoString( T nSrc )
{
    tstringstream sstream;
    sstream << nSrc;
    tstring ret;
    sstream >> ret;
    return ret;
}