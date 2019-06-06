#pragma once

const TCHAR *const g_szWindowClass=_T("GWin32Wnd");
const DWORD GAPP_QUIT=1000;//1000���ϵ���Ϣ���û�����
/**	\class GWin32App
	\brief Win32Ӧ�ó������
	\remarks
	\par ����һ������,��ִ����Ϣѭ��
	\par һ����ϷӦ�ó��������һ��,����ֻ����һ��GWin32App���������
*/
class GWin32App
{
	struct tagMsg
	{
		DWORD	msg,
				wParam,
				lParam;
	};
public:
	GWin32App(void);
	virtual ~GWin32App(void);
	static GWin32App* Inst();

	void Create(UINT windowW,UINT windowH,const TCHAR* szTitle,bool bFullScreenWndStyle,HICON hIcon=NULL);
	/**	��Ӧ�ó������һ����Ϣ,��Щ��Ϣû֡��ʼִ��OnIdle()֮ǰ�Ŵ���
	*/
	void PostMessage(DWORD msg,DWORD wParam,DWORD lParam);
	void MainLoop();

	HWND GetHWnd() { return m_hWnd; }
	
	virtual void OnCreate() =0;
	virtual void OnDestroy() =0;
	/**	��Ӧ�ó��򲻴���Win32 Messageʱ����
	*/
	virtual void OnIdle() =0;
	/**	Windows Message֪ͨ����
	*/
	virtual DWORD OnWin32Msg(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);

	void ScreenToClient(POINT *pPt);
	bool IfPointInClient(const POINT& pt);

private:
	void DoMessages();
	virtual bool DoMessage(DWORD msg,DWORD wParam,DWORD lParam)=0;
	list<tagMsg>	m_msgList;

protected:
	void Destroy();

protected:
	bool			m_bFullScreen;
	bool			m_bRun;
	HWND			m_hWnd;
};
