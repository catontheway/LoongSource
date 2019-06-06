#pragma once
#include "..\WorldDefine\creature_define.h"
#include "..\WorldDefine\msg_func_npc.h"
class NetSession;
/**
	class DakFrame
	brief ��վ
*/
class DakFrame :
	public GameFrame
{
public:
	DakFrame(void);
	~DakFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	void ShowWnd();
	void SetDakData(DWORD id, DWORD dwFunctionID);
	VOID SetCaption(LPCTSTR szCaption);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD NetRecvNS_Dak(tagNS_Dak* pMsg, DWORD dwParam);

	//��Ϸ�¼�
	DWORD OnUseDak(tagMsgBoxEvent* pGameEvent);

private:
	TSFPTrunk<DakFrame>			m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd						*m_pWnd;				//������
	GUIPatch					*m_pCaption;			//����
	GUIStaticEx					*m_pStcCondition;		//�Ի����ݴ���
	GUIListBox					*m_pListOptions;		//�Ի�ѡ��
	GUIButton					*m_pBtnCloseWnd1;		//�ر����������水ť����
	GUIButton					*m_pBtnCloseWnd2;		//�ر����������水ť����

	DWORD						m_dwNpcID;
	DWORD						m_dwFunctionID;
	DWORD						m_dwCurRow;
	DWORD						m_dwTotalRow;
	DWORD						m_escapeTime;
};