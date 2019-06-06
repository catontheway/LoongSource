#pragma once
#include "..\rendermtl\imaterial.h"

namespace Cool3D
{
	/**	\class SimpleTerrainMtl 
		\brief Զ�����εĲ���
		\remark ֻ��һ�������ɫ
	*/
	class SimpleTerrainMtl : public IMaterial
	{
	public:
		SimpleTerrainMtl(void);
		virtual ~SimpleTerrainMtl(void);
		virtual bool IsResourceReady() const;
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

	public:
		Color4ub m_color;

		DECL_RTTI_DYNC(SimpleTerrainMtl);
	};
}//namespace Cool3D