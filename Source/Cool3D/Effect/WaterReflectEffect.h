#pragma once
#include "..\Cool3D.h"

namespace Cool3D
{
	class SceneNode;
	class SceneGraph;
	/** \class WaterReflectEffect
		\brief ˮ�淴��Ч��
	*/
	class Cool3D_API WaterReflectEffect
	{
	public:
		enum EEffectLevel//Ч������
		{
			EL_Low,//�� 
			EL_High,//��
		};
	public:
		static WaterReflectEffect* Inst();
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

	private:
		WaterReflectEffect(void);
		virtual ~WaterReflectEffect(void);

		class Member;
		Member* m_p;

		bool m_bEnable;
	};
}//end namespace Cool3D