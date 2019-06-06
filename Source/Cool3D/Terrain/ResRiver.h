#pragma once
#include "..\ResSys\ResRenderable.h"

namespace Cool3D
{
	class TRiverEditor;
	class IVertBuffer;
	class VertStream;
	class Heightmap;

	//--�����Ĳ���
	struct Cool3D_API tagRiverMtlDesc
	{
		TCHAR	szWaterTex[256];		//ˮ����ͼ��·��
		TCHAR	szBumpMap[256];			//��͹��ͼ��·��

		int		waterFrm;				//ˮ����ͼ�ж���֡(ÿ֡һ���ļ�)
		float	waterFps;				//ˮ����ͼ֡��

		float	uTileSize;				//���ٸ������ظ�һ��u
		float	vTileSize;				//���ٸ������ظ�һ��v

		bool	bEnableEffect;			//�������䡢���䡢�߹�Ч��
		Color4f blendColor;				//����Ч�������ɫ�ͻ������
		float	bumpScale;				//bump map����ֵ

		int		bumpFrm;				//������ͼ�ж���֡				----- δʹ��
		float	power;					//����ϵ��
		float	reflectPower;			//����Ч������ϵ��
		float	alpha;					//͸����

		Color4f	diffuse;				//���������
		Color4f ambient;				//���������
		Color4f	specular;				//�߹����

		Vector2 bumpUVScroll;           //bump map UV�Ĺ����ٶ�
		float   reflectSpecularScale;   //����Ч���߹�����
		float   underWaterFogStart;     //ˮ������ʼ�߶�
		float   underWaterFogEnd;       //ˮ��������߶�
		Vector3 underWaterFogColor;     //ˮ������ɫ
		bool	bCloseEdgeFade;			//�رձ�Ե����
		BYTE	reserve[31];			//�����ֶ�

		tagRiverMtlDesc()
		{
			memset(this,0,sizeof(tagRiverMtlDesc));
			waterFrm=1;
			waterFps=16.0f;
			bEnableEffect=true;
			blendColor=Color4f(0.5f,0,0.4f,0.4f);
			bumpScale=0.02f;
			uTileSize=64.0f;
			vTileSize=64.0f;
			alpha=1.0f;
			power=32.0f;
			reflectPower=64.0f;
			diffuse=ambient=specular=Color4f(1.0f,1.0f,1.0f,1.0f);
			bumpUVScroll=Vector2( 0.01f, 0.0f );
			reflectSpecularScale=0.2f;
		}
	};

	/**	\class ResRiver
		\brief ��������Ĺ�����Դ
	*/
	class ResRiver : public ResRenderable
	{
		struct tagHeader
		{
			TCHAR	magic[8];
			UINT	riverID;
			bool	bEnableEffect;		//�������䡢���䡢�߹�Ч��
			float	height;
			UINT	numVerts;
			DWORD	vertsOffset;
		};

	public:
		ResRiver(const TCHAR *szName);
		virtual ~ResRiver();

		//--ResBase
		virtual UINT CreateContent(DWORD param);

		//--ResRenderable
		virtual UINT GetNumRenderNode() const;
		virtual RenderNode *GetRenderNode(UINT i) const;
		virtual const AABBox *GetAABBox() const;
		virtual int GetNumMtl() const;
		virtual void CreateMtlArray(vector<IMaterial*>& mtls,const TCHAR* szLightMap,float mtlDiffuseFactor=1.0f) const {}

		//--���ⲿ�༭�����ã�����build����
		static bool EditorSave(TRiverEditor *pEditor,UINT riverID,const TCHAR *szBasePath);

		//--
		IVertBuffer *GetNormalAlphaVB();
		bool IsEnableEffect();
		float GetHeight();
		UINT GetRiverID();

	private:
		static void FillVertStream(VertStream *pVS,Heightmap *pHMap);

	private:
		class Member;
		Member *m_p;

		DECL_RTTI(ResRiver);
	};
}//namespace Cool3D