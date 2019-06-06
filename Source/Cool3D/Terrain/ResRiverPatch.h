#pragma once
#include "..\ResSys\ResRenderable.h"

namespace Cool3D
{
	class TRiverEditor;
	class VertStream;
	class Heightmap;
	class IVertBuffer;
	class IMaterial;
	class MWaterReflect;
	class MWaterRefract;

	/**	\class ResRiverPatch
		\brief ��������Դ
	*/
	class ResRiverPatch : public ResRenderable
	{
		struct tagHeader
		{
			TCHAR	magic[8];
			AABBox	box;
			int		patchX;
			int		patchZ;
			TCHAR	szWaterTex[256];
			TCHAR	szBumpMap[256];
			int		waterFrm;
			float	waterFps;
			Color4f blendColor;
			float	bumpScale;
			Color4f	diffuse;
			Color4f ambient;
			Color4f specular;
			float	power;
			float	reflectPower;
			int		bumpFrm;                //-- δʹ��
			UINT	numVerts;
			DWORD	vertsOffset;
			Vector2 bumpUVScroll;
			float   reflectSpecularScale;
		};
	public:
		ResRiverPatch(const TCHAR* szName);
		virtual ~ResRiverPatch();

		//--ResBase
		virtual UINT CreateContent(DWORD param);

		//--ResRenderable
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const AABBox *GetAABBox() const;
		virtual int GetNumMtl() const;
		virtual void CreateMtlArray(vector<IMaterial*>& mtls,const TCHAR* szLightMap,float mtlDiffuseFactor=1.0f) const {}

		//--���ⲿ�༭�����ã�����Build����Patch
		static bool EditorSave(TRiverEditor *pEditor,UINT riverID,const TCHAR *szVertsPath,
			const TCHAR *szAlphaPath,const RECT *pRect);
		int GetPatchX();
		int GetPatchZ();

		IVertBuffer *GetPosTexVB();
		IMaterial *CreateWaterMtl();
		MWaterReflect *CreateReflectMtl();
		MWaterRefract *CreateRefractMtl();

	private:
		static void FillVertStream(VertStream *pVS,Heightmap *pHMap,const RECT *pRect,float uTileSize,float vTileSize);

	private:
		class Member;
		Member	*m_p;

		DECL_RTTI(ResRiverPatch);
	};
}//namespace Cool3D