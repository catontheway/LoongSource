#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectCylinder 
		\brief Ͳ״/��״/��״��Ч
	*/
	class Cool3D_API EffectCylinder : public EffectBase
	{
	public:
		struct tagCylinderProp : public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			int		div;			//��Բ�ָܷ���ٴ�
			Vector3 offset;			//��Path���ƫ����
			float	uScale;
			Vector3	initRot;		//��ʼ��ת

			DWORD	dwReserve[8];

			tagCylinderProp():div(12),offset(0,0,0),uScale(1),initRot(0,0,0)
			{
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
		struct tagState
		{
			float	lastTime;		//���״̬������ʱ��,������������һ״̬���в�ֵ
			float	numCircle;		//Ȧ��
			float	high;			//Ͳ�ĸ߶�,���Ŀ��(��ֱ���򳤶�)
			float	topRadius;		//���뾶
			float	bottomRadius;	//�װ뾶
			float	divTopRadiusInc;	//�ָ�ʱ���뾶�����ٶ�
			float	divBottomRadiusInc;	//�ָ�ʱ�װ뾶�����ٶ�
			float	topShrink;		//�������̶�,�������������,��â�ǵ�
			float	bottomShrink;	//�������̶�
			float	divTopRise;		//ÿ�ηָ��߶������ٶ�
			float	divBottomRise;	//ÿ�ηָ����߶������ٶ�
			Color4f	topColor;		//����������ɫ
			Color4f	bottomColor;	//�ײ�������ɫ

			Vector3	rotVel;			//��ת�ٶ�

			DWORD	dwReserve[8];

			tagState():topColor(1,1,1,1),bottomColor(1,1,1,1),rotVel(0,0,0)
			{
				lastTime=1.0f;
				numCircle=1.0f;
				high=50.0f;
				topRadius=80.0f;
				bottomRadius=40.0f;
				divTopRadiusInc=0;
				divBottomRadiusInc=0;
				topShrink=0.0f;
				bottomShrink=0.0f;
				divTopRise=0.0f;
				divBottomRise=0.0f;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectCylinder(void);
		virtual ~EffectCylinder(void);

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
		virtual float GetPlayTime();
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		//--
		//�༭�����ݵ����ӿ�
		tagCylinderProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagCylinderProp& prop);
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectCylinder);
	};
}//namespace Cool3D