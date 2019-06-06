#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectGrid 
		\brief ����Ť��
		\todo ��������
	*/
	class Cool3D_API EffectGrid : public EffectBase
	{
	public:
		struct tagGridProp : public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			int		xDiv,
					yDiv;//x*y������
			Vector3	offset;
			Vector3 initRot;

			int		faceType;//0ƽ��,1˫����,2���ι����,3Ͳ�ι����

			DWORD	dwReserve[7];

			tagGridProp():offset(0,0,0),initRot(0,0,0)
			{
				xDiv=yDiv=9;
				faceType=0;

				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));

				memset(dwReserve,0,sizeof(dwReserve));
			}

		};
		struct tagState
		{
			float	lastTime;	//���״̬������ʱ��,������������һ״̬���в�ֵ
			Color4f color;		//��ɫ
			Vector2 uvScale;	//��ͼ�ظ�����
			Vector2 size;		//��С

			Vector3 rotVel;		//��ת�ٶ�

			float	waveSpeed;	//���Ĵ����ٶ�
			Vector3	wavePower;	//����������������

			DWORD	dwReserve[8];

			tagState():color(1,1,1,1),uvScale(1,1),size(100,100),rotVel(0,0,0),
				wavePower(0,4,0)
			{
				lastTime=1.0f;
				waveSpeed=0.5f;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectGrid(void);
		virtual ~EffectGrid(void);

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
		tagGridProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagGridProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectGrid);
	};
}//namespace Cool3D