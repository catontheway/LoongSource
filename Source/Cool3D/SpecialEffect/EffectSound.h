#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectSound 
		\brief ��Ч�������Ч
	*/
	class Cool3D_API EffectSound :	public EffectBase
	{
	public:
		static bool OP_bEnable;
	public:
		struct tagSoundProp : public tagProp
		{
			bool	b3DSound;		//�Ƿ�ʹ��3D��Ч
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			float	timeLoop;	//ѭ�����ʱ��

			DWORD	dwReserve[7];

			tagSoundProp()
			{
				b3DSound=true;
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));
				timeLoop = 0;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
		struct tagState
		{
			float	soundTime;	//������Ч�����˶��֮�󲥷������Ч
			bool	bLoop;		//�����Ч�Ƿ�ѭ������
			TCHAR	szSoundPath[100];
			float   minDist;    //��С����
			float   maxDist;    //������
			float   volume;     //����
			DWORD	dwReserve[5];

			tagState()
			{
				memset(this,0,sizeof(*this));
				minDist = 100.0f;
				maxDist = 5000.0f;
				volume = 1.0f;
			}
		};
	public:
		EffectSound(void);
		virtual ~EffectSound(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);
		//--EffectBase
		virtual int GetNumRenderNodes()	{	return 0;}
		virtual RenderNode* GetRenderNode(int i);
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime);
		virtual void ResetPlayState();
		virtual const TCHAR* GetName() const;
		virtual void Rename(const TCHAR* szNewName);
		virtual void SetGroupID(int id){}
		virtual bool IsClosed();
		virtual void Close(float sgNodeRunTime);
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		//--
		//�༭�����ݵ����ӿ�
		tagSoundProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagSoundProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectSound);
	};
}//namespace Cool3D