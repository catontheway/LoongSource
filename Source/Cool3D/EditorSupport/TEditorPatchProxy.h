#pragma once
#include "TerrainEditor.h"

namespace Cool3D
{
	class TerrainEditor;
	class Heightmap;
	class TEditorPatch;
	class CameraBase;
	class TerrainPatchMgr;
	class IUICallBack;
	class Frustum;
	/**	\class TEditorPatchProxy
		\brief TEditorPatch�Ĵ���
		\see TEditorPatch
	*/
	class TEditorPatchProxy
	{
	public:
		TEditorPatchProxy(void);
		virtual ~TEditorPatchProxy(void);

		void Create(RECT rc,int id);

		/**	�Զ������Ƿ񴴽��������ڲ�����
			\remarks �ӵ�λ��Patch��,����Patch���ӵ�����Patch�����򴴽�real patch,
			����save real patch to disk,then free it
		*/
		void Update(TerrainEditor *pEditor,Heightmap *pHMap,const RECT& drawRect,const RECT& editRect);
		void UpdateVB(TerrainEditor* pEditor,Heightmap* pHMap,const RECT* pRc);
		void Draw(const Frustum& frustum);
		void DrawPatchEdge(TerrainEditor *pEditor);
		void DrawSubPatchEdge(TerrainEditor *pEditor);
		void AddLayerMtl(TerrainEditor *pEditor,int layerIndex,BYTE initAlpha);
		/**	\todo Remove cache???
		*/
		void RemoveLayer(const TCHAR* szLayerName);
		void ClearLayers();

		void MoveLayer(const TCHAR* szLayerName,int offset);
		void BrushLayerMtl(const TCHAR* szLayerName,const tagTEditBrush& brush);
		void SmoothLayerMtl(const TCHAR* szLayerName,const tagTEditBrush& brush);
		void NoiseLayerMtl(const TCHAR* szLayerName,const tagTEditBrush& brush,float noiseFreq);

		void SaveCache();
		bool BuildRunTimeData(TerrainEditor *pEditor,Heightmap *pHMap,const TCHAR* szBasepath,IUICallBack* pUI);

		static void SetCachePath(const TCHAR* szPath);

		/**	\����rect
		*/
		void GetMapRect(RECT& rect)
		{
			rect=m_mapRect;
		}
		/** \����û���ص���rect
		*/
		void GetMapRectNoOverlap(RECT& rect);
		/**	\ȡ��ͼ����
		*/
		void GetLayers(POINT ptMap,TerrainEditor *pEditor,Heightmap *pHMap,vector<int>& layers);
		int GetID(){ return m_id;}
		/** ��LightmapӦ�õ�Ԥ�����ͼ
		*/
		bool ApplyLightMapToCombineImage(const TCHAR* szLightMapPath,const TCHAR* szTrnPath);
		/** ��LightmapӦ�õ��ر�װ�β�
		*/
		bool ApplyLightMapToDecoLayer(const TCHAR* szLightMapPath,const TCHAR* szTrnPath,TerrainEditor* pEditor);
		TEditorPatch *GetPatch() { return m_pRealPatch; }

		/** ���ɵ����߶�ͼ
		*/
		void BuildNavHeightmap(Heightmap* pHMap,Heightmap& nav);

		/**	���ɵر�����ͼ
		*/
		bool BuildLayerTypeMap(TerrainEditor *pEditor,Heightmap *pHMap,Heightmap* pLayerType,IUICallBack* pUI);

		void SetMtlDiffuse(const Color4f& color);
		void SetMtlAmbient(const Color4f& color);
		void SetMtlSpecular(const Color4f& color);
		void SetMtlPower(float power);

	private:
		void GetRealPatch(TerrainEditor *pEditor,Heightmap *pHMap);
		bool GetMeshHeight(VertStream& vertStream,Vector3& pos);
		static tstring s_cachePath;
	private:
		RECT			m_mapRect;		//��Patch������height map��ռ�õ�Rect
		POINT			m_center;
		int				m_id;
		TEditorPatch	*m_pRealPatch;	//����������
	};
}//namespace Cool3D