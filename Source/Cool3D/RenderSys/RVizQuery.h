#pragma once
#include "rendernode.h"

namespace Cool3D
{
	/**	\class RVizQuery 
		\brief ����ָ����RenderNodeִ��Device�ϵ�VizQuery
		\see IDeviceVizQuery
	*/
	class RVizQuery : public RenderNode
	{
	public:
		RVizQuery(void);
		virtual ~RVizQuery(void);
	};
}//namespace Cool3D