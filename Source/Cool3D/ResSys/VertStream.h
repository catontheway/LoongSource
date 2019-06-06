#pragma once
#include "..\Cool3D.h"
#include "..\Util\Noncopyable.h"
#include "..\Public\VertexDefine.h"
#include "..\Util\SharedPtr.h"
#include "..\Math\GeomDefine.h"
#include "..\Util\FArchive.h"

	class IFS;
namespace Cool3D
{
	/**	\class VertStream
		\brief vertex stream
	*/
	class Cool3D_API VertStream : private Noncopyable
	{
	public:
		VertStream(void);
		virtual ~VertStream(void);

		/**	����buffer,�����ļ��ж�ȡ���ж���
		*/
		void CreateFromFile(IFS *pFS,DWORD hFile,EVertType vertType,UINT numVert);
		void Create(UINT stride,UINT numVert);
		void Create(EVertType vertType,UINT numVert);
		/**	�´���һ������,�������ݺ��Լ�����ȫһ�µ�
			\param �Ƿ񿽱�Buffer�еĶ�������
		*/
		VertStream *Clone(bool bMemCpy);
		/**	�õ���������Ĵ�С
		*/
		UINT GetStride() const
		{	return m_stride; }
		EVertType GetType() const
		{	return m_type;	}
		UINT GetNumVert() const
		{	return m_numVert; }
		UINT GetBufferSize() const
		{	return m_numVert*m_stride; }
		void* GetRawBuffer() const
		{	return m_pBuf;	}

		//--
		bool GetPos(UINT i,Vector3& out) const;
		bool GetNormal(UINT i,Vector3& out) const;
		bool GetTangent(UINT i,Vector3& out) const;
		void BuildAABBox(AABBox& box);

		void Serialize(FArchive& ar);
		void Deserialize(FArchive& ar);

	protected:
		void Destroy();
	protected:
		EVertType	m_type;
		UINT		m_numVert;
		BYTE		*m_pBuf;
		UINT		m_stride;
	};

	//typedef SharedPtr<VertStream,SharePtrDelete<VertStream> >	SharedVertStream;
}//namespace Cool3D