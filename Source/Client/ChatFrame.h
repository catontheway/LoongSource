#pragma once
#include ".\ChatSysDefine.h"

#include "EmotionFrame.h"
#include "PlayerNameTab.h"
#include "StaticNoInput.h"
#include "..\WorldDefine\role_info.h"
#include "..\WorldDefine\chat.h"
#include "..\WorldDefine\msg_show_item.h"


//-----------------------------------------------------------------------------
//!class ChatFrame.h
//!brief ������
//-----------------------------------------------------------------------------
class NetSession;
struct tagPrivateChatEvent;
struct tagSocialInfoEvent;
struct tagDungeonEvent;
struct tagChannelSetEvent;
class RichEditBoxForChatInput;
class RichEditBoxForChatShow;
class ScrollRichEditBox;
class StaticForCast;
class HttpPicEx;
class GlintPushButton;

struct tagNPCEaseSpeechEvent;
struct tagNS_MonsterEnterCombat;
struct tagNS_RoleDead;
struct tagNS_Skill;
class ChatFrame:
	public GameFrame
{
public:
	enum SendMsgState
	{
		SendMsgState_NULL = 0,
		SendMsgState_Chat,
		SendMsgState_Show,
	};
public:
	ChatFrame(void);
	~ChatFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	//!�л���Ӣ����ʾͼ��,isEnglish:�Ƿ���Ӣ��
	VOID SwitchLangueInfo(bool isEnglish=true);

	//!�������������ĵ�����,�ڷ�����������ʱʹ��
	VOID GetInputText(tstring& str);	

	//��÷���Ƶ��
	ESendChatChannel GetSendChannel(){return m_curSendChannel;}

	//�����ʾƵ��
	DWORD GetRecvChannel(){return m_curRecvChannel;}

	//���˸�ʽ�Զ����ʽ�ַ���
	VOID FilterMarkerString(tstring&  strText);

	//ɾ��richEditBoxΪ�˽����Ű������֣�strText�����Զ����ϵĻس���
	VOID DeleteRETURNS(tstring& strText);

	//���ù����ı���Ĭ��ʱ��Ϊ1����(ʱ��δ��)
	VOID PushScrollText(LPCTSTR szText, float fTime = 60.0f);

	//���ô�����Ĭ��ʱ��Ϊ1����(ʱ��δ��)
	VOID PushCastText(LPCTSTR szText, DWORD dwRoleID, float fTime = 30.0f);

	//��Ӵӷ��������ܵ��������ݵ���Ӧ����ʾƵ��
	VOID PushInfo(const TCHAR* strText, ESendChatChannel escc= ESCC_End);

	//!ΪRichEditBoxע��ͼƬ��ת���ַ���
	//!��������ʹ�õ��ǹ̶�·�������Ұ�����ź�ͼƬ����ֻ��Ҫindex
	void RegisterPicForRichEdit(const TCHAR *szName,int index);

	//��������ǰ�����ı��Ĵ�С
	int GetInputValidTextSize();

	//���շ��������͵�������Ϣ
	DWORD NetRecvMsgNS_RoleChat( tagNS_RoleChat* pMsg, DWORD dwParam );

	void OnSendChatMsg(const tstring& strMsg, ESendChatChannel eChannel, DWORD dwDestID=GT_INVALID, BYTE byAutoReply=0);

	//����win32��Ϣ
	void OnWin32Msg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	void SetPvtRoleName(const TCHAR* name);

	HKL GetEnglishHKL() { return m_HKLenglish; }

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	BOOL GUIEventHandlerMenu(tagGUIEvent* pEvent);

	DWORD OnGameEventForChat(tagGameEventForChat* pEvent);

	//--�õ���ɫ���֡�ID
	DWORD OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg);
	DWORD OnRecvNetRoleGetID( tagRoleGetIDEvent* pMsg);
	//--�õ���ɫ����ͼURL
	DWORD OnRecvHeadpicByID( tagGetHeadpicByID* pGameEvent );
	//Ŀ�괰������˽��
	DWORD OnPrivateChatEvent(tagPrivateChatEvent* pEvent);
	//���պ�����������ʾ
	DWORD OnSocialInfoEvent(tagSocialInfoEvent* pEvent);
	//�����㲥
	DWORD OntagDungeonEvent(tagDungeonEvent* pEvent);
	// NPC���ж���˵��
	DWORD OnNPCEaseSpeechEvent(tagNPCEaseSpeechEvent* pEvent);
	// Ƶ������
	DWORD OnChannelSetEvent(tagChannelSetEvent* pGameEvent);
	// ����
	DWORD OnLeaveMsg(tagGameEvent* pGameEvent);

	//��ʼ��ʱ����������ʾƵ��
	VOID ResetRecvChannel();

	//���õ�ǰ˵��Ƶ����ť��ʾ
	VOID SetCurChannel(ESendChatChannel eChannel);

	//������Ƶ��ת��Ϊ�ͻ�����ʾƵ��
	DWORD TranIntEChat2EClient(ESendChatChannel eChannel);

	//������������ݷ����仯
	VOID OnInputContextChange();

	//�����������л�����Ƶ��
	ESendChatChannel TranCMD2SendChannel(int nChannel);

	//�л�������ʾƵ�������ͨ������Ƶ��listboxѡ��
	VOID OnSwitchPageShow(int index);

	//�����ʾƵ����ťʱ���Զ��л�����Ƶ����ת�����������ڽ������
	ESendChatChannel TranRecv2SendChannel(int nRecvChannel);

	//��������Ϣ��ӵ���ʾƵ��
	VOID AddStringToWnd(const tstring& strText, int nPate, ESendChatChannel escc);

	//���������ӱ���
	void AddInputEmotion(const tstring &str);

	//����˽�ģ�������������ֵ��ַ����������������
	tstring HandleStringForPvt(tstring& strText);

	//--����
	void OnSendCastTalk();
	void OnSendWorld();
	void OnSendPvt();
	void OnSendGuild();
	void OnSendTroop();
	void OnSendCurrent();
	void OnSendAffiche();

	//�Ƿ��ظ�����
	bool IsViladeSpeak(const tstring& str,ESendChatChannel escc);

	//����Ƶ�������Ƿ���С��10��
	bool IsIntervalSpeak(ESendChatChannel escc);

	//�õ���ӦƵ����������ɫ
	const TCHAR* GetChatChannelColor(ESendChatChannel escc);

	//���ش���
	void HideWnd();

	//--չʾװ������Ʒ
	void OnRoleSendItemOrEquip(ShowEvent* pEvent);
	DWORD NetRecvMsgNS_RoleShowEquip(tagNS_RoleShowEquip* pMsg, DWORD dwParam);
	DWORD NetRecvMsgNS_RoleShowItem(tagNS_RoleShowItem* pMsg, DWORD dwParam);
	DWORD OnNS_TemmatePickUpBroadCast( tagNS_RoleShowDroptItemInTeam* pMsg, DWORD dwParam );

	void OnRoleShowItem(DWORD data, const TCHAR* szSrcRole, const TCHAR* szDstRole, ESendChatChannel escc);
	void OnRoleShowEquip(const tagEquip& data, const TCHAR* szSrcRole, const TCHAR* szDstRole, ESendChatChannel escc);

	//--���Զ���
	BOOL JudePersonalityAct();

	//�������ý���
	void SetActive();

	//�Զ��ظ�
	void AutoReplyPvt(DWORD dwRoleID, LPCTSTR strText);

	void ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow );
	void ForceMoveMenu(const tagPoint& pos);

	//NPC���˵��
	DWORD OnNS_MonsterEnterCombat(tagNS_MonsterEnterCombat* pMsg, DWORD dwParam);
	DWORD OnNS_RoleDead(tagNS_RoleDead* pMsg, DWORD dwParam);
	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);

	//ϵͳ�����������
	void OnNS_AutoNotice(tagNS_AutoNotice* pMsg, DWORD dwParam);
	tstring AutoNoticeColor(INT nType);

	//Ƶ������
	void ChannelSet();
	void SaveChannelSet();

	//��ݼ�����
	DWORD OnKey(DWORD dwID, BOOL bDown);
	DWORD OnKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	
