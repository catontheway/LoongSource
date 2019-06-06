//-----------------------------------------------------------------------------
//!\file ClientMain.h
//!\author Lyp
//!
//!\date 2008-03-27
//! last 2008-03-27
//!
//!\brief ���ͻ���
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
// Client
//-----------------------------------------------------------------------------
class Client
{
public:
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	VOID WaitThreadLoad();
	VOID ExitThreadLoad();

	Client():m_Trunk(this){}

private:
	TSFPTrunk<Client>			m_Trunk;
	TObjRef<InputDX8>			m_pInput;
	TObjRef<GameInputMap>		m_pInputMap;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<LuaManager>			m_pLuaMgr;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GUIEditor>			m_pGUIEditor;
	TObjRef<Console>			m_pConsole;
	TObjRef<GUIRender>			m_pGUIRender;
	TObjRef<GUIEditor>			m_pEditor;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GameScriptMgr>		m_pGameScriptMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<AudioSys>			m_pAudioSys;


	BOOL InitRenderSystem(HINSTANCE hInst);

	VOID RegisterControl();
	VOID RegisterFrame();

	// GUIWnd��ʾ����
	DWORD SwitchGUIWnd(DWORD dwID, BOOL bDown);
	// ��ͼ
	DWORD ScreenShot(DWORD dwID, BOOL bDown);

	// GM�����
	DWORD GMCommand(LPCTSTR szCommand);
	
	// ��3D������Դ��״̬д����־�ļ�
	DWORD LogResPool(LPCTSTR szCommand);

	// ��ʾ���ؽ�ɫ��������
	DWORD DisplayLocalRoleAtt(LPCTSTR szCommand);

	// �ر�/����ͷ��tips
	DWORD HeadTipsCommand(LPCTSTR szCommand);

    // ɾ��һ���ļ����������ļ�
    BOOL DeleteDirectory(LPCTSTR szDirName);
	
	HANDLE	m_hThreadLoad;	// ��ȡ�̵߳�HANDLE
	VOLATILE BOOL m_bTerminateThreadLoad; // �Ƿ������ȡ�߳�
	// ��ȡ��Դ�ļ����߳�
	UINT ThreadLoad(LPVOID);

};

