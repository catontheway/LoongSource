/********************************************************************
	created:	2008/12/10
	created:	10:12:2008   11:31
	filename: 	d:\loong_client\Client\SocialListFrame.h
	file path:	d:\loong_client\Client
	file base:	SocialListFrame
	file ext:	h
	author:		leili
	
	purpose:	�����б��������
*********************************************************************/

#pragma once

#include "SocialMgr.h"

struct tagRoleGetNameEvent;
struct tagRoleGetIDEvent;

class SocialListFrame : public GameFrame
{
	enum EMenuItem
	{
		EMI_Chat,				//˽��
		EMI_GiveGift,			//��������
		EMI_PlayerInfo,			//�������
		EMI_InviteGroup,		//�������
		EMI_ApplyGroup,			//�������
		EMI_MoveTo,				//�ƶ���
		EMI_CopyName,			//��������
		EMI_Num,
	};

public:
	SocialListFrame(void);
	virtual ~SocialListFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 Show(void);
	VOID		 Hide(void);

	VOID		 RefreshList(SocialData::EListType listType);
	VOID		 RefreshList(void);

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	BOOL		 EventHandlerChild1(tagGUIEvent* pEvent);
	BOOL		 EventHandlerChildGroup(tagGUIEvent* pEvent);
	BOOL		 EventHandlerChildGift(tagGUIEvent* pEvent);

	DWORD		 OnAddFriend(tagMsgInputBoxEvent* pEvent);
	DWORD		 OnAddBlack(tagMsgInputBoxEvent* pEvent);
	DWORD		 OnDeleteFriend(tagMsgBoxEvent* pEvent);
	DWORD		 OnGetRoleName(tagRoleGetNameEvent* pEvent);
	DWORD		 OnSetReply(tagMsgInputBoxEvent* pEvent);
	DWORD		 OnGetRoleID(tagRoleGetIDEvent* pEvent);
	DWORD		 OnGetRoleIDError(tagGameEvent* pEvent);
	VOID		 DeleteBlack(DWORD roleID);

	VOID		 ShowFriendList(SocialData::EListType listType, bool bShowOutline);
	VOID		 ShowBlackList(bool bShowOutline);

	VOID		 ResizeMenu(GUIListBox* pListBox, int row, int width);
	VOID		 MoveMenu(GUIListBox* pListBox, const tagPoint& ptDest);

	VOID		 MoveToList(SocialData::EListType listType, int row);

	VOID		 SetFriendValPic(DWORD row,DWORD friendVal);

	VOID		 UpdateMenu(void);

private:
	TSFPTrunk<SocialListFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<MsgInputBox>		m_pMsgInputBox;
	TObjRef<MsgBox>				m_pMsgBox;

	GUIWnd						*m_pWnd;				// ������
	GUIListBox					*m_pListFriends;		// �����б�
	GUIPushButton				*m_pPshFriend1;			// ����Tab1
	GUIPushButton				*m_pPshFriend2;			// ����Tab2
	GUIPushButton				*m_pPshFriend3;			// ����Tab3
	GUIPushButton				*m_pPshFriend4;			// ����Tab4
	GUIPushButton				*m_pPshBlack;			// ������Tab
	GUIButton					*m_pBtnAddFriend;		// ���Ӻ��Ѱ�ť
	GUIButton					*m_pBtnAddBlack;		// ���Ӻ�������ť
	GUIButton					*m_pBtnDelete;			// ɾ����ť
	GUIButton					*m_pBtnClose;			// �رմ��ڰ�ť
	GUIPushButton				*m_pPshSwitchOnline;	// ���߿��ذ�ť
	GUIStatic					*m_pLevelCaption;		// �ȼ�����
	GUIPushButton				*m_pPshAutoReply;		// �Զ��ظ���ť

	SocialData::EListType		m_curListTab;			// ��ǰListTab
	bool						m_showOutline;			// �Ƿ���ʾ���ߺ��ѻ������
	bool						m_autoReply;			// �Ƿ��Զ��ظ�
	bool						m_bIsInitUI;			// �Ƿ��ʼ��UI

	GUIWnd						*m_pWndChild1;			// һ���Ӳ˵�
	GUIListBox					*m_pListMenu1;			// һ���б�
	GUIWnd						*m_pWndChildGroup;		// ���� �����Ӳ˵�
	GUIListBox					*m_pListMenuGroup;		// ���� �����б�
	GUIWnd						*m_pWndChildGift;		// ���� �����Ӳ˵�
	GUIListBox					*m_pListMenuGift;		// ���� �����б�

	DWORD						m_curRow;
	int							m_clickRow;				// ��ǰѡ�е�Row

	enum EAddType
	{
		EAT_None	= -1,
		EAT_Friend	= 0,
		EAT_Black
	};
	EAddType					m_eAddType;

	typedef std::pair<DWORD,DWORD> DelayNameRow;
	std::list<DelayNameRow>	m_delayNameRows;

	TList<DWORD>				m_gifts;				// �����е������б�

	enum EUIState
	{
		EUIS_OutOfChild1	= 1,
		EUIS_OutOfChild2	= 2,
	};
	DWORD						m_uiState;
	bool						m_bPreHideChild;
	float						m_startHideChildTime;

	INT							m_nCurMenuIndex[EMI_Num];  // ��ǰĿ��˵�ѡ���ӦListBox������
};