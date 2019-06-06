#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectQuad 
		\brief Ƭ״��Ч
	*/
	class Cool3D_API EffectQuad : public EffectBase
	{
	public:
		struct tagQuadProp : public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			Vector3 offset;			//��Path���ƫ����
			Vector3 initRot;	//��ʼ��ת
			BYTE	faceType;	//0��ͨƬ,1����billboard,2Ͳ��billboard
			int		tailSize;	//��β����(��Ӱ),���ָ��ʱ���¼��Quad��״̬,Alphav�𽥽���,��Ϊ��Ӱ
			float	tailInterval;//��β���ʱ��
			float	tailLife;	//��Ӱ����������,���Ϊ0,���Զ����ݲ�Ӱ�����Ͳ�Ӱ��������

			DWORD	dwReserve[8];

			tagQuadProp():offset(0,0,0),initRot(0,0,0),faceType(0),
				tailSize(1),tailInterval(0.1f),tailLife(0)
			{
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};

		struct tagState
		{
			float	lastTime;//���״̬������ʱ��,������������һ״̬���в�ֵ
			Color4f color;	//��ɫ
			Vector2 size;	//��С
			Vector3 rotVel;		//rot�仯�ٶ�

			DWORD	dwReserve[8];

			tagState()	
			{	
				memset(this,0,sizeof(*this));
				lastTime=1.0f;
				color=Color4f(1,1,1,1);
				size=Vector2(100,100);

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectQuad(void);
		virtual ~EffectQuad(void);

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
		tagQuadProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagQuadProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

		void SetOffset( const Vector3& pos );

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectQuad);
	};
}//namespace Cool3D