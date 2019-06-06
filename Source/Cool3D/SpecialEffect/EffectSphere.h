#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectSphere 
		\brief һ����������
	*/
	class Cool3D_API EffectSphere : public EffectBase
	{
	public:
		struct tagSphereProp : public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			Vector3 offset;			//��Path���ƫ����
			Vector3 initRot;		//��ʼ��ת,�Ƕ�
			float	radius;
			Vector2	pitchRange;//�Ƕȷ�Χ(mini,max)
			Vector2	yawRange;
			int		slices;	//�ָ����
			Vector2	uvScale;	//��ͼ�ظ�����
			
			BYTE	byReserve[64];

			tagSphereProp()
			{
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));
				offset=initRot=Vector3(0,0,0);
				radius=200;
				yawRange=Vector2(0,360);
				pitchRange=Vector2(0,180);
				slices=16;
				uvScale=Vector2(1,1);

				memset(byReserve,0,sizeof(byReserve));
			}
		};

		struct tagState
		{
			float	lastTime;//���״̬������ʱ��,������������һ״̬���в�ֵ
			Color4f color;	//��ɫ
			Vector3 rotVel;	//rot�仯�ٶ�
			Vector3	sacle;
						
			BYTE	byReserve[64];

			tagState():color(1,1,1,1),rotVel(0,0,0),sacle(1,1,1)
			{
				lastTime=1.0f;

				memset(byReserve,0,sizeof(byReserve));
			}
		};

	public:
		EffectSphere(void);
		virtual ~EffectSphere(void);

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
		virtual void SetGroupID(int id);
		virtual float GetPlayTime();
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		//--
		//�༭�����ݵ����ӿ�
		tagSphereProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagSphereProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectSphere);
	};
}//namespace Cool3D