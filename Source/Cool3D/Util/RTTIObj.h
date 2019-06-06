#pragma once
#include "..\Cool3D.h"
#include "RTTInfo.h"

/**	\class RTTIObj
	\biref Runtime type identify object, ��Ҫ����ʱ������Ϣ�Ķ���������
	\remraks ʵ��һ��RTTIϵͳ,�ܵõ�ĳ����������ʲô��,�Ƿ����cast��ĳ����(��������),��������Ϣ����������ʱ��������;
*/
class Cool3D_API RTTIObj
{
public:
	RTTIObj(void);
	virtual ~RTTIObj(void);

	bool	IsKindOf(const TCHAR* szClassName) const;
	bool	IsStaticClass(const TCHAR* szClassName) const;
	bool	IsKindOf(const RTTInfo *pInfo) const;
	bool	IsStaticClass(const RTTInfo *pInfo) const;
	
	const TCHAR* GetClassName();
	//--RTTIϵ�еĺ��������Ķ���
	static	RTTInfo		m_classRTTIObj;
	virtual RTTInfo*	GetRTTInfo() const;
	//static RTTIObj	*CreateObj();
};

//--���ܶ�̬����ʵ����class������
#define DECL_RTTI(className)\
	public:\
	static	RTTInfo		m_class##className;\
    virtual RTTInfo*	GetRTTInfo() const;

#define IMP_RTTI(className,superClass)\
	RTTInfo		className::m_class##className(_T(#className),&superClass::m_class##superClass,NULL);\
	RTTInfo*	className::GetRTTInfo() const\
	{	return &m_class##className; }

//--�ܶ�̬�ؽ�ʵ����class������
#define DECL_RTTI_DYNC(className)\
	public:\
	static	RTTInfo		m_class##className;\
	virtual RTTInfo*	GetRTTInfo() const;\
	static RTTIObj	*CreateObj();
#define IMP_RTTI_DYNC(className,superClass)\
	RTTIObj*	className::CreateObj()\
	{ return new className;}\
	RTTInfo		className::m_class##className(_T(#className),&superClass::m_class##superClass,className::CreateObj);\
	RTTInfo*	className::GetRTTInfo() const\
	{	return &m_class##className; }

//--
#define IS_KIND_OF(className)\
	IsKindOf(&className::m_class##className)
#define IS_STATIC_CLASS(className)\
	IsStaticClass(&className::m_class##className)
	
	
	