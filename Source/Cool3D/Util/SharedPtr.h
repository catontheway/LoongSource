#pragma once
#include "DebugMisc.h"

namespace Cool3D
{

	template<class T>
	class SharePtrDelete
	{
	public:
		void FreePtr(T* pObj)
		{ delete pObj; }
	};
	template<class T>
	class SharePtrCRTFree
	{
	public:
		void FreePtr(T* pObj)
		{ free(pObj); }
	};
	/**	\class SharedPtr
		\biref �������ü�����ָ��
		\remarks ����һ��ָ��������һ��UINT�Ŀռ�����ż���, Ҳ����˵���е�SharePtr����ļ������ǹ����
	*/
	template<class T,class Del = SharePtrDelete<T> >
	class SharedPtr
	{
	protected:
		T* m_pObj;
		unsigned int *m_pCounter;

		void Release()
		{
			if(m_pCounter == NULL)
				return;
			
			if(*m_pCounter == 1)//��ʣ���Լ���Ψһ��������
			{
				delete m_pCounter;
				m_pCounter=NULL;
				Del tmp;
				tmp.FreePtr(m_pObj);
				m_pObj=NULL;
			}
			else
				(*m_pCounter)--;
		}
	public:
		virtual ~SharedPtr()
		{
			Release();
		}
		/** ���ʹ�ô�Ĭ�Ϲ��캯��,��������Bind()
			\see Bind()
		*/
		SharedPtr()
		{
			m_pObj=NULL;
			m_pCounter=NULL;
		}
		SharedPtr(T* pObj)
		{
			m_pObj=pObj;
			m_pCounter=new unsigned int;
			(*m_pCounter)=1;//��ʼ��Ϊ��һ������
		}
		SharedPtr(const SharedPtr& right)
		{
			if(right.m_pObj==NULL)
			{
				m_pObj=NULL;
				m_pCounter=NULL;
			}
			else
			{
				m_pObj=right.m_pObj;
				m_pCounter=right.m_pCounter;
				(*m_pCounter)++;
			}
		}
		const SharedPtr& operator=(const SharedPtr& right)
		{
			if(this==&right)
				return *this;
			
			Release();

			if(right.m_pObj==NULL)
			{
				m_pObj=NULL;
				m_pCounter=NULL;
			}
			else
			{
				m_pObj=right.m_pObj;
				m_pCounter=right.m_pCounter;
				(*m_pCounter)++;
			}
			return *this;
		}
		bool operator == (const SharedPtr& right) const
		{
			return m_pObj==right.m_pObj;
		}
		bool operator != (const SharedPtr& right) const
		{
			return m_pObj!=right.m_pObj;
		}

		void Bind(T* pObj)
		{
			Release();
			m_pObj=pObj;
			m_pCounter=new unsigned int;
			(*m_pCounter)=1;//��ʼ��Ϊ��һ������
		}

		//--ģ��Ĭ��ָ�����Ϊ
		T& operator*() const
		{ return *m_pObj; }
		T* operator->() const
		{ return m_pObj; }

		T* GetMem() const
		{ return m_pObj; }
		/*
		operator T*()//��ʽת����T*
		{	return m_pObj;	}
		operator void*()//����ת����void*, �������Զ�ת���Ķ�����, �����ⲿ����delete SharedPtrObj;
		{ return m_pObj; }
		*/
	};

	//typedef SharedPtr<unsigned char,SharePtrCRTFree<unsigned char> > SharedBytePtr;
	//typedef SharedPtr<unsigned short,SharePtrCRTFree<unsigned short> > SharedWordPtr;
}//namespace Cool3D