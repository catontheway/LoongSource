#pragma once
#include "gameframe.h"
#include "..\WorldDefine\container_define.h"

class ItemButton;
class Item;
struct tagUpdateLPItemEvent;
struct tagItemPosChangeExEvent;

//! \class  GuildStorageFrame
//! \brief  ���ɲֿ����
//! \author hyu
//! \date   2009-8-3
//! \last   2009-8-3
class GuildStorageFrame :
    public GameFrame
{
public:
    GuildStorageFrame(void);
    ~GuildStorageFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

    //! ����NPC ID
    void SetNPCID(DWORD dwID) {m_dwNPCID = dwID;}

    //! �򿪽���
    VOID ShowWnd(BOOL bVal);

private:
    //! receive the initial msg
    DWORD OnInitGuildWareEvent(tagGameEvent* pGameEvent);

    //! update game event
    DWORD OnEventUpdateLPItem(tagUpdateLPItemEvent* pGameEvent);

    //! close
    DWORD OnEventCloseGuildWare(tagGameEvent* pGameEvent);

    DWORD OnEventItemPosChange(tagItemPosChangeExEvent* pGameEvent);

    //! ��ʾ��ǰҳ
    VOID ShowCurPage(INT16 page);

    //! ����λ�û����Ʒ
    Item* GetItem(INT16 index);

    //! ��λ���������е���Ʒˢ�µ�����
    VOID RefreshItem(INT16 index);

    //! �Ƿ��ڵ�ǰҳ
    BOOL IsCurPage(INT16 index){return ((SPACE_ONE_GUILDWARE * m_n16CurPage <= index) && (SPACE_ONE_GUILDWARE * (m_n16CurPage + 1) > index));}

    //! ������Ʒ�ƶ�
    //! \param n16PosSrcΪ�ƶ�����Ʒ���
    //! \param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
    VOID OnItemChange(INT16 n16PosSrc, INT16 n16PosDst = GT_INVALID);

    //! ������Ʒ����ֿ�
    //! \param ePosSrcΪ��Ʒԭ����
    //! \param ePosDstΪ��ƷĿ������
    //! \param n16PosSrcΪ��Ʒԭ���
    //! \param n16PosDstΪ�ƶ�����λ�ã�Ĭ��ֵΪ�ƶ�λ�ò�ָ��
    VOID OnItemChangeEx(EItemConType ePosSrc, EItemConType ePosDst, INT16 n16PosSrc, INT16 n16PosDst = GT_INVALID);

private:

    //GUI�ؼ�
    GUIWnd*         m_pWnd;
    GUIButton*      m_pBtClose;                                 //!< �رմ���
    GUIPushButton*  m_pPbt[MAX_GUILDWARE_NUM];                  //!< ��ҳ��ť
    ItemButton*     m_pUnitItem[SPACE_ONE_GUILDWARE];			//!< �ֿⵥԪ��

    INT16           m_n16CurPage;                               //!< ��ǰ�ֿ�ҳ

    DWORD           m_dwNPCID;                                  //!< �Ի�npc id

    DWORD           m_updateTimeFirst;
    DWORD           m_updateTimeDistance;
private:
    //! ����GUI��Ϣ
    VOID EventHandler(tagGUIEvent* pEvent);

    TSFPTrunk<GuildStorageFrame>        m_Trunk;
    TObjRef<GUISystem>			        m_pGUI;
    TObjRef<Util>				        m_pUtil;
    TObjRef<NetCmdMgr>			        m_pCmdMgr;
    TObjRef<GameFrameMgr>		        m_pFrameMgr;
    TObjRef<NetSession>			        m_pSession;
};
