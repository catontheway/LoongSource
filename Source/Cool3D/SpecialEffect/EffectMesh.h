#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectMesh 
		\brief ����һ��ģ�͵Ŀռ��˶�
		\remarks 
		.ģ�ͱ�����StaticMesh
		.ģ�ͱ���ֻ��һ������,�����޷���EffectBase��Mtl�ӿ�ͳһ
	*/
	class Cool3D_API EffectMesh : public EffectBase
	{
	public:
		struct tagMeshProp : public tagProp
		{
			TCHAR		szPathName[32];	//ʹ���Ǹ�Path
			TCHAR		szMeshName[64];
			Vector3		offset;
			Vector3		initRot;

			DWORD	dwReserve[8];

			tagMeshProp():offset(0,0,0),initRot(0,0,0)
			{
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));
				memset(szMeshName,0,sizeof(szMeshName));

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};

		struct tagState
		{
			float	lastTime;	//���״̬������ʱ��,������������һ״̬���в�ֵ
			float	alpha;		//͸����
			Vector3 scale;		//��С
			Vector3 rotVel;		//��ת�ٶ�
			Vector3	vel;		//�ƶ��ٶ�,���������·��,��������·��ƫ�Ƶ��ٶ�

			DWORD	dwReserve[8];
			
			tagState()	
			{	
				memset(this,0,sizeof(*this));
				lastTime=1.0f;
				alpha=1.0f;
				scale=Vector3(1,1,1);

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectMesh(void);
		virtual ~EffectMesh(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--EffectBase
		virtual int GetNumRenderNodes()	{	return 0;}
		virtual RenderNode* GetRenderNode(int i);
		virtual void OnDetachFromSGNode(SGEffectNode *pSGNode);
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime);
		virtual void ResetPlayState();
		virtual const TCHAR* GetName() const;
		virtual void Rename(const TCHAR* szNewName);
		virtual void SetGroupID(int id);
		virtual void CreateMtl(const tagMtlEffectDesc& desc);
		virtual float GetPlayTime();
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		//--
		//�༭�����ݵ����ӿ�
		tagMeshProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagMeshProp& prop);
		void AddKeyState(const tagState& state);
		void EndBuild();
	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectMesh);
	};
}//namespace Cool3D