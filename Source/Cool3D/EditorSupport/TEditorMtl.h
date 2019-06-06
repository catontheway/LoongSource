#pragma once
#include "..\rendermtl\imaterial.h"
#include "..\RenderMtl\MConstColor.h"

namespace Cool3D
{
	class MBitmap;
	class ResDynaAlphaMap;
	/**	\class TEditorMtl 
		\brief Terrain Editor��ʹ�õĲ���
	*/
	class TEditorMtl : public IMaterial
	{
	public:
		TEditorMtl(void);
		virtual ~TEditorMtl(void);

		void LoadBaseTex(const TCHAR* szBaseTex);
		void LoadAlphaMap(const TCHAR* szPath,int size,int initVal);
		void DeleteCacheFile();
				
		virtual bool IsResourceReady() const;
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

	public:
		MBitmap			*m_pTileTex;	//ͼ����ͼ
		ResDynaAlphaMap *m_pAlphaMap;	//͸����ͼ
		MConstColor		m_color;
		int				m_uvSetIndex;

		DECL_RTTI_DYNC(TEditorMtl);
	};
}//namespace Cool3D