#pragma once
#include ".\RenderSys.h"

namespace Cool3D
{
	/**	\class SortByMtlAndZVal 
		\biref ��RenderItem����
		\remarks
			\par ����shader�л�����
			\par ������ͼ�л�����
			\par ����pixelshader�ĵ���
	*/
	class SortByMtlAndZVal
	{
		class GroupByPri;
		class GroupByTrait1;
		class GroupByTrait2;
		class Sorter;
	public:
		void Sort(vector<RenderItem>& items,const Matrix4& matView);
	};
}//namespace Cool3D