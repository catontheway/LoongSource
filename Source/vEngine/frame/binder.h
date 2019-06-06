//-----------------------------------------------------------------------------
//!\file binder.h
//!\author Lyp
//!\data 2004/3/11
//! last 2004/3/14
//!
//!\brief ���೧�����ʹ�õİ���
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {
//-----------------------------------------------------------------------------
//! ��һЩ�����ӿڰ󶨵�ָ����
//-----------------------------------------------------------------------------
class BinderAbstract
{ 	
public:
	virtual LPVOID Create()=0;	// ��������Ľӿ�
	virtual VOID Destroy(LPVOID)=0;	// ɾ������Ľӿ�
};


//-----------------------------------------------------------------------------
//! ��create,destroy�󶨵�ָ����,���೧�����ʹ��
//-----------------------------------------------------------------------------
template<typename concrete_class>class TBinder : public BinderAbstract
{
public:
	LPVOID Create() { return (LPVOID)(new concrete_class); }
	VOID Destroy(LPVOID p) { delete((concrete_class*)p); }
};


//-----------------------------------------------------------------------------
//! ������������������Ϣ��ģ����
//-----------------------------------------------------------------------------
template<typename concrete_class>class TWrap{};



//-----------------------------------------------------------------------------
//! ���ͺ���ָ��ί��
//-----------------------------------------------------------------------------
class Delegate 
{
public:
      virtual void Invoke()=0;
protected:
      Delegate(){}
      virtual ~Delegate(){}
};


class NonTypeDelegate : public Delegate
{
public:
	void Invoke() { m_pfn(m_nParam);}
	NonTypeDelegate(void (*pfn)(int),int nParam):m_pfn(pfn),m_nParam(nParam){}
	virtual ~NonTypeDelegate(){}

private:
	void (*m_pfn)(int);
	int m_nParam;
};


template <typename T>class TTypeDelegate : public Delegate
{
public:
	void Invoke(){(m_t.*m_pfn)(m_nParam);}
	TTypeDelegate(T &t, void (T::*pfn)(int), int nParam):m_t(t),m_pfn(pfn),m_nParam(nParam){}
	~TTypeDelegate(){}
private:
	T &m_t;
	void (T::*m_pfn)(int);
	int m_nParam;
};



} // namespace vEngine {
