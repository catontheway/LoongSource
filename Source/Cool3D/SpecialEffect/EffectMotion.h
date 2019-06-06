#pragma once
#include "effectpathctrl.h"

namespace Cool3D
{
	/**	\class EffectMotion 
		\brief ʹ���ٶ�,��ת�������˶�·������Ч·������
	*/
	class Cool3D_API EffectMotion : public EffectPathCtrl
	{
	public:
		struct tagMotionProp : public tagProp
		{
			Vector3		initPos;	//��ʼλ��
			bool        bCalcRot;   //�Ƿ���㳯��
			BYTE		byReserve[3];
			DWORD		dwReserve[7];
			
			tagMotionProp():initPos(0,0,0)
			{
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};

		struct tagState
		{
			float	lastTime;	//���״̬������ʱ��,������������һ״̬���в�ֵ
			Vector3 vel;		//�ƶ��ٶ�
			Vector3 revVel;		//��ת�ٶ�

			DWORD	dwReserve[8];

			tagState():lastTime(1),vel(0,0,0),revVel(0,0,0)
			{
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectMotion(void);
		virtual ~EffectMotion(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--EffectBase
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime);
		virtual void ResetPlayState();
		virtual const TCHAR* GetName() const;
		virtual void Rename(const TCHAR* szNewName);
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;
		
		//--EffectPathCtrl
		virtual const Vector3& GetCurPos() const;
		virtual const Vector3& GetCurVel() const;
		virtual const Vector3& GetCurRot() const;

		//--
		//�༭�����ݵ����ӿ�
		tagMotionProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagMotionProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectMotion);
	};
}//namespace Cool3D