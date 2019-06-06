// MapEditor.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MapEditor.h"
#include ".\PlugMapEditor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMapEditorApp

BEGIN_MESSAGE_MAP(CMapEditorApp, CWinApp)
END_MESSAGE_MAP()


// CMapEditorApp ����

CMapEditorApp::CMapEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMapEditorApp ����

CMapEditorApp theApp;


// CMapEditorApp ��ʼ��

BOOL CMapEditorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C"
{
	__declspec(dllexport) bool _cdecl Plug_Init()
	{
		return true;
	}

	__declspec(dllexport) void _cdecl Plug_Destory()
	{

	}

	__declspec(dllexport) int _cdecl Plug_GetNumClass()
	{
		return 1;
	}

	__declspec(dllexport) const TCHAR* _cdecl Plug_GetClassName(int i)
	{
		return _T("MapObjEditor");
	}

	__declspec(dllexport) void* _cdecl Plug_NewObject(const TCHAR* szClassName, DWORD param1, const TCHAR* param2)
	{
		return new CPlugMapEditor;
	}

	__declspec(dllexport)void _cdecl Plug_DelObject(const TCHAR* szClassName, void* obj, const TCHAR* param2)
	{
		CPlugMapEditor* pPlug=(CPlugMapEditor*)obj;
		delete pPlug;
	}
}