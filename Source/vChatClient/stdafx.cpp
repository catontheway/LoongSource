// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// xengine_test.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include "vChatClient.h"

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
INT APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	vEngine::InitNetwork();
	vEngine::InitEngine();
	
	_set_se_translator( vEngine::SEH_Translation ); 

	try
	{
		ChatClient* pTest = new ChatClient;
		if( pTest->Init(hInst) )
			pTest->Mainloop();
		pTest->Destroy();
		delete(pTest);
	} 
	catch(vEngine::Exception)
	{
		throw;
	}
	
	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();
	return 0;
}