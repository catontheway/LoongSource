#pragma once
#include "effectpathctrl.h"

namespace Cool3D
{
	/**	\class EffectTarget 
		\brief ָ��Ŀ�����˶�����,�������˶�·������Ч·������
	*/
	class Cool3D_API EffectTarget : public EffectPathCtrl
	{
	public:
		struct tagTargetProp : public tagProp
		{
			int			speedType;	//0ֱ��,1cos����,2sin����
			float		time;		//����㵽�յ�ʹ�ö���ʱ��
			float		speed;		//��time<=0ʱ,ʹ��ָ�����ٶȹ���time
			Vector3		startPt;
			Vector3		endPt;
			Vector3		reservePt[4];

			int			upLineType;	//�߶���������,3�����ߣ���Ϊ������֮���ٶ����߽����������ã�
			int			sideLineType;//�ڶ���������
			float		upMax;	//���߶�
			float		sideMax;//���ڶ�����
			DWORD		dwReserve[12];

			tagTargetProp()
			{
				speedType=0;
				time=-1;
				startPt=Vector3(0,0,0);
				endPt=Vector3(1000,0,0);
				speed=200;

				upLineType=sideLineType=0;
				upMax=sideMax=200;

				memset(reservePt,0,sizeof(reservePt));
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectTarget(void);
		virtual ~EffectTarget(void);

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

		void SetProperty(const tagTargetProp& prop);
		tagTargetProp& GetProperty();
		void ResetTarget(const Vector3& startPt,const Vector3& targetPt);
		void ChangeTargetPt(const Vector3& targetPt);

		// �Ƿ񵽴��յ�
		bool IsArrive();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectTarget);
	};
}//namespace Cool3D