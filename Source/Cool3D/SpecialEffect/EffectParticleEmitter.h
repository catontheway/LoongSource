#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectParticleEmitter
		\brief ���ӷ�����
		\remarks 
		.����������ӷ���ʱ�ĳ�ʼ״̬,���ӷ���ʱ����״̬�ı仯�ٶ�(���Ӹ�����Щ�ٶ��Լ��ݱ�)
	*/
	class Cool3D_API EffectParticleEmitter : public EffectBase
	{
	public:
		struct tagPEMProp:public tagProp
		{
			TCHAR		szPathName[32];	//ʹ���Ǹ�Path
			int			shape;			//����������״(0��,1box,2cone,��Ӧ����λ����Ϣ),��������Ĵ������״�в���
			int			maxParticles;	//������Ӹ���
			int			initParNum;		//��ʼ��ʱ�������ٸ�����
			Vector3		pathOffset;
			int			velType;		//��ʼ���ӵ��ٶȷ��������ʽ(0���ķ���,1ָ���ٶ�)
			int			polygonType;	//0Ϊsprite,1Ϊspark like,2��ͨ��Ƭ

			float		gForce;		//����ϵ��(Ӱ���ٶ�)
			float		airForce;	//��������ϵ��(Ӱ���ٶ�)

			Vector3		initRot;	//��ʼ��ת

			bool		bParAutoFade;	//�Զ���������Alpha�������ٶ�
			bool		bToWorld;		//�����Ƿ���淢�����ƶ�(true������)

			BYTE		byReserve[30];

			tagPEMProp()
			{
				memset(szPathName,0,sizeof(szPathName));
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));

				shape=0;
				maxParticles=200;
				initParNum=0;
				pathOffset=Vector3(0,0,0);
				velType=0;
				polygonType=0;
				gForce=airForce=0;
				initRot=Vector3(0,0,0);

				bToWorld = true;
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
		struct tagState
		{
			float		lastTime;
			Vector4		shapeParam;		//(��:xΪR),(box:xyzΪsize),(cone:bottomR,topR,Height)
			Vector3		rotVel;			//��������ת�ٶ�
			float		emitSpeed;		//ÿ���Ӳ������ٸ�����

			float		parLifeTimeMax;	//���ӵ�����ʱ��
			Vector3		parVelDir;		//�ٶȵķ���
			float		parSpeedMax;	//���ӵĳ�����
			float		parSpeedMin;

			float		parSwingMax,	//���ӵ�ҡ��
						parSwingMin;

			Vector3		parRotSpeedMax,	//���ӵ���ת.��Ϊ���ӵ�Quad���������������,������תֻ��һ������
						parRotSpeedMin;
		
			float		parSizeMax;		//���ӳ�ʼ��С,���ֵ
			float		parSizeMin;		//���ӳ�ʼ��С,��Сֵ,ÿ�β���������max~min֮�����
			float		parSizeVel;		//���Ӵ�С�ı仯�ٶ�
			
			Color4f		parColor;		//���ӵĳ�ʼ��ɫ
			Color4f		parColorVel;	//���ӵ���ɫ�仯�ٶ�

			float		parLifeTimeMin;
			DWORD	dwReserve[7];

			tagState():lastTime(1),shapeParam(10,0,0,0),rotVel(0,0,0),emitSpeed(10),
				parLifeTimeMax(6),parLifeTimeMin(6),
				parVelDir(0,1,0),parSpeedMax(8),parSpeedMin(8),
				parSwingMax(0),parSwingMin(0),
				parRotSpeedMax(0,0,0),parRotSpeedMin(0,0,0),
				parSizeMax(20),parSizeMin(20),parSizeVel(0),
				parColor(1,1,1,1),parColorVel(0,0,0,0)
			{
				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectParticleEmitter(void);
		virtual ~EffectParticleEmitter(void);

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
		virtual bool IsClosed();
		virtual void SetGroupID(int id);
		virtual float GetPlayTime();
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		void OnSetWorldMat( const Matrix4& matWorld );
		//--
		//�༭�����ݵ����ӿ�
		tagPEMProp GetProp() const;
		int GetNumStates() const;
		tagState GetState(int i) const;
		//�����ӿ�
		void BeginBuild(const tagPEMProp& prop);	//��������
		void AddKeyState(const tagState& state);
		void EndBuild();

		void SetPathOffset(const Vector3& pos);

		static bool s_bLimitMaxParticleNum;//�Ƿ����������������
	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectParticleEmitter);
	};
}//namespace Cool3D