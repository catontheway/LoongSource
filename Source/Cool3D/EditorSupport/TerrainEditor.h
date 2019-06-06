#pragma once
#include "..\Cool3D.h"
#include "..\Math\Math.h"
#include "..\Math\Color.h"
#include "..\Public\VertexDefine.h"
#include ".\IUICallBack.h"

namespace Cool3D
{
	class Noise;
	class CameraBase;
	class Image;
	class ResRenderable;
	class SceneGraph;
	class Heightmap;
	class BitMap;
	class VertStream;
	class TRiverEditor;
	class EditorDecoFile;
	
	/**	\struct TMapLayer
		\brief terrain map layer
	*/
	struct Cool3D_API tagTMapLayer
	{
		TCHAR	szName[64];			//����
		TCHAR	szTexFile[256];		//ͼ����ͼ����
		BYTE	uvSetIndex;			//ʹ������UV����
		BYTE	surfaceType;		//�ر�����

		BYTE	reserve[63];
		tagTMapLayer()	
		{	
			memset(this,0,sizeof(*this));
		}
	};

	struct Cool3D_API tagTDecoLayer
	{
		TCHAR			szName[64];		//����,Ũ��ͼ��Ӧ�����ļ�"name.deco",Ũ��ͼ��ÿ�����ش�����������м���װ����
		float			fadeOut;		//�ھ���eyeһ����Χ����ʧ,�����Ż�

		int				type;			//0��
		TCHAR			szFileName[256];//װ����·��[��,as texture]
		Vector3			size;			//��С[��x,y]
		float			sizeRandRate;	//��С�������,��׼��С�ı���(0~n)
		BYTE			alphaRef;		//[��,alpha mask]
		Vector3			rotateRand;		//pitch,yaw,yoll��ת���
		float			wave;			//[�ݣ����]
		float			speed;			//[�ݣ�����]
		EVertType		vertType;
		int				numVert;

		Color4f			diffuse;		//���������

		DWORD			dwReserve[54];

		tagTDecoLayer()
		{
			memset(this,0,sizeof(*this));
			fadeOut=128*50;
			size.x=40;
			size.y=60;
			size.z=0;
			alphaRef=244;
			rotateRand.y=90;
			wave=20.0f;
			speed=0.8f;
			vertType=EVType_PT2;
			numVert=0;
			diffuse=Color4f(1.0f,1.0f,1.0f,1.0f);
		}
	};

	enum Cool3D_API ELerpType
	{
		ELerp_None=0,		//�޲�ֵ
		ELerp_Linear,	//���Բ�ֵ
		ELerp_Cosine,	//cos��ֵ
		ELerp_Quadratic,//���β�ֵ
	};
	enum Cool3D_API EShapeType
	{
		EShape_Circle=0,	//Բ�α�ˢ
		EShape_Quad,		//���α�ˢ��������������Quad
		EShape_Line,		//���ͱ�ˢ��
		EShape_Plane,		//ƽ����ˢ
		EShape_TypeDef,		//�Զ����ı��Σ����ڱ༭�ı��������б��
	};
	struct Cool3D_API tagTEditBrush
	{
		int			centerX,
					centerZ;//���ĵ���θ�������
		int			inner,	//��ˢ�ھ���С
					outer;	//��ˢ�⾶��С
		EShapeType	shape;
		ELerpType	lerp;
		int			strength;
	};

	/**	\class TerrainEditor
		\brief ���α༭�����Ĺ���
	*/
	class Cool3D_API TerrainEditor
	{
	public:
		enum TE_CONST
		{
			//--�༭
			MAX_MAP_LAYER=32,	//����ж��ٸ�ͼ��
			MAX_DECO_LAYER=16,

			//--��������
			EPatchAlphaMapSize=256,		//����Patch��ͼ�Ĵ�С�ж���*���ٸ�����
			EPatchSize=128,				//����Patch�ж���*���ٸ�����,(ETextureSize��Ӧ����*���ٸ����ӵĵ���)
			EPatchSideVerts=33,			//����Patch�ж���*���ٸ�����
			EPatchVerts=33*33,			//����Patch�ж��ٸ�����
			EPatchSideGridsLod0=16,		//����Patch Lod0�ж���*���ٸ�Grid
			ESimplePatchSize=512,		//����Զ��Patch�ж���*���ٸ�����
			ESimplePatchSideVerts=33,	//����Զ��Patch�ж���*���ٸ�����
			ESimplePatchVerts=33*33,	//����Զ��Patch�ж��ٸ�����
		};
		enum EEditFocus//��ǰ�༭�Ľ�������,������ǽ������ݾͿ��Լ����ڲ����µļ���
		{
			EFocus_HeightMap =0,
			EFocus_Layer,
			EFocus_Deco
		};

		static int OP_Draw_Dist;
		static int OP_Edit_Dist;
		static int OP_Deco_Draw_Size;
	public:
		TerrainEditor(void);
		virtual ~TerrainEditor(void);

