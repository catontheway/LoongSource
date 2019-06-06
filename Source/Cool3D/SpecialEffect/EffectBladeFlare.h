#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectBladeFlare 
		\brief ������Ч
		\remarks 
	*/
	class Cool3D_API EffectBladeFlare : public EffectBase
	{
	public:
		enum { MAX_TAGNODE_NUM = 8 };
		enum { COSTOM_TAGNODE_NUM = 2 };

		struct tagBladeFlareProp : public tagProp
		{
			float	trackTimeLen;	// ����ʱ��
			float	sampleTime;		// ������ʱ����
			Color4f color;          // ��ɫ
			int     nIndex;         // ���
			bool    bNoSmooth;      // �Ƿ�ƽ��
			bool    bCustomTagNode; // �Զ�����ص�
			BYTE    byReserve[2];   // ����
			TCHAR   szTagNode[COSTOM_TAGNODE_NUM][24];
			DWORD	dwReserve[6];

			tagBladeFlareProp() :
			trackTimeLen( 0.1f ),
			sampleTime( 0.005f ),
			color( 1.0f, 1.0f, 1.0f, 1.0f ),
			nIndex( 0 ),
			bCustomTagNode( false ),
			bNoSmooth( true )
			{
				m_bActive = true;
				memset(szTagNode,0,sizeof(szTagNode));
				memset(byReserve,0,sizeof(byReserve));
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
		
		struct tagFlareLine
		{
			Vector3	pt[MAX_TAGNODE_NUM];

			tagFlareLine()
			{
				memset(this,0,sizeof(*this));
			}
		};
	public:
		EffectBladeFlare(void);
		virtual ~EffectBladeFlare(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--EffectBase
		virtual int GetNumRenderNodes()	{	return 1;}
		virtual RenderNode* GetRenderNode(int i);
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime);
		virtual void ResetPlayState();
		virtual void Close(float sgNodeRunTime);
		virtual const TCHAR* GetName() const;
		virtual void Rename(const TCHAR* szNewName);
		virtual void CreateMtl(const tagMtlEffectDesc& desc);
		virtual void SetGroupID(int id);
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		tagBladeFlareProp GetProp() const;
		void Build(const tagBladeFlareProp& prop);

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectBladeFlare);
	};
}//namespace Cool3D