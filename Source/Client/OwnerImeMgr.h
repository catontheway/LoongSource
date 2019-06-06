#pragma once

#pragma comment ( lib, "imm32.lib" )
#include <windows.h>
#include <imm.h>

class OwnerImeMgr
{
public:
	OwnerImeMgr(void);
	~OwnerImeMgr(void);
	void Init();
	static OwnerImeMgr* Inst();

	void Destroy();

	void OnWindowsMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	bool IfOwnerIme() { return m_bOwnerIme; }
	void EnableOwnerIme();				//�������뷨���˺���ҿ������ȼ�����ime
	void DisableOwnerIme();				//�رղ���ֹ���뷨����ime�Ѿ�����رգ��˺���Ҳ������ȼ�����ime
	void NextOwnerIme();					//�л�����һ�����뷨������EnableIme�����Ч
	void SharpOwnerIme( HWND hWnd );		//�л�ȫ��/���
	void SymbolOwnerIme( HWND hWnd );	//�л���/Ӣ�ı��

	//״̬����
	TCHAR* GetOwnerImeName();				//�õ����뷨���֣������ǰ��Ӣ���򷵻�NULL
	bool IfOwnerImeSharp();				//�Ƿ�ȫ��
	bool IfOwnerImeSymbol();				//�Ƿ����ı��
	void GetOwnerImeInput( TCHAR **pszCompStr, TCHAR **pszCompReadStr, TCHAR *pnImeCursor, TCHAR **pszCandList );

	//��������Ϣ�е��õĺ��������������true���򴰿ں���Ӧֱ�ӷ���0������Ӧ���ݸ�DefWindowProc
	bool OnWM_INPUTLANGCHANGEREQUEST();
	bool OnWM_INPUTLANGCHANGE( HWND hWnd );
	bool OnWM_IME_SETCONTEXT(){ return true; }
	bool OnWM_IME_STARTCOMPOSITION(){ return true; }
	bool OnWM_IME_ENDCOMPOSITION(){ return true; }
	bool OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam );
	bool OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam );

private:		
	void ConvertCandList( CANDIDATELIST* pCandList, TCHAR* pszCandList );	//��ѡ�ֱ�����ɴ�


private:
	TSFPTrunk<OwnerImeMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	bool						m_bOwnerIme;								//ime�����־
	TCHAR						m_szCompStr[ X_LONG_NAME ];					//�洢ת����Ĵ�
	TCHAR						m_szCompReadStr[ X_LONG_NAME ];				//�洢����Ĵ�
	TCHAR						m_szCandList[ X_LONG_NAME ];				//�洢������ַ���ѡ�ֱ�
	int							m_nImeCursor;								//�洢ת����Ĵ��еĹ��λ��
	CANDIDATELIST*				m_lpCandList;								//�洢��׼��ѡ�ֱ�
	TCHAR						m_szImeName[64];							//�洢���뷨������
	bool						m_bImeSharp;								//ȫ�Ǳ�־
	bool						m_bImeSymbol;								//���ı���־
};