#pragma once
#include "projectshadow.h"

namespace F3D
{
	/**	\class DynaProjectShadow 
		\brief ��̬��Ͷ����Ӱ,������״����ͼ����ı�
		\remarks ��Ϊ���������������ƶ�,����Ͷ�䵽�ļ������ı�;
		��Ϊ��������ж���,������ͼҲҪ�ı�
	*/
	class DynaProjectShadow : public ProjectShadow
	{
	public:
		DynaProjectShadow(void);
		virtual ~DynaProjectShadow(void);
	};
}//namespace F3D