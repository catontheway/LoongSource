#pragma once
#include "gameframe.h"


struct tagGuildMemberClient;
struct tagGuildInfoReflashEvent;
class HttpPicEx;
struct tagGetHeadpicByID;

class GuildMemberInfoFrame :
	public GameFrame
{
public:
	GuildMemberInfoFrame(void);
	~GuildMemberInfoFrame(void);
	
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();

	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventGuildInfoUpdate(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildInfoReflash(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildPosChange(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildRemoveMember(tagGuildInfoReflashEvent* pGameEvent);

	VOID ReflashInfo(const tagGuildMemberClient* pMember);
	DWORD OnEventGetHeadPicByID(tagGetHeadpicByID *pEvent);

private:
	TSFPTrunk<GuildMemberInfoFrame>		m_Trunk;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GUISystem>					m_pGUI;

	GUIWnd*								m_pWnd;
	GUIWnd*								m_pWndInfo;
	GUIButton*							m_pBtnClose;		// �ر�
	GUIButton*							m_pBtnFriend;		// ��Ϊ����
	GUIButton*							m_pBtnTeam;			// �������
	GUIButton*							m_pBtnChat;			// ��Ϊ˽��
	GUIButton*							m_pBtnReflash;		// ������Ϣ
	HttpPicEx*							m_pStcImage;		// ��ͷ��
	GUIStatic*							m_pStcName;			// ����
	GUIStatic*							m_pStcPosition;		// ְλ
	GUIStatic*							m_pStcLevel;		// �ȼ�
	GUIStatic*							m_pStcSex;			// �Ա�
	GUIStatic*							m_pStcClass;		// ְҵ
	GUIStatic*							m_pStcCont;			// ���ɹ���
	GUIStatic*							m_pStcTotalCont;	// �ۼƹ���
	GUIStatic*							m_pStcKnowledge;	// ��������	
	GUIStatic*							m_pStcExploit;		// ���ɹ�ѫ

	DWORD								m_dwRoleID;			// ��ǰ���
};
