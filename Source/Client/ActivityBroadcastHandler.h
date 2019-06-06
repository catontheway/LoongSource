#pragma once

#include "..\WorldDefine\ScriptMsgInfo.h"
#include "..\WorldDefine\role_info.h"
#include "ChatFrame.h"

class ActivityBroadcastHandler
{
public:
	static ActivityBroadcastHandler* Inst();

	~ActivityBroadcastHandler();

	void Init();

	void Destroy();

private:
	// ���캯��
	ActivityBroadcastHandler();

	// ע��������ϢNS_MsgBlob
	DWORD OnNS_MsgBlob(tagNS_MsgBlob *pCmd, DWORD);
	
	// ��ʼ��msg_id��BroadcastImpl����ָ���Ӧ..
	void InitMsgID2BroadcastImplMap();
	
	// ����ȡ��һЩ���ֵ��¼�
	DWORD OnGetSomeNameOKEvent(tagRoleGetSomeNameEvent *pEvent);
	
	// ������������NS_MsgBlob
	void HandleNetMsg(tagNS_MsgBlob *pCmd);
	
	// ��ʼ�㲥
	void StartBroadcast();
	
	// ��ջ�õ�MsgUnit;
	void ClearMsgUnitVector();
	
	// ��ȡ�õ�ROLENAME�еõ�����
	tstring GetRoleName(DWORD dwRoleID) const;

	//֪ͨ�㲥
	inline void NotifyBroadcast(const TCHAR *szBuffer);
	
	// �ͷ���Ϣ������Դ
	void ReleaseMsgUnit();


//-------------------�����·�-----------------------------
	void BroadcastImpl1(const TCHAR *szBroadcastContent);
	void BroadcastImpl2(const TCHAR *szBroadcastContent);
	void BroadcastImpl3(const TCHAR *szBroadcastContent);
	void BroadcastImpl4(const TCHAR *szBroadcastContent);
	void BroadcastImpl5(const TCHAR *szBroadcastContent);
//--------------------------------------------------------

//-------------------�������-----------------------------
	void BroadcastImpl10(const TCHAR *szBroadcastContent);
	void BroadcastImpl11(const TCHAR *szBroadcastContent);
	void BroadcastImpl12(const TCHAR *szBroadcastContent);
	void BroadcastImpl13(const TCHAR *szBroadcastContent);
	void BroadcastImpl14(const TCHAR *szBroadcastContent);
	void BroadcastImpl15(const TCHAR *szBroadcastContent);
	void BroadcastImpl16(const TCHAR *szBroadcastContent);
	void BroadcastImpl17(const TCHAR *szBroadcastContent);
//--------------------------------------------------------

	void BroadcastImpl30(const TCHAR *szBroadcastContent);
	void BroadcastImpl31(const TCHAR *szBroadcastContent);
	void BroadcastImpl35(const TCHAR *szBroadcastContent);
	
private:
	std::list<tagNS_MsgBlob*> m_MsgList;	// ������յ�����������
	std::vector<DWORD> m_ReqIDList;			// ��ǰ��Ϣ��Ҫ���������ID
	std::vector<MsgUnit*> m_vecMsgUnit;		// �յ�������
	bool m_bHandlingNow;					// �Ƿ����ڴ���
	int m_msgID;							// ��ϢID
	
	// ָ��BroadcastImpl##�����Ա����ָ��
	typedef void (ActivityBroadcastHandler::*BroadcastImplPtr)(const TCHAR*);
	// msgid�����Ա����ָ���Ӧ��map
	std::map<int, BroadcastImplPtr> m_mapMsgID2BroadcastImpl;
	
	TObjRef<NetSession>					m_pSession;
	TSFPTrunk<ActivityBroadcastHandler> m_trunk;
	TObjRef<NetCmdMgr>					m_pNetCmdMgr;
	TObjRef<GameFrameMgr>				m_pGameFrameMgr;
	TObjRef<GameScriptMgr>				m_pGSMgr;
};

void ActivityBroadcastHandler::NotifyBroadcast(const TCHAR *szBuffer)
{
	ChatFrame* pFrame = (ChatFrame*)m_pGameFrameMgr->GetFrame(_T("Chat"));
	if (P_VALID(pFrame))
	{
		pFrame->PushScrollText(szBuffer);
		pFrame->PushInfo(szBuffer, ESCC_Affiche);
	}
	
}