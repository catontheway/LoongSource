#pragma once
#include "effectbase.h"

namespace Cool3D
{
	/**	\class EffectExplosive 
		\brief Ƭ,��ը��Ч
	*/
	class Cool3D_API EffectExplosive :	public EffectBase
	{
	public:
		struct tagExpProp: public tagProp
		{
			TCHAR	szPathName[32];	//ʹ���Ǹ�Path
			Vector3 offset;			//��Path���ƫ����
			Vector3 rot;
			BYTE	faceType;		//0��ͨ��Ƭ,1����billboard,2Ͳ��billboard
			BYTE	bodyType;		//0Plane,1Sphere
			Vector4	bodyParam;		//plane(x,y��С),sphere(x=>R)
			int		xDiv,			//�зֲ���
					yDiv;
			float	powerDec;		//���ڷǶԳ���body(plane),��ը������˥��(�뿪���ĵ�Զ��ը��������),(0��1֮��,ԽС˥��Խ��)
			float	maxPower;
			float	minPower;
			float	airForce;//��������ϵ��
			float	gForce;	//����ϵ��

			Vector3	dir;		//��ը����
			Vector2	uvScale;	//��ͼ�ظ�����
			float	maxLifeTime;
			float	minLifeTime;

			BYTE	byReserve[64];
			
			tagExpProp()
			{
				_tcscpy_s(szPathName,sizeof(szPathName)/sizeof(TCHAR),_T("none"));
				offset=rot=Vector3(0,0,0);
				faceType=0;
				bodyType=0;
				bodyParam=Vector4(500,500,500,500);
				xDiv=yDiv=20;
				powerDec=1;
				maxPower=200;
				minPower=120;
				airForce=0.2f;
				gForce=-12;
				dir=Vector3(0,1,0);
				uvScale=Vector2(1,1);
				maxLifeTime=5;
				minLifeTime=2;
			}
		};
	public:
		EffectExplosive(void);
		virtual ~EffectExplosive(void);

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
		virtual bool GetBox(AABBox& out) const;
		virtual int GetLOD() const;

		//--
		void Build(const tagExpProp& prop);
		tagExpProp GetProperty() const;

	private:
		class Member;
		Member *m_p;

		DECL_RTTI_DYNC(EffectExplosive);
	};
}//namespace Cool3D