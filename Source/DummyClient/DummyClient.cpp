// DummyClient.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DummyClient.h"
#include "ClientApp.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	ConfigFile cfgfile(_T(".\\config\\DummyCfg.txt"));

	int nWidth=cfgfile.GetInt(_T("width"));
	int nHeight=cfgfile.GetInt(_T("height"));

	ClientApp app;
	app.Create(nWidth,nHeight,_T("Dummy Client"),false);
	::ShowWindow(app.GetHWnd(),SW_SHOW);
	g_hWnd = app.GetHWnd();
	

#ifdef _DEBUG
	app.MainLoop();
#else
	try
	{
		app.MainLoop();
	}
	catch(Cool3D::Exception e)
	{
		MessageBox(NULL,_T("�༭�������쳣���뽫log�ļ�����������Ա!"),_T("�쳣"),MB_ICONERROR);
		exit(0);
	}
#endif
}