
#include "RTTIObj.h"
#include "RTTInfo.h"
#pragma once
#include "..\Cool3D.h"
#include "Exception.h"
#include "..\VFS\IFS.h"
#include <string>
#include "..\Math\Math.h"
#include "..\Math\Color.h"

namespace Cool3D
{
	/**	\class FArchive
		\brief ���л���ʹ�õ�Ŀ���ĵ�
		\remarks ���л��ʺ����ڴ����ӵĶ���,�������νӿڴ洢�Ķ���
	*/
	class Cool3D_API FArchive
	{
	public:
		FArchive(void);
		virtual ~FArchive(void);

		void OpenForWrite(const TCHAR* szFileName);
		void OpenForRead(IFS* pFS,const TCHAR* szFileName);
		void Close();


		//--color
		friend FArchive& operator<<(FArchive& ar,const Color4f& color)
		{
			ar.ValueWrite(color.R);
			ar.ValueWrite(color.G);
			ar.ValueWrite(color.B);
			ar.ValueWrite(color.A);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Color4f& color)
		{
			ar.ValueRead(color.R);
			ar.ValueRead(color.G);
			ar.ValueRead(color.B);
			ar.ValueRead(color.A);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,const Color4ub& color)
		{
			ar.ValueWrite(color.R);
			ar.ValueWrite(color.G);
			ar.ValueWrite(color.B);
			ar.ValueWrite(color.A);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Color4ub& color)
		{
			ar.ValueRead(color.R);
			ar.ValueRead(color.G);
			ar.ValueRead(color.B);
			ar.ValueRead(color.A);
			return ar;
		}
		//--math
		friend FArchive& operator<<(FArchive& ar,const Vector2& vec)
		{
			ar.ValueWrite(vec.x);
			ar.ValueWrite(vec.y);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Vector2& vec)
		{
			ar.ValueRead(vec.x);
			ar.ValueRead(vec.y);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,const Vector3& vec)
		{
			ar.ValueWrite(vec.x);
			ar.ValueWrite(vec.y);
			ar.ValueWrite(vec.z);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Vector3& vec)
		{
			ar.ValueRead(vec.x);
			ar.ValueRead(vec.y);
			ar.ValueRead(vec.z);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,const Vector4& vec)
		{
			ar.ValueWrite(vec.x);
			ar.ValueWrite(vec.y);
			ar.ValueWrite(vec.z);
			ar.ValueWrite(vec.w);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Vector4& vec)
		{
			ar.ValueRead(vec.x);
			ar.ValueRead(vec.y);
			ar.ValueRead(vec.z);
			ar.ValueRead(vec.w);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,const Quaternion& quat)
		{
			ar.ValueWrite(quat.x);
			ar.ValueWrite(quat.y);
			ar.ValueWrite(quat.z);
			ar.ValueWrite(quat.w);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Quaternion& quat)
		{
			ar.ValueRead(quat.x);
			ar.ValueRead(quat.y);
			ar.ValueRead(quat.z);
			ar.ValueRead(quat.w);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,const Matrix4& mat)
		{
			ar.BufferWrite(mat.m,sizeof(float)*16);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,Matrix4& mat)
		{
			ar.BufferRead(mat.m,sizeof(float)*16);
			return ar;
		}
		//--string
		friend FArchive& operator<<(FArchive& ar,const TCHAR* szStr)
		{
			ASSERT(szStr!=NULL);
			size_t len=_tcslen(szStr)+1;
			ar.ValueWrite(len);
			ar.BufferWrite(szStr,len*sizeof(TCHAR));//д���ַ����ͽ�β��0
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,tstring& szStr)
		{
			return ar<<szStr.c_str();
		}
		friend FArchive& operator>>(FArchive& ar,tstring& szStr)
		{
			unsigned int len=0;
			ar.ValueRead(len);
			if(len==0)
			{
				szStr=_T("");
				return ar;
			}
			TCHAR* szTmp=new TCHAR[len];
			ar.BufferRead(szTmp,len*sizeof(TCHAR));
			szStr=szTmp;
			delete[] szTmp;
			return ar;
		}

