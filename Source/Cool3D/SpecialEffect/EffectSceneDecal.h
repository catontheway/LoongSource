#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class		EffectSceneDecal 
		\brief		��������
		\remarks	
	*/
	class Cool3D_API EffectSceneDecal : public EffectBase
	{
	public:
		struct tagDecalProp : public tagProp
		{
			TCHAR	 szPathName[32];	   // ʹ���Ǹ�Path
			Vector3  offset;			   // ��Path���ƫ����
			float    initRot;	           // ��ʼ��ת
			BOOL     bUseDefaultMesh;	   // ��û������DecalMeshʱʹ��Ĭ��Mesh
			BOOL     bOnlyUseDefaultMesh;  // ֻʹ��Ĭ��Mesh
			float    rangeFront;           // ������Χ��ǰ����
			float    rangeBack;            // ������Χ���󷽣�
			DWORD	 dwReserve[8];

			tagDecalProp()
			{
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));

				offset = Vector3( 0.0f, 0.0f, 0.0f );
				initRot = 0.0f;
				bUseDefaultMesh = 0;
				bOnlyUseDefaultMesh = 0;
				rangeFront = 50.0f;
				rangeBack = 50.0f;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};

		struct tagState
		{
			float	lastTime;	// ���״̬������ʱ��,������������һ״̬���в�ֵ
			Color4f color;		// ��ɫ
			Vector2 size;		// ��С
			float	rotVel;		// rot�仯�ٶ�

			DWORD	dwReserve[8];

			tagState()	
			{	
				memset(this,0,sizeof(*this));
				lastTime=1.0f;
				color=Color4f(1,1,1,1);
				size=Vector2(200,200);
				
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectSceneDecal(void);
		virtual ~EffectSceneDecal(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--EffectBase
		virtual int GetNumRenderNodes()	{	return 1;}
		virtual RenderNode* GetRenderNode(int i);
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime);
		virtual void ResetPlayState();
		virtual const TCHAR* GetName() const;
		virtual void Rename(const TCHAR* szNewName);
		virtual void CreateMtl(const tagMtlEffectDesc& desc);
		virtual void SetGroupID(int id);
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		// �ֶ���������λ�ã�Ĭ��Ϊ��Ч�ڵ��λ��+offset
		void SetUserPos(const Vector3& pos);
		// ������������Ĭ��ΪY�᷽��
		void SetDir( const Vector3& vDir, const Vector3& vUpVec );
		// ���������С
		const Vector2& GetDecalSize() const;
		float GetFrontRange() const;
		float GetBackRange() const;

		//�༭�����ݵ����ӿ�
		tagDecalProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagDecalProp& prop);
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectSceneDecal);
	};
}//namespace Cool3D