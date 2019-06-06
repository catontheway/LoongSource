#pragma once
#include "gameframe.h"
#include "ChatSysDefine.h"

struct tagGetHeadpicByID;
struct tagRoleGetNameEvent;
struct tagNS_RoleChat;

class MilleSoundFrame : public GameFrame
{
	
public:
	MilleSoundFrame(void);
	~MilleSoundFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID PushCastText( LPCTSTR szText, DWORD dwRoleID, float fTime = 30.0f );

	VOID Move( tagPoint& pos );

private:

	// ����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);
	
	//��ʾһ��������Ϣ
	VOID ShowCastInfo();

	//�õ���ɫ����
	DWORD OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg );				//���� PlayerNameTab �����������
	
	//�õ���ɫURL
	DWORD OnRecvHeadpicByID( tagGetHeadpicByID* pGameEvent );				//���� PlayerNameTab �������URL

	//����������Ϣ������ֻ�Դ�����Ϣ����Ȥ
	DWORD OnNS_RoleChat( tagNS_RoleChat* pMsg, DWORD dwParam );

private:
	TSFPTrunk<MilleSoundFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	
	std::list<tagScroText>		m_lstCast;			//�����������
	std::map<DWORD,tagScroText>		m_mapCastNoName;	//����Ĵ���,û�еõ���ɫ��s
	

	GUIWnd*						m_pWnd;				//������
	GUIStatic*					m_pStcImg;			//ͼƬ
	GUIStatic*					m_pStcName;			//����
	GUIStaticEx*				m_pStcExContent;	//��������

	DWORD						m_dwStartTime;		//��ǰ������Ϣ��ʼ��ʱ��
	
	DWORD						m_dwMsgPersistTime;	//ÿ����������ͣ��ʱ��
	DWORD						m_dwWndCloseTime;	//���û�д����೤ʱ��ر�

	BOOL						m_bDisplaying;		//����������ʾ����
};
