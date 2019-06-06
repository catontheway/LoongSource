#pragma once
#include "skinnode.h"
#include "..\ResSys\VertStream.h"

namespace F3D
{
	/**	\class SkinNodeSW
		\brief ʹ���������Skin��skin node
	*/
	class SkinNodeSW : public SkinNode
	{
	public:
		SkinNodeSW(void);
		virtual ~SkinNodeSW(void);

		//--render node
		virtual bool SupportLod();	//�Ƿ�֧��LOD,Ĭ��Ϊ��֧��
		virtual void Draw(int nLod =0);	//֧��Lod��
		virtual bool IsTransparent() const;
		virtual DWORD GetMtlTrait() const;
		virtual const AABBox& GetAABBox() const;

		//--skin node
		virtual void Create(ResSkin *pRes);

	private:
		//VertStream	m_
		int			m_numLod;
		AABBox		m_box;
	};
}//namespace F3D