#pragma once

#include "..\WorldDefine\reputation.h"
#include "TreasureMgr.h"
#include "FameHallFrame.h"
#include "ItemButton.h"
#include "StaticEx.h"

const INT MAX_TREASURE_PLACE = 10;        //����ÿҳ���������Ʒ��

//! \class ClanTreasureFrame
//! \brief �����䱦����
//! \author hyu
//! \base GameFrame
//! \remark
class ClanTreasureFrame :
    public GameFrame
{
public:
    ClanTreasureFrame(void);
    ~ClanTreasureFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();

private:
    VOID UpdateClanTreasure();

    DWORD OnGameEvent(tagGameEvent* pGameEvent);

    DWORD OnOpenClanTreasure(tagOpenClanTreasure* pGameEvent);
    DWORD OnCloseClanTreasure(tagGameEvent* pGameEvent);

    //! �������㲥������µ��䱦������
    DWORD OnNewActivedTreasure(tagNewActivedTreasure* pGameEvent);
    //! �����䱦��ķ���
    DWORD OnActiveTreasureRet(tagActTreasureRezult* pGameEvent);

    //! ���ݵ�ǰ�䱦���ݳ�ʼ������
    VOID InitTreasure();
    //! ������ˢ�µ�����
    VOID RefreshToUI();
    //! ��ʣ�༤�����ˢ�µ�����
    VOID RefreshActCntToUI();
    //! �����һҳ����
    VOID PagePrev();
    //! �����һҳ����
    VOID PageNext();
    //! ��������䱦����
    VOID BeginActive();

    //! ����ҳ������λ�÷���������䱦,��û���򷵻�NULL
    const tagTreasureMgrData* GetTreaByPos(INT currPage, INT pos);

    DWORD EventHandler(tagGUIEvent* pEvent);

    VOID ShowWnd(bool bval);

    //! ���ѡ��ĳ�����
    VOID SelectItemPad(INT nIndex = GT_INVALID);

    TSFPTrunk<ClanTreasureFrame>    m_Trunk;
    TObjRef<GUISystem>              m_pGUI;
    TObjRef<Util>                   m_pUtil;
    TObjRef<GameFrameMgr>           m_pFrameMgr;
    TObjRef<NetSession>             m_pSession;

    // GUI�ؼ�
    GUIWnd*                         m_pWnd;

    StaticEx*                       m_pStcItemPad[MAX_TREASURE_PLACE];  //!< ��Ʒ��
    StaticEx*                       m_pStcIcon[MAX_TREASURE_PLACE];     //!< ��Ʒͼ��
    ItemButton*                     m_pIbtTrea[MAX_TREASURE_PLACE];     //!< ��Ʒ
    GUIStatic*                      m_pStcName[MAX_TREASURE_PLACE];     //!< ��Ʒ��
    GUIStatic*                      m_pStcStus[MAX_TREASURE_PLACE];     //!< ����״̬

    GUIStatic*                      m_pStcRestnum;          //!< ʣ�༤����

    GUIStatic*                      m_pStcPage;             //!< ҳ��
    GUIButton*                      m_pBtPrevPage;          //!< ǰһҳ
    GUIButton*                      m_pBtNextPage;          //!< ��һҳ

    GUIButton*                      m_pBtActiveTreasure;    //!< �����䱦
    GUIButton*                      m_pBtClose;             //!< �ر�

    INT                             m_nSelected;            //!< ѡ�е���Ʒ���������,GT_INVALIDΪ��ѡ�κ����

    ECLanType                       m_eClan;                //!< ��ǰѡ�������

    INT                             m_nPageCount;           //!< ��ǰ���ҳ��
    INT                             m_nCurrPage;            //!< ��ǰ����ҳ��

    vector<tagTreasureMgrData*>     m_vecTreasure;          //!< ��������ʵ�ȫ���䱦����

    tstring                         strSelectIcon;          //!< ѡ��ͼ��·��
    tstring                         strItemIcon;            //!< Ĭ��ͼ��·��
};

