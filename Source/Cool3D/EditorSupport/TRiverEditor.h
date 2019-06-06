#pragma once
#include "..\Terrain\ResRiver.h"
#include ".\TerrainEditor.h"

namespace Cool3D
{
	class SceneGraph;
	class IUICallBack;
	class Heightmap;
	class BitMap;
	class TRiverPatchProxy;
	/** \class TRiverEditor
		\brief �����༭�����Ĺ���
		\remarks ʹ�����Ƶ���ˢ�ӵķ�ʽ��������
	*/
	class Cool3D_API TRiverEditor
	{
	public:
		enum
		{
			//--�༭
			MAX_RIVER=32,

			//--��������
			EPatchSize=128,				//����Patch�ж���*���ٸ�����
			EPatchAlphaMapSize=128,		//����Patch͸����ͼ�Ĵ�С�ж���*���ٸ�����
			EPatchSideVerts=17,			//����Patch�ж���*���ٸ�����
			EPatchVerts=17*17,			//����Patch�ж��ٸ�����
		};

		static UINT OP_Draw_Dist;

		struct tagRiver
		{
			UINT						id;				//����ID
			int							height;			//�����ĸ߶�
			tagRiverMtlDesc				mtlDesc;		//����������Ϣ
			Heightmap					*pHMap;			//�����߶�ͼ���������ɶ�����Ϣ
			vector<TRiverPatchProxy*>	patchs;			//�鼯��
		};

	public:
		TRiverEditor();
		virtual ~TRiverEditor();

		//--����
		bool Save() const;
		//--����
		bool Load();

		//--��Ⱦ
		void Draw(SceneGraph *pSG);

		//--
		int Build(const TCHAR* szBasePath,Heightmap* pTerrainHmap,IUICallBack* pUI);

		//--�ڵ����ϻ���һ��ԲȦ param���ĵ����������,ԲȦ�İ뾶(��λ:����)
		void DrawGroundCircle(UINT riverIndex,float centreX,float centreZ,UINT r);
		void DrawGroundQuad(UINT riverIndex,int left,int right,int top,int bottom,DWORD color);

		//--�༭����������ˢӦ���ں���ǿ��ͼ
		void ApplyBrush2River(UINT riverIndex,const tagTEditBrush& brush);
		void SmoothRiver(UINT riverIndex,const tagTEditBrush& brush);
		void NoiseRiver(UINT riverIndex,const tagTEditBrush& brush,float noiseFreq);

		//--�ⲿ����
		bool MousePick(UINT riverIndex,UINT cursorX,UINT cursorY,CameraBase *pCamera,Vector3& out);
		void SetName(const TCHAR *szMapName);
		tagRiverMtlDesc *GetRiverMtlDescByID(UINT riverID);
		tagRiverMtlDesc *GetRiverMtlDescByIndex(UINT riverIndex);
		bool RemoveRiver(UINT riverIndex);
		bool AddRiver(int riverHeight,UINT sizeW,UINT sizeH,float xScale,float zScale,float yScale);
		void ClearRivers();
		UINT GetRiverNum();
		const tagRiver& GetRiverByIndex( UINT riverIndex );
		int GetRiverHeightByIndex(UINT riverIndex);
		int GetRiverHeightByID(UINT riverID);
		Heightmap *GetRiverHeightMapByIndex(UINT riverIndex);
		Heightmap *GetRiverHeightMapByID(UINT riverID);
		bool IsRiverOverLayGround(const Vector3& vecGround);
		//--�ⲿ�ı����
		void ApplyHeight2River(UINT riverIndex,int height);
		void ApplyMtl2River(UINT riverIndex);

		/** ����ˮ������bitmap
		*/
		bool BuildWaterBitmap(BitMap& bitmap,Heightmap* pTrnHMap,IUICallBack* pUI);
		/** ����ˮ��߶�ͼ
		*/
		bool BuildWaterHMap(Heightmap& hmap,Heightmap* pTrnHMap,IUICallBack* pUI);

	private:
		void DrawLineStrip(vector<Vert_PD>& vertBuf);

		bool CheckRivers(UINT curRiverIndex,UINT curPatchIndex);

	private:
		class Member;
		Member	*m_p;
	};
}//namespace Cool3D
