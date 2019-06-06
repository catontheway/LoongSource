#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectBeam 
		\brief �����Ч
		\remarks 
		\todo ���Լ���Noise
	*/
	class Cool3D_API EffectBeam : public EffectBase
	{
	public:
		struct tagBeamProp : public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			int		div;		//�ָ����
			float	sampleTime;	//������ʱ����
			float	divVScale;	//��ͼV����

			DWORD	dwReserve[8];

			tagBeamProp()
			{
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));

				div=64;
				sampleTime=0.1f;
				divVScale=1.0f;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
		struct tagState
		{
			float	lastTime;
			float	width;			//���
			float	lenWidthInc;	//������ų��ȵ����Ӷ�����(ÿ�ηָ��Ǽ�������)
			Color4f	headColor;		//ͷ����ɫ
			Color4f endColor;		//β����ɫ

			DWORD	dwReserve[8];

			tagState():headColor(1,1,1,1),endColor(1,1,1,1)
			{
				lastTime=1;
				width=50;
				lenWidthInc=0;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectBeam(void);
		virtual ~EffectBeam(void);

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
		tagBeamProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagBeamProp& prop);
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectBeam);
	};
}//namespace Cool3D