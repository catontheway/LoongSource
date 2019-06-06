#pragma once
#include "..\Cool3D.h"

namespace Cool3D
{
	class SceneNode;
	class RenderNode;
	/** \class IRenderCallBack
		\brief ��Ⱦ���ص��ӿ�
	*/
	class Cool3D_API IRenderCallBack
	{
	public:
		IRenderCallBack(void);
		virtual ~IRenderCallBack(void);

		virtual void OnRenderBefore(SceneNode* pSGNode,RenderNode* pRenderNode) =0;
	};
}//namespace Cool3D