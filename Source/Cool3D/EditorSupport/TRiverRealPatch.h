#pragma once
#include ".\TerrainEditor.h"

namespace Cool3D
{
	class TRiverEditor;
	class IUICallBack;
	class AABBox;
	class BitMap;

	/** \class TRiverRealPatch
		\brief �����е�һ��������
	*/
	class TRiverRealPatch
	{
	public:
		enum ELoadState
		{
			LS_INVALID		=-1,
			LS_NOTLOAD		=0,			//δ����
			LS_LOADING,					//������
			LS_LOADED,					//�Ѽ���
		};
	public:
		TRiverRealPatch(UINT riverID,int height);
		virtual ~TRiverRealPatch();

		bool CreateRenderBuffer(TRiverEditor *pEditor,Heightmap *pHMap,const RECT *pRect);
		bool CreateMtl(const TCHAR *szTexFile,const TCHAR *szMapFile,BYTE initVal,float alpha);

		//--��Ⱦ
		void Draw();

		//--�༭
		void UpdateVB(Heightmap *pHMap);
		void BrushMtl(const tagTEditBrush& brush);
		void SmoothMtl(const tagTEditBrush& brush);
		void NoiseMtl(const tagTEditBrush& brush,float noiseFreq);

		//--����
		bool SaveToFile() const;
		//--����
		void LoadFromFile(TRiverEditor *pEditor,const TCHAR *szFileName);

		bool CacheIsLoaded() const;
		void ClearCache();
		bool Build(TRiverEditor *pEditor,const TCHAR* szSavePath,int id,Heightmap* pTerrainHmap,IUICallBack* pUI);
		void ApplyMtl(TRiverEditor *pEditor);
		void DeleteCacheFile();

		const AABBox& GetWorldBox() const;

		bool HaveWater();
		/** ����ˮ������bitmap
		*/
		bool BuildWaterBitmap(BitMap& bitmap,RECT rc,IUICallBack* pUI);

	private:
		void DrawPass(bool useMtl,int numFace);
		void FillVertStream(Heightmap* pHMap);
		bool ConvLocalMtlBrush(tagTEditBrush& brush);

	protected:
		class Member;
		Member	*m_p;
	};
}//namespace Cool3D