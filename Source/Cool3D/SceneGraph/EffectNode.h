#pragma once
#include "scenenode.h"

namespace F3D
{
	/**	\class EffectNode 
		\brief ���Ӱ����Ⱦ����Ľڵ�
		\todo �ƹⵥ��ʹ��һ��class����, ȥ�����class???
	*/
	class EffectNode : public SceneNode
	{
	public:
		EffectNode(void);
		virtual ~EffectNode(void);

		DECL_RTTI(EffectNode);
	};
}//namespace F3D