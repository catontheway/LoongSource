#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "PetDef.h"
#include "../WorldDefine/pet_skill_define.h"
#include "PetEvent.h"

const int MAX_PICKUP_FUNC   =   6;

//! \class  PetSkillFrame
//! \brief  ������ҳ��＼�����ý���
//! \author hyu
//! \base   GameFrame
//! \sa     PetManager
class PetSkillFrame :
    public GameFrame
{
public:
    PetSkillFrame(void);
    ~PetSkillFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

private:
    ////! ��������
    //void SavePetSkillCfgToFile(EPetskillType ePetSkillType);
    ////! ��ȡ����
    //void LoadPetSkillCfgToFile(EPetskillType ePetSkillType);

    //! ��ʼ��UI
    void InitalUI();
    //! resize combobox
    void ResizeCombobox(GUIComboBox* pComboBox, int rowCount);

    void SavePetSkillCfg(EPetskillType ePetSkillType);
    void LoadPetSkillCfg(EPetskillType ePetSkillType);
private:
    //! ��������¼�
    DWORD OnPetSkillCfgOpen(tagPetSkillCfgOpen* pEvent);

    //! ����GUI��Ϣ
    DWORD EventHandlerForage(tagGUIEvent* pEvent);
    DWORD EventHandlerSupply(tagGUIEvent* pEvent);
    DWORD EventHandlerPickUp(tagGUIEvent* pEvent);
    DWORD EventHandlerTraining(tagGUIEvent* pEvent);

    //! �򿪴���
    VOID ShowWndForage(BOOL bVal);
    VOID ShowWndSupply(BOOL bVal);
    VOID ShowWndPickUp(BOOL bVal);
    VOID ShowWndTraining(BOOL bVal);

    //! ȡ����������ID(�����ȼ���)
    DWORD GetComposeSkill(ESkillTypeEx2 eType);

    //! �Ƿ����ﵱǰ״̬����ʹ�ú�ҩ
    bool IfCanUseHPItem(Item* pItem);
    //! �Ƿ����ﵱǰ״̬����ʹ����ҩ
    bool IfCanUseMPItem(Item* pItem);
    //! �Ƿ����ﵱǰ״̬����ʹ�ó־���ҩ
    bool IfCanUseToughnessItem(Item* pItem);


    DWORD   m_CurrPetID;        //!< ӵ�д˼��ܵ�ǰ����
    DWORD   m_CurrSkillID;      //!< ��ǰ����ID

    //! ������������
    //tagPetSkillForage           m_ForageCfg;            //!< ��ʳ
    //bool                        m_bForageHasLoaded;
    //tagPetSkillPickup           m_PickupCfg;            //!< ����ʰȡ
    //bool                        m_bPickupHasLoaded;
    //tagPetSkillSupply           m_MedicineFeedCfg;      //!< ҩͯ
    //bool                        m_bMedicineFeedHasLoaded;

    TSFPTrunk<PetSkillFrame>    m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    //GUI�ؼ�
    GUIWnd*						m_pWndForage;               //!< ��ʳ
    GUIComboBox*                m_pCbCondition;             //!< ��ʳ����
    GUIPushButton*              m_pPbtEnable;               //!< ������ʳ����
    ItemButton*                 m_pBtFood;                  //!< ����ʳƷ
    GUIButton*                  m_pBtForageClose;           //!< �ر�

    GUIWnd*                     m_pWndSupply;               //!< ҩͯ
    GUIComboBox*                m_pCbRedCondition;          //!< ��ҩ����
    GUIPushButton*              m_pPbtRedEnable;            //!< ��ҩ���ܿ���
    ItemButton*                 m_pBtRed;                   //!< ��ҩ
    GUIComboBox*                m_pCbBlueCondition;         //!< ��ҩ����
    GUIPushButton*              m_pPbtBlueEnable;           //!< ��ҩ���ܿ���
    ItemButton*                 m_pBtBlue;                  //!< ��ҩ
    GUIComboBox*                m_pCbToughnessCondition;    //!< �־�������
    GUIPushButton*              m_pPbtToughnessEnable;      //!< �־������ܿ���
    ItemButton*                 m_pBtToughness;             //!< �־���ҩ
    GUIButton*                  m_pBtSupplyClose;           //!< �ر�

    GUIWnd*                     m_pWndPickUp;               //!< ����ʰȡ
    GUIPushButton*              m_pBtFunc[MAX_PICKUP_FUNC]; //!< ���ܿ���
    GUIButton*                  m_pBtPickClose;             //!< �ر�

    GUIWnd*                     m_pWndTraining;             //!< ѵ��
    GUIPushButton*              m_pPbtComposeSkill[ESSTE_End]; //!< �������ܸ�ѡ��
    GUIButton*                  m_pBtTrainingConfirm;       //!< ȷ��
    GUIButton*                  m_pBtTrainingCanel;         //!< ȡ��
    GUIButton*                  m_pBtTrainingClose;         //!< �ر�

};
