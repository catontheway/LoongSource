#pragma once
#include "..\Device\IDevice.h"
#include "imtlmodifier.h"

namespace Cool3D
{
	/**	\class MMAlphaMask 
		\brief ʹ��alpha test��ʵ�ְ���ָ����alpha��ֵ����͸��Ч��(�������͸��)
	*/
	class Cool3D_API MMAlphaMask :	public IMtlModifier
	{
	public:
		MMAlphaMask(void);
		virtual ~MMAlphaMask(void);

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	public:
		ECmpFunc	m_alphaTestFunc;
		BYTE		m_alphaRef;

		DECL_RTTI_DYNC(MMAlphaMask);
	};
}//namespace Cool3D