		//--int
		friend FArchive& operator<<(FArchive& ar,int n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,int& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,unsigned int n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,unsigned int& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		//short
		friend FArchive& operator<<(FArchive& ar,short n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,short& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,unsigned short n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,unsigned short& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		//char
		friend FArchive& operator<<(FArchive& ar,char n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,char& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		friend FArchive& operator<<(FArchive& ar,unsigned char n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,unsigned char& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		//float
		friend FArchive& operator<<(FArchive& ar,float n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,float& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		//double
		friend FArchive& operator<<(FArchive& ar,double n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,double& n)
		{
			ar.ValueRead(n);
			return ar;
		}
		//bool
		friend FArchive& operator<<(FArchive& ar,bool n)
		{
			ar.ValueWrite(n);
			return ar;
		}
		friend FArchive& operator>>(FArchive& ar,bool& n)
		{
			ar.ValueRead(n);
			return ar;
		}
	public:
		//--���ڼ򵥵�int��ֵ���͵Ķ�д
		template<typename ValType>
			void ValueWrite(ValType& val)
		{
			ASSERT(m_pWriteFile!=NULL);
			if(m_pWriteFile)
				fwrite(&val,sizeof(ValType),1,m_pWriteFile);
		}
		template<typename ValType>
			void ValueRead(ValType& val)
		{
			ASSERT(m_pFS!=NULL && m_hReadFile!=NULL);
			if(m_pFS)
				m_pFS->Read(&val,sizeof(ValType),m_hReadFile);
		}
		//--д��buffer
		void BufferWrite(const void *pBuf,DWORD size)
		{
			ASSERT(m_pWriteFile!=NULL);
			if(m_pWriteFile)
				fwrite(pBuf,size,1,m_pWriteFile);
		}
		void BufferRead(void *pBuf,DWORD size)
		{
			ASSERT(m_pFS!=NULL && m_hReadFile!=NULL);
			if(m_pFS)
				m_pFS->Read(pBuf,size,m_hReadFile);
		}
	protected:
		int		m_mode;	//-1��Ч,0ΪOpenFroRead(),1ΪOpenForWrite
		FILE*	m_pWriteFile;	//д��ʱʹ��stdio file system
		IFS*	m_pFS;
		DWORD	m_hReadFile;	//����ʱʹ�������ļ�ϵͳ
	};

	/**	\class ISerializable
		\brief ���л��ӿ�
		\remarks ISerializable��һ�����ӿ�class,�ⲿclass���Զ�̳�
	*/
	class Cool3D_API ISerializable
	{
	public:
		ISerializable()	{}
		virtual ~ISerializable()	{}

		virtual void Serialize(FArchive& ar) =0;
		virtual void Deserialize(FArchive& ar) =0;
	};

	/**	��һ��������ISerializable���������"ָ��"��vector���л�
		\remarks Ϊ��֧�������л�,object�����ǿɶ�̬������RTTIObj
	*/
	template<typename SerType>
		void SerialObjPtrVector(FArchive& ar,vector<SerType*>& objPtrVector)
	{
		unsigned int size=objPtrVector.size();
		ar<<size;
		if(size==0)//û���κ�Ԫ��
			return;
		//--
		for(vector<SerType*>::iterator iter=objPtrVector.begin();
			iter!=objPtrVector.end();iter++)
		{
			SerType* obj=(*iter);
			//--����class name
			const RTTInfo *pClass=obj->GetRTTInfo();
			ar<<pClass->m_szClassName;
			ASSERT(pClass->m_pfnCreateObj!=NULL);

			obj->Serialize(ar);
		}
	}
	template<typename SerType>
		void DeserialObjPtrVector(FArchive& ar,vector<SerType*>& objPtrVector)
	{
		unsigned int size=0;
		ar>>size;
		if(size==0)
			return;
		//--
		ASSERT(objPtrVector.size()==0);//���治�����κ�Ԫ��
		objPtrVector.resize(size);
		for(vector<SerType*>::iterator iter=objPtrVector.begin();
			iter!=objPtrVector.end();iter++)
		{
			tstring szClassName;
			ar>>szClassName;
			SerType* obj=(SerType*)RTTFactory::Inst()->CreateObj(szClassName.c_str());
			obj->Deserialize(ar);
			(*iter)=obj;
		}
	}
	/**	��һ������ISerializable����������vector���л�
	*/
	template<typename SerType>
		void SerialObjVector(FArchive& ar,vector<SerType>& objVector)
	{
		unsigned int size=objVector.size();
		ar<<size;
		if(size==0)
			return;
		for(vector<SerType>::iterator iter=objVector.begin();
			iter!=objVector.end();iter++)
		{
			SerType& obj=(*iter);
			obj.Serialize(ar);
		}
	}
	template<typename SerType>
		void DeserialObjVector(FArchive& ar,vector<SerType>& objVector)
	{
		objVector.clear();
		//--
		unsigned int size=0;
		ar>>size;
		if(size==0)
			return;
		objVector.resize(size);
		for(vector<SerType>::iterator iter=objVector.begin();
			iter!=objVector.end();iter++)
		{
			SerType& obj=(*iter);
			obj.Deserialize(ar);
		}
	}
	//--��һ��������(int,float��)��vector�������л�
	template<typename ValType>
		void SerialValVector(FArchive& ar,vector<ValType>& valVector)
	{
		unsigned int size=valVector.size();
		ar<<size;
		if(size==0)
			return;
		//for(vector<ValType>::iterator iter=valVector.begin();
		//	iter!=valVector.end();iter++)
		//{
		//	ar.ValueWrite(*iter);
		//}
		ar.BufferWrite(&valVector[0],size*sizeof(ValType));
	}
	template<typename ValType>
		void DeserialValVector(FArchive& ar,vector<ValType>& valVector)
	{
		valVector.clear();
		//--
		unsigned int size=0;
		ar>>size;
		if(size==0)
			return;
		valVector.resize(size);
		//for(vector<ValType>::iterator iter=valVector.begin();
		//	iter!=valVector.end();iter++)
		//{
		//	ar.ValueRead(*iter);
		//}
		ar.BufferRead(&valVector[0],size*sizeof(ValType));
	}
}//namespace Cool3D