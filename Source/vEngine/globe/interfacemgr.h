//-----------------------------------------------------------------------------
// File: obj_mgr
// Auth: Lyp
// Date: 2003/12/11
//-----------------------------------------------------------------------------
#pragma once
#include "frame\auto_factory.h"

namespace vEngine{
//-----------------------------------------------------------------------------
// ���˼��
// ��Interface�����������ȫ�ֵĶ���
// ʹ��ȫ�ֶ������ʹ�ø���ֻ��Ҫһ����������ʹ�ø��ӷ���,�������ʹ��
// ��ͨ��Singleton�����������кܶ�ײ�,�������ｨ��һ��ͳһ����Ľӿ�
//-----------------------------------------------------------------------------
class VENGINE_API ObjMgr
{
public:
	LPVOID	Get(LPCSTR szName);	// ����szName��ö���
	LPVOID	GetByTypeName(LPCSTR szName);	// ����typename�õ���Ӧ�Ķ���

	BOOL	Create(LPCSTR szDestObjName, LPCSTR szClassName);	// ����ע����Ķ��󴴽�����
	BOOL	Kill(LPCSTR szObjName);	// ��������

	// ע�������
	template<typename class_type>
	VOID Register(TWrap<class_type> wrap, LPCSTR szName);

	ObjMgr();~ObjMgr();

private:
	struct tagObj
	{
		LPVOID			pObj;
		DWORD			dwClassID;
		std::string		strObjName;
	};

	std::map<DWORD, tagObj>	m_mapObj;
	std::map<DWORD, DWORD>	m_mapTypeObj;	// typename -> m_mapObj ID
	std::list<std::string>	m_listObj;
	TAutoFactory<VOID>		m_AF;
	CRITICAL_SECTION		m_cs;

	BOOL CheckName(LPCSTR szName);

	// Ϊ�˲�ǣ��CUtil������ʵ��һ��CRC32
	DWORD Crc32(LPCSTR lpData);
};


//-----------------------------------------------------------------------------
// �ⲿע��
//-----------------------------------------------------------------------------
template<typename class_type>
VOID ObjMgr::Register(TWrap<class_type> wrap, LPCSTR szName)
{
	DWORD dwID = Crc32(szName); 
	DWORD dwTypeID = Crc32(typeid(class_type*).raw_name());
	CheckName(szName);

	EnterCriticalSection(&m_cs);
	m_AF.Register(wrap, dwID);
	tagObj obj;
	obj.pObj = NULL;
	obj.dwClassID = dwID;
	obj.strObjName.assign(szName);
	m_mapObj.insert( std::pair<DWORD, tagObj>(dwID, obj) );
	m_listObj.push_back(szName);

	// ͬʱע��type id
	m_mapTypeObj.insert(std::pair<DWORD, DWORD>(dwTypeID, dwID));
	LeaveCriticalSection(&m_cs);
}


}	// namespace vEngine{