		/**	���õ�ǰ���ε�����,�����ڴ���֮ǰ����
			\remarks �༭����ʹ�����������Ϊ�༭��Ϣ����·��;
			������Ϊ����ʱ������
		*/
		void SetName(const TCHAR* szName);
		const TCHAR* GetName();
		void SetDrawOption(bool bWireFrame,bool showDecoLayer);
		void GetDrawOption(bool& bWireFrame,bool& showDecoLayer);
		
		void SetEditFocus(EEditFocus focus);
		/**	�����������Ϊ���ĵľ��η�Χ�������и���
		*/
		void Draw(SceneGraph *pSG);
		void DrawPatchEdge();
		void DrawSubPatchEdge();
		/**	�ڵ����ϻ���һ��ԲȦ
			\param,���ĵ����������,ԲȦ�İ뾶(��λ:����)
		*/
		void DrawGroundCircle(float centreX,float centreZ,UINT r);
		void DrawGroundQuad(int left,int right,int top,int bottom,DWORD color);
		void DrawGroundLine(int sx,int sy,int length,float angle=0.0f,DWORD color=0xFFFFFF00);
		void DrawGroundLine(int sx,int sy,int dx,int dy,DWORD color=0xFFFFFF00);

		/**	����߶�ͼ
		*/
		bool ImportHeightMap(const TCHAR* szFileName,float xScale,float zScale,float yScale,Vector2 layerUVScale);
		bool ExportHeightMap(const TCHAR* szFileName);
		void CreateByNoise(float freq,UINT width,UINT height,float xScale,float zScale,float yScale,Vector2 layerUVScale);
		void Create(BYTE initVal,UINT width,UINT height,float xScale,float zScale,float yScale,Vector2 layerUVScale);
		//!	�༭���޸�Terrain�Ļ�������,��Ҫ�༭������,rebuild������Ч
		void ModAtt(float xScale,float zScale,float yScale);
		/**	�����߶�ͼ
		*/
		void ExportHeightMap(Image *pImg);

		bool AddLayer(const TCHAR* szLayerName,const TCHAR* szTileFileName,BYTE uvSetIndex,BYTE surfaceType);
		/**	ɾ��һ��ͼ��
			\return �Ƿ��ҵ�,��ɾ����
		*/
		bool RemoveLayer(const TCHAR* szLayerName);
		/**	ɾ������ͼ��*/
		void ClearLayers();
		const tagTMapLayer *GetMapLayer(UINT index) const;
		tagTMapLayer* FindMapLayer(const TCHAR* szLayerName);
		/**	��ָ����layer��Layer���������ϻ��������ƶ�
		*/
		void MoveLayer(const TCHAR* szLayerName,int offset);
		void ApplyBrush2Layer(const tagTEditBrush& brush,const TCHAR* szLayerName);
		void SmoothLayer(const tagTEditBrush& brush,const TCHAR* szLayerName);
		void NoiseLayer(const tagTEditBrush& brush,float noiseFreq,const TCHAR* szLayerName);
		void ApplyBrush2HMap(const tagTEditBrush& brush);
		void FlattenHMap(const tagTEditBrush& brush);
		void SmoothHMap(const tagTEditBrush& brush);
		void NoiseHMap(const tagTEditBrush& brush,float noiseFreq);
		void ApplyValue2HMap(const tagTEditBrush& brush);
		void ApplyBrush2River(const tagTEditBrush& brush,UINT riverIndex);
		void SmoothRiver(const tagTEditBrush& brush,UINT riverIndex);
		void NoiseRiver(const tagTEditBrush& brush,float noiseFreq,UINT riverIndex);

		//!	����һ��װ�β�
		bool AddDecoLayer(const TCHAR * szLayerName);
		//!	ɾ��һ��װ�β�
		bool RemoveDecoLayer(const TCHAR * szLayerName);
		//! ����װ�β�����,�����ⲿ�༭�޸�
		tagTDecoLayer* GetDecoLayer(const TCHAR * szLayerName);
		tagTDecoLayer* GetDecoLayer(int index);
		void PostEditChangeDecoLayer(const TCHAR * szLayerName,bool bForceRebuild);
		//!	�޸�װ������Ũ��ͼ
		void BrushDecoDensityMap(const TCHAR* szLayerName,tagTEditBrush& brush,CameraBase *pCamera);
		void NoiseDecoDensityMap(const TCHAR* szLayerName,const tagTEditBrush& brush,float noiseFreq,CameraBase *pCamera);
		//!	ֱ�ӵ���һ���Ҷ�ͼ������Ũ��ͼ
		void ImportDensityMap(const TCHAR* szLayerName,const TCHAR* szImgPath,CameraBase *pCamera);
		//!	ˢ������װ�β�Ԥ��
		void RefreshDecoView(CameraBase *pCamera);
		//!	�õ�Ũ��ͼ
		Heightmap *GetDecoDensityMap(const TCHAR * szLayerName);
		//! �õ�װ�β��ļ�
		EditorDecoFile *GetDecoFile(const TCHAR *szLayerName);
		//! �õ�װ�β��VB IB
		bool GetDecoVBAndIB(const TCHAR * szLayerName,const RECT& rect,
			VertStream * pVertStream, vector<WORD>& indexArray);
		