private:

	TSFPTrunk<ChatFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameInputMap>		m_pKeyMap;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pWndCaption;
	GUIWnd*						m_pSclFatherWnd;			//�������ؼ�
	GUIButton*					m_pBtnHide;					//���ذ�ť
	GUIButton*					m_pBtnChannelSet;			//Ƶ�����ð�ť
	GUIButton*					m_pBtnLeave;				//����
	GUIButton*					m_pBtnCurChannel;			//��ǰ����Ƶ����ť
	GUIButton*					m_pBtnEmotion;				//���鰴ť
	GUIStatic*					m_pStaticLangue[2];			//��Ӣ����ʾ
	RichEditBoxForChatInput*	m_pRebInput;				//���������
	GUIStatic*					m_pInpuBack;				//����򱳾�
	GUIListBox*					m_pChannelSel;				//����Ƶ��ѡ��
	GUIWnd*						m_pWndChannel;				//listbox������
	GUIPatch*					m_pMidBack;					//
	ScrollRichEditBox*			m_pScrollRichText;			//�����
	GUIFrameMutex*				m_pMutex;					//����Ƶ���Ļ���ؼ�
	//GUIPushButton*				m_pPage[NUM_CHAT_CHANNEL];	//�Ծ���ʾƵ����pushbutton
	GlintPushButton*				m_pPage[NUM_CHAT_CHANNEL];

	RichEditBoxForChatShow*		m_pChatContext[NUM_CHAT_CHANNEL];				//�������ʾ(����)Ƶ��
	GUIScrollBar*				m_pScl[NUM_CHAT_CHANNEL];	//��Ӧ��8��������
	GUIButton*					m_pBtnUp[NUM_CHAT_CHANNEL];	//��Ӧ�����������ϰ�ť
	GUIButton*					m_pBtnDp[NUM_CHAT_CHANNEL];	//��Ӧ�����������°�ť

	GUIWnd*						m_pWndHeadPic;				//����ͷ��
	GUIWnd*						m_pWndImName;				//��������
	StaticForCast*				m_pStcName[NUM_CHAT_CAST_WND];//������ҵ�����
	HttpPicEx*					m_pStcHeadPic[NUM_CHAT_CAST_WND];//��������
	GUIWnd*						m_pStcCast;
	ScrollRichEditBox*			m_pCastRichText[NUM_CHAT_CAST_WND];	//����
	GUIButton*					m_pBtnZoomUp;					//���Ű�ť����
	GUIButton*					m_pBtnZoomDn;					//���Ű�ť����
	
	GUIWnd*						m_pWndWorld;
	GUIButton*					m_pBtnWorldOK;
	GUIButton*					m_pBtnWorldCancel;
	GUIPushButton*				m_pPBtnWorld;
	GUIStatic*					m_pStcExWold;

	GUIWnd*						m_pWndMenuEx;					//��չ�˵�
	GUIListBox*					m_pListMenuEx;					//�˵�ѡ��

	INT							m_nZoomMax;

	BOOL						m_bChatContext;
	BOOL						m_bHide;

	
	ESendChatChannel			m_curSendChannel;			//��¼��ǰ����Ƶ��
	int							m_curRecvChannel;			//��ǰ��ʾƵ��
	DWORD						m_dwRecvChalShow[NUM_CHAT_CHANNEL];			//��ǰ��ʾƵ������	
	
	LONG						m_lnSlcFatherWndX;			// ����Ƹ��ؼ���client����
	LONG						m_lnCastWndX;				//�������ؼ���client����
	LONG						m_lnCastWndY;

	EmotionFrame*				m_pEmotionFrame;			//������

	
	vector<tagRecvSpeak>		m_vecRecv;					//���ܵ��������ݻ��棬���浽��ȷ��ʾ����Ϊֹ
	vector<tagSendSpeak>		m_vecSpeak;					//�Ѿ������ķ��ԣ�����һ��ʱ��
	vector<tagSendSpeak>		m_vecPvtSpeak;				//˽��˵�Ļ�
	vector<tagScroText>			m_vecSclText;				//�ӷ����������������Ϣ����
	vector<tagScroText>			m_vecCast;					//����Ĵ���
	vector<tagScroText>			m_vecCastNoName;			//����Ĵ���,û�еõ���ɫ��s
	DWORD						m_dwLocalID;				//�������ID
	tstring						m_strLocalName;				//�����������
	tstring						m_strLastName;				//��һ��˽���������
	tstring						m_strMenuName;				//��չ�˵����ʱ���������

	vector<tagChatShowEquip>	m_vecRecvEquip;				//���յ�չʾװ������
	vector<tagChatShowItem>		m_vecRecvItem;				//���յ�չʾ��Ʒ����

	BOOL						m_bWorldOK;					//����Ƶ�����Կ�Ǯȷ��
	SendMsgState				m_eSendMsgState;
	ShowEvent					m_ShowEvent;
	HKL							m_HKL;
	HKL							m_HKLenglish;

};
