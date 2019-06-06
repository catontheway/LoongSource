#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectCross 
		\brief ʮ�ֽ������Ƭ����
	*/
	class Cool3D_API EffectCross :	public EffectBase
	{
	public:
		struct tagCrossProp: public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			Vector3 offset;			//��Path���ƫ����
			Vector3 initRot;		//��ʼ��ת
			int		numFace;		//��������ĸ�������(���ĸ�����),0��ֻ��һ��ʮ��
			bool	bHFace;			//�Ƿ����������
			
			BYTE	byReserve[64];

			tagCrossProp()
			{
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));
				offset=initRot=Vector3(0,0,0);
				numFace=0;
				bHFace=false;

				memset(byReserve,0,sizeof(byReserve));
			}
		};

		struct tagState
		{
			float	lastTime;//���״̬������ʱ��,������������һ״̬���в�ֵ
			Color4f color;	//��ɫ
			Vector2 size;	//��С
			Vector3 rotVel;	//rot�仯�ٶ�

			float	faceDist;//ÿ����֮��Ļ������
			float	faceDistInc;//���ŵ�������,û����һ����,�������Ӷ���
			Vector2	faceSizeInc;//�ص�������,ÿ����һ����,��С���Ӷ���
			float	faceAlphaDec;//�ص�������,ÿ����һ����,͸���ȼ��ٶ���

			BYTE	byReserve[64];

			tagState()
			{
				memset(this,0,sizeof(*this));
				lastTime=1.0f;
				color=Color4f(1,1,1,1);
				size=Vector2(200,200);
				faceDist=50;
			}

		};
	public:
		EffectCross(void);
		virtual ~EffectCross(void);

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
		tagCrossProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagCrossProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectCross);
	};
}//namespace Cool3D