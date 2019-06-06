#pragma once
#include "rendernode.h"

namespace F3D
{
	class ResSkin;
	/**	\class SkinNode 
		\brief Skin��Ⱦ�ڵ�
		\par
		�ڴ˽ڵ�Attach��RenderSysʱ��Ҫ��SceneNode�õ�AniCtrl�ӿ�
		\par
		һ��ResSkin����ʹ��һ��SkinNod����,�ѷ���vert buffer����
	*/
	class SkinNode : public RenderNode
	{
	public:
		SkinNode(void);
		virtual ~SkinNode(void);

		virtual void Create(ResSkin *pRes) =0;
		
	};
}//namespace F3D