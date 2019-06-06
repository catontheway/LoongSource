#pragma once
#include "terraineditor.h"
#include "TMapLayerExt.h"
#include "..\renderMtl\MConstColor.h"
#include "..\Terrain\TerrainRes.h"
#include "..\Terrain\TerrainPatchRes.h"
#include "..\Terrain\TerrainPatchRenderGroupsRes.h"

namespace Cool3D
{
	class TMtlEditor;
	class Heightmap;
	class VertStream;
	class IIndexBuffer;
	class IVertBuffer;
	class BitMap;
	class IUICallBack;
	
	/**	\class TEditorPatch
		\brief ���α༭���е�һ�����ο�
	*/
	class TEditorPatch
	{
		typedef TerrainPatchRenderGroupsRes::tagRenderGroup RENDER_GROUP;
	public:
		TEditorPatch(void);
		virtual ~TEditorPatch(void);

		void CreateRenderBuffer(TerrainEditor *pEditor,Heightmap *pHMap,RECT rc);
		void AddLayer(const tagTMapLayer *pLayer,int layerIndex,BYTE initAlpha,const TCHAR* szCachePath,
			const Color4f& diffuse,const Color4f& ambient,const Color4f& specular);
		void RemoveLayer(const TCHAR* szLayerName);
		void ClearLayers();

		void Draw();

		/**	����vert buffer������,�ⲿ�༭height map����Ҫ����
		*/
		void UpdateVB(TerrainEditor *pEditor,Heightmap *pHMap);
		void BrushLayerMtl(const TCHAR* szLayerName,const tagTEditBrush& brush);
		void SmoothLayerMtl(const TCHAR* szLayerName,const tagTEditBrush& brush);
		void NoiseLayerMtl(const TCHAR* szLayerName,const tagTEditBrush& brush,float noiseFreq);
		void MoveLayer(const TCHAR* szLayerName,int offset);
		
		bool IsCacheLoaded() { return m_cacheState==2;}
		void LoadCache(TerrainEditor* pEditor,const TCHAR* szPath);
		void SaveToCacheFile();
		void ClearCache();
		bool BuildRunTimeData(TerrainEditor *pEditor,Heightmap *pHMap,const TCHAR* szSavePath,int id,IUICallBack* pUI);
		bool BuildLayerTypeMap(Heightmap* pLayerType,IUICallBack* pUI);
		/**	����ͼ����
		*/
		void GetLayers(POINT ptMap,vector<int>& layers);
		const AABBox& GetWorldBox() const { return m_worldBox; }

		/** \����û���ص���rect
		*/
		void GetMapRectNoOverlap(RECT& rect);

		void SetMtlDiffuse(const Color4f& color);
		void SetMtlAmbient(const Color4f& color);
		void SetMtlSpecular(const Color4f& color);
		void SetMtlPower(float power);

	private:
		tstring GetMtlCacheName(TMtlEditor *pMtl,const TCHAR* szBasePath);
		tstring GetMtlCacheName(const TCHAR* szMtlName,const TCHAR* szBasePath);
		/**	��brush�еĵ��θ�������,ת��Ϊ������ͼ��������
			\return brush�Ƿ�Ӱ�챾patch����ͼ
		*/
		bool ConvLoaclMtlBrush(tagTEditBrush& brush);
		bool CheckLayers(const TCHAR* szLayerName,const tagTEditBrush& localBrush);
		void FreeRenderBuffer();
		void FreeMtls();
		void DrawLayerPass(int nLayer,bool useMtl,int numFace);//!ÿ��layer��Ҫһ��pass
		TMtlEditor *FindMtlByName(const TCHAR* szName);

		void BuildRenderGroups(const TCHAR* szSavepath,int id,TerrainEditor* pEditor);
		void _BuildRenderGroups(map<string,RENDER_GROUP>& renderGroups);
		void _BuildRenderGroupsAlphaMap(map<string,RENDER_GROUP>& renderGroups,const TCHAR* szSavepath,int id);
		void _OptimizeShaderSwitch(map<string,RENDER_GROUP>& src,list<RENDER_GROUP> dst[4]);
		void _OptimizeTextureSwitch(list<RENDER_GROUP>& src,vector<RENDER_GROUP>& dst);
		int _CompareRenderGroupByTex(const RENDER_GROUP& group1,const RENDER_GROUP& group2);
		bool _IsLayerPixelVisible(int layerIndex,int x,int z);
		bool _IsLayerGridVisible(int layerIndex,RECT& rc);

		void BuildPrecombine(const TCHAR* szFilename);
		void CalSurfaceType(Heightmap *pTypeMap);
	private:
		TMtlEditor		*m_mtls[TerrainEditor::MAX_MAP_LAYER];
		//--
		RECT			m_rect;
		VertStream		*m_pVertStream;	//��height mapָ����rect������vert stream
		IVertBuffer		*m_pVB;			//��Ⱦ�õ�Vert buffer
		int				m_cacheState;	//0δ����,1������,2�������
		static MConstColor	s_color;
		AABBox			m_worldBox;
		Vector2			m_layerUVScale[TRN_MAX_UV_SET];
	};
}//namespace Cool3D