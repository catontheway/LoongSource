#pragma once

#include "RoleInfoProtoData.h"
#include "..\WorldDefine\msg_role_vcard.h"
#include "HttpPicEx.h"
#include "InformationFrame.h"

struct tagRoleGetNameEvent;

struct tagOpenRoleInfoEvent : public tagGameEvent
{
    DWORD           dwRoleID;       //!< ��ɫid
    tagOpenRoleInfoEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
    tagGameEvent(szEventName, pSenderFrame) {}
};

/**
*class DispRoleInfoFrame
*brief ��ʾ�����������
*author hyu
*base GameFrame
*remark
*/
const DWORD TITLE_REFRESH_DELAY = 3000;     //!< ����

class DispRoleInfoFrame :
	public GameFrame
{
public:
	DispRoleInfoFrame(void);
	~DispRoleInfoFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

protected:
	VOID ShowWnd(BOOL bVal);

	//--����GUI��Ϣ
	DWORD EventHandler(tagGUIEvent* pEvent);
	//--������Ϸ�¼�
	DWORD OnOpenInfo(tagOpenRoleInfoEvent* pGameEvent);
	DWORD OnCloseInfo(tagGameEvent* pGameEvent);

    VOID SendGetRoleVCard();
    //�����ȡ��ɫ��������������Ϣ
    DWORD OnNS_GetRoleVCard(tagNS_GetRoleVCard* pMsg, DWORD dwParam);
    DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pGameEvent);

    //���
    VOID ClearAll();

    DWORD       m_dwCurrRoleID;     // ��ǰ����������roleID
    bool        m_bisRefreshing;    // �Ƿ�����ˢ��
    DWORD       m_dwCurrMateID;     // ��ǰ�����ѯ��ż���ֵ�ID

	TSFPTrunk<DispRoleInfoFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
    TObjRef<NetSession>				m_pSession;
    TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;

    GUIStatic*                      m_pStcName;             //����
	GUIStatic*						m_pStcSpouse;			//��ż
	GUIStatic*						m_pStcGender;			//�Ա�
	GUIStatic*						m_pStcConstellation;	//����
	GUIStatic*						m_pStcCity;				//����
	GUIStatic*						m_pStcChZodiac;			//��Ф
	GUIEditBoxEx*					m_pEditBoxExPresent;	//����ǩ��
	GUIEditBoxEx*					m_pEditNameHistory;		//������¼
	HttpPicEx*						m_pStcHeadPic;			//ͷ��

	GUIButton*						m_pBtRefresh;			//ˢ��
	GUIButton*						m_pBtClose;				//�ر�

	GUIButton*						m_pBtQuit;				//�رմ���
};
