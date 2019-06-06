#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectCamera 
		\brief ��ͷ��Ч
	*/
	class Cool3D_API EffectCamera : public EffectBase
	{
	public:
		struct tagCameraProp : public tagProp
		{
			float   attenuationFactor;  // ˥��ϵ��
			float   range;              // Ӱ�췶Χ
			DWORD	dwReserve[32];

			tagCameraProp() : attenuationFactor( 0.0f ), range( 1000.0f )
			{
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};

		struct tagState
		{
			float	lastTime;			// ���״̬������ʱ��,������������һ״̬���в�ֵ
			Vector3 swing;				// ���
			DWORD	dwReserve[8];

			tagState()	
			{	
				memset(this,0,sizeof(*this));
				lastTime=1.0f;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectCamera(void);
		virtual ~EffectCamera(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--EffectBase
		virtual int GetNumRenderNodes()	{ return 0;}
		virtual RenderNode* GetRenderNode(int i) { return NULL; }
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime);
		virtual void ResetPlayState();
		virtual const TCHAR* GetName() const;
		virtual void Rename(const TCHAR* szNewName);
		virtual void SetGroupID(int id){}
		virtual float GetPlayTime();
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		//--
		//�༭�����ݵ����ӿ�
		tagCameraProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagCameraProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectCamera);
	};
}//namespace Cool3D