		bool LoadInfo();
		/**	ʹ��name����
			\see SetName()
		*/
		bool SaveInfo() const;
		/**	����RunTime��Ⱦ��Ҫ�ĵ�������
			\remarks �Զ����̵� basepath\\myname\\*.*
			\par ��ʹ���൱·��
			\see SetName()
		*/
		bool BuildTerrain(const TCHAR* szBasePath,IUICallBack* pUI);
		/** ������������
		*/
		bool BuildRiver(const TCHAR* szBasePath,IUICallBack* pUI);
		/** �Ѻ�����ĳЩ����д�뵽���ο���
		*/
		bool WriteRiverDataToTerrainPatch(const TCHAR* szTerrainBasePath,IUICallBack* pUI);

		bool MousePick(UINT cursorX,UINT cursorY,CameraBase *pCamera,Vector3& out);

		void SetLayerUVScale(BYTE uvSetIndex,Vector2 uvScale);
		Vector2 GetLayerUVScale(BYTE uvSetIndex) const;
		bool IsHeightmapCreated() const;
		POINT World2Tile(const Vector3& wpt);
		UINT GetHMapWidth();
		UINT GetHMapHeight();
		BYTE GetHMapValue(UINT hmapX,UINT hmapY);
		float GetHMapYScale();
		float GetHMapXScale();
		float GetHMapZScale();
		Vector2 GetStartPos();

		void SetDetailMap(const TCHAR* szPath);
		const TCHAR* GetDetailMap() const;
		/*ʹ�õ�ǰ����������������������*/
		void ResetNoise();
		
		const Heightmap *GetHeightmap() const;
		Heightmap *GetHeightmapVar();

		void NewRiver(int riverHeight);
		void DelRiver(UINT riverIndex);
		int GetRiverNum();
		TRiverEditor *GetRiverEditor();
		//--�ж�ĳ��ر��Ƿ񱻺�������
		bool IsRiverOverLayGround(Vector3& vecGround);

		void DrawLineStrip(vector<Vert_PD>& vertBuf);

		/**	���ָ�������ڵ�Patchʹ�õ���ͼ����
		*/		
		void GetLayers(Vector3& pos,vector<int>& layers);
		//! ģ��BlendͼƬ
		bool BlurBlendImg(const TCHAR* szBasePath);

		//! ָ����ˢ����ʱ�Ƿ���ʾ����
		void SetGroundRole(bool bVal);
		void DrawGroundRole(float &x, float &y, float& z);

		/** ��LightmapӦ�õ��ر�Ԥ�����ͼ
		*/
		bool ApplyLightMapToCombineImage(const TCHAR* szLightMapPath,const TCHAR* szTrnPath,IUICallBack* pUI);
		/** ��LightmapӦ�õ��ر�װ�β�
		*/
		bool ApplyLightMapToDecoLayer(const TCHAR* szLightMapPath,const TCHAR* szTrnPath,IUICallBack* pUI);

		/**	��ֹ��¼��ˢ��������ʱֻ֧����Ը߶�ͼ�Ĳ���
		*/
		void EndBrushRecord();

		/** ִ��һ�λ�ˢ�ĳ������ܣ���ʱֻ֧����Ը߶�ͼ�Ĳ���
		*/
		void UndoBrush();
		/** ִ��һ�λ�ˢ���������ܣ���ʱֻ֧����Ը߶�ͼ�Ĳ���
		*/
		void RedoBrush();
		/** �Ƿ����ִ�л�ˢ�ĳ�������
		*/
		bool GetUndoBrushEnable();
		/** �Ƿ����ִ�л�ˢ����������
		*/
		bool GetRedoBrushEnable();

		/** ���ɵ����߶�ͼ
		*/
		void BuildNavHeightmap(Heightmap& nav,IUICallBack* pUI);
		/** ����ˮ������bitmap
		*/
		bool BuildWaterBitmap(BitMap& bitmap,IUICallBack* pUI);
		/** ����ˮ��߶�ͼ
		*/
		bool BuildWaterHMap(Heightmap& hmap,IUICallBack* pUI);

		/**	���ɵر�����ͼ
		*/
		void BuildGroundTypeMap(Heightmap& layerType,IUICallBack* pUI);

		//--���õر����
		void SetMtlDiffuse(const Color4f& mtlColor);
		void SetMtlAmbient(const Color4f& mtlColor);
		void SetMtlSpecular(const Color4f& mtlColor);
		void SetMtlPower(float power);
		//--��õر����
		const Color4f& GetMtlDiffuse();
		const Color4f& GetMtlAmbient();
		const Color4f& GetMtlSpecular();
		float GetMtlPower();

	private:
		void DrawTerrain(SceneGraph *pSG);
		void DrawRiver(SceneGraph *pSG);
		void DrawDeco(SceneGraph *pSG);

	protected:
		class Member;
		Member *m_p;
	};

	class SGTrnDecoPatch;
	SGTrnDecoPatch *NewTrnDecoNode(int type);
}//namespace Cool3D
