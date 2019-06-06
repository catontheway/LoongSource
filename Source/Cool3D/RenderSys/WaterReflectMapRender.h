#pragma once
#include "..\Cool3D.h"
#include "..\Public\MathBaseDefine.h"

namespace Cool3D
{
	class SceneNode;
	class SceneGraph;
	/** \class WaterReflectEffect
		\brief ˮ�淴��Ч��
	*/
	class Cool3D_API WaterReflectMapRender
	{
	public:
		enum EEffectLevel//Ч������
		{
			EL_Low,//�� 
			EL_Mid,//��
			EL_High,//��
		};
	public:
		static WaterReflectMapRender* Inst();
		void Init();
		void Destroy();

		bool IsHWSurpport();
		void Enable(bool bEnable);
		bool IsEnable(){ return m_bEnable;}

		void Begin(SceneGraph* pSG);
		void AttachNode(SceneNode* pSceneNode);
		void End();

		EEffectLevel GetEffectLevel();
		void SetEffectLevel(EEffectLevel level);

		bool GetRViewPrjMatrix(UINT riverID,Matrix4& mat);

	private:
		WaterReflectMapRender(void);
		virtual ~WaterReflectMapRender(void);

		class Member;
		Member* m_p;

		bool m_bEnable;
	};
}//end namespace Cool3D