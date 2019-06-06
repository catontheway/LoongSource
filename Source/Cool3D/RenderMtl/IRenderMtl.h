#include "..\Device\IDevice.h"

#pragma once
#include "..\Cool3D.h"
#include "imaterial.h"

namespace Cool3D
{
	/**	\class IRenderMtl 
		\brief Texture����Ⱦ�õĲ�������
	*/
	class Cool3D_API IRenderMtl : public IMaterial
	{
	public:
		IRenderMtl(void);
		virtual ~IRenderMtl(void);

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

	public:
		bool	m_bEnableLight;
		bool	m_bTwoSide;		//�Ƿ���˫�����
		DECL_RTTI(IRenderMtl);
	};
}//namespace Cool3D