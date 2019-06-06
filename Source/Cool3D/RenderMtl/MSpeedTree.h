#pragma once
#include "imaterial.h"

namespace F3D
{
	class CSpeedTreeRT;
	class MConstColor;
	class MBitmap;
	/**	\class MSpeedTree 
		\brief speed treeϵͳ�Ĳ��ʰ�װ
		\todo һ��Speedtree���ʰ����������ʶ���,һ�������ɲ���,һ������Ҷ��֦�Ĳ���
	*/
	class MSpeedTree :	public IMaterial
	{
	public:
		MSpeedTree(void);
		~MSpeedTree(void);

		void CreateFromTree(const CSpeedTreeRT* pTree);

		//--ISerializable
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
	public:
		MConstColor	*m_pColor;
		MBitmap		*m_pDiffuse;
		MBitmap		*m_pShadow;
		MBitmap		*m_pBump;
	};
}//namespace F3D