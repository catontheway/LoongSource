#pragma once
#include "..\Cool3D.h"
#include "Noncopyable.h"
namespace Cool3D
{
	/** \class PlugIn
		\brief һ���ⲿ����ļ��غ�ʹ��
		\remarks �ⲿ�Ĳ�������������¹淶:1.����������һ��������DLL;
		2.����һ��class�Ǵ����������е�class(Ӧ���Ǵ�����)������;
		3.���DLLӦ������ĸ�ָ����ʽ�ĺ���,�������ƺ���������:
	*/
	class Cool3D_API PlugIn : private Noncopyable
	{
		typedef bool		(_cdecl Plug_Init)();	//��ʼ��һ��
		typedef void		(_cdecl Plug_Destroy)();	//����
		typedef int			(_cdecl Plug_GetNumClass)();	//�õ���DLL�а�������PlugIn��Class
		typedef const TCHAR* (_cdecl Plug_GetClassName)(int i);	//����һ���ַ���,�����ж����PlugIn�ṩ����һ��ʲô�ӿڵ�ʵ��
		typedef void*		(_cdecl Plug_NewObject)(const TCHAR* szClassName,DWORD param1,const TCHAR* param2);	//����һ�����õĽӿڵ�ʵ�ֶ����ָ��
		typedef void		(_cdecl Plug_DelObject)(const TCHAR* szClassName,void *pObj,const TCHAR* param2);	//�ͷ�Plug_NewObject()���õ��Ķ���ָ��
		typedef void*		(_cdecl Plug_GetObject)(const TCHAR* szName);	//����Plugin�е�һ��ȫ�ֶ���,�˶�����Plugin���Ѿ�������
	public:
		PlugIn(void);
		virtual ~PlugIn(void);

		bool LoadFromDll(const TCHAR* szPath);
		void Close();

		int			GetNumClass();
		const TCHAR*	GetClassName(int i);
		void*		NewObject(const TCHAR* szClassName,DWORD param1,const TCHAR* param2);
		void		DelObject(const TCHAR* szClassName,void *pObj,const TCHAR* param2);
		void*		GetPlugObject(const TCHAR* szName);

	protected:
		HMODULE				m_hDll;
		Plug_GetNumClass	*m_funGetNumClass;
		Plug_GetClassName	*m_funGetClassName;
		Plug_NewObject		*m_funNewObject;
		Plug_DelObject		*m_funDelObject;
		Plug_Destroy		*m_funDestroy;
		Plug_GetObject		*m_funGetObj;
	};
}//namespace Cool3D