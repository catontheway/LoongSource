// TerrainEditor.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "TerrainEditor.h"
#include "PlugTerrainEditor.h"

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

// CTerrainEditorApp

BEGIN_MESSAGE_MAP(CTerrainEditorApp, CWinApp)
END_MESSAGE_MAP()


// CTerrainEditorApp ����

CTerrainEditorApp::CTerrainEditorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTerrainEditorApp ����

CTerrainEditorApp theApp;


// CTerrainEditorApp ��ʼ��

BOOL CTerrainEditorApp::InitInstance()
{
	CWinApp::InitInstance();
	::SetCurrentDirectory(Kernel::Inst()->GetWorkPath());

	return TRUE;
}

extern "C"
{
	__declspec(dllexport) bool _cdecl Plug_Init()
	{
		return true;
	}
	__declspec(dllexport) void _cdecl Plug_Destroy()
	{}

	__declspec(dllexport) int _cdecl Plug_GetNumClass()
	{
		return 1;
	}

	__declspec(dllexport) const TCHAR* _cdecl Plug_GetClassName(int i)
	{
		return _T("TerrainEditor");
	}
	__declspec(dllexport) void* _cdecl Plug_NewObject(const TCHAR* szClassName,DWORD param1,const TCHAR* param2)
	{
		g_editor=new PlugTerrainEditor;
		return g_editor;
	}
	__declspec(dllexport) void _cdecl Plug_DelObject(const TCHAR* szClassName,void *pObj,const TCHAR* param2)
	{
		PlugTerrainEditor* pPlug=(PlugTerrainEditor*)pObj;
		delete pPlug;
		ASSERT(pPlug==g_editor);
		g_editor=NULL;
	}
}