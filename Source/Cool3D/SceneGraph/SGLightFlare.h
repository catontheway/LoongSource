#pragma once
#include "scenenode.h"

namespace Cool3D
{
	class RVizQuery;
	class RPointSprites;
	/**	\class SGLightFlare 
		\brief �ƹ����
		\remarks ��nodeӦ��ΪSGLightNode��childʹ��;
		\par �ƹ���κ;�ͷ���β�����һ������.
	*/
	class SGLightFlare : public SceneNode
	{
	public:
		SGLightFlare(void);
		virtual ~SGLightFlare(void);

	private:
		RVizQuery		*m_pRenderQuery;
		RPointSprites	*m_pRenderNode;
	};
}//namespace Cool3D