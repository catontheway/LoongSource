#include "StdAfx.h"
#include ".\skinnodesw.h"

namespace F3D
{
	SkinNodeSW::SkinNodeSW(void)
	{
	}

	SkinNodeSW::~SkinNodeSW(void)
	{
	}

	bool SkinNodeSW::SupportLod()	//�Ƿ�֧��LOD,Ĭ��Ϊ��֧��
	{
		return true;
	}

	void SkinNodeSW::Draw(int nLod)	//֧��Lod��
	{
		if(nLod<0)
			nLod=0;
		if(nLod>=m_numLod)
			nLod=m_numLod-1;
	}

	bool SkinNodeSW::IsTransparent() const
	{
		return false;//temp
	}

	DWORD SkinNodeSW::GetMtlTrait() const
	{
		return 0;
	}

	const AABBox& SkinNodeSW::GetAABBox() const
	{
		return m_box;
	}

	void SkinNodeSW::Create(ResSkin *pRes)
	{}
}//namespace F3D