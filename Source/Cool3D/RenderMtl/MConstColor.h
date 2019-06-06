#pragma once
#include "irendermtl.h"
#include "..\Math\Color.h"
#include "..\Device\IDevice.h"

namespace Cool3D
{
	/**	\class MConstColor
		\brief ��ɫ����
	*/
	class Cool3D_API MConstColor :	public IRenderMtl
	{
	public:
		MConstColor(void);
		virtual ~MConstColor(void);

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	public:
		tagRenderColor	m_color;
		bool			m_bUsingVertColor;//�رյƹ�,��ɫ����,ʹ��Vertexcolor����,vertex stream������Diffuse��Ա

		DECL_RTTI_DYNC(MConstColor);
	};
}//namespace Cool3D