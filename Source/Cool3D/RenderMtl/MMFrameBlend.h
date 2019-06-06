#pragma once
#include "..\Device\IDevice.h"
#include "imtlmodifier.h"

namespace Cool3D
{
	/**	\class MMFrameBlend 
		\brief ���õ�ǰ����ȾPass��֡����Ļ��״̬
	*/
	class Cool3D_API MMFrameBlend : public IMtlModifier
	{
	public:
		MMFrameBlend(void);
		virtual ~MMFrameBlend(void);

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	public:
		bool	m_blendEnable;
		bool	m_bMultiPass;
		EBlend	m_src,
				m_dst;

		DECL_RTTI_DYNC(MMFrameBlend);
	};
}//namespace Cool3D