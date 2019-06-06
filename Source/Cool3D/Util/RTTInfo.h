#pragma once
#include "..\Cool3D.h"
class RTTIObj;
/**	\class RTTInfo
	\brief Runtime type info
*/
class Cool3D_API RTTInfo
{
public:
	typedef RTTIObj* (CreateObj)();
	RTTInfo(const TCHAR* szClassName,RTTInfo *pSuperClass,CreateObj *pFunc);
	~RTTInfo(void);

	bool IsKindOf(const RTTInfo *pInfo);
	bool IsKindOf(const TCHAR* szClassName);

	const TCHAR*	m_szClassName;
	RTTInfo		*m_pSuperClass;
	CreateObj	*m_pfnCreateObj;	//�������Ҫ��ΪNULL
};


/**	\class RTTFactory
	\brief Runtime type factor,�������ж����RunTimeClass����Ϣ,���ܸ���className����ָ�����Ķ���
	\see RTTInfo,RTTIObj
*/
class Cool3D_API RTTFactory
{
public:
	RTTFactory();
	~RTTFactory();
	void	AddRuntimeClass(const RTTInfo *pInfo);
	RTTIObj	*CreateObj(const TCHAR* szClassName);
	unsigned int GetNumClass() const;
	const RTTInfo *GetClass(unsigned int i) const;
	static RTTFactory *Inst();

private:
	class Member;
	Member	*m_p;
};

#define RTTI_NEW(className)\
	static_cast<className*>(RTTFactory::Inst()->CreateObj(#className))
