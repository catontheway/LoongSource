#pragma once
#include "..\Cool3D.h"

namespace Cool3D
{
	/** \class IUICallBack
		\brief ����ص��ӿ�
		\remark �����ڽ�������ʾ��Ϣ�ͽ���
	*/
	class Cool3D_API IUICallBack
	{
	public:
		IUICallBack(void);
		virtual ~IUICallBack(void);

		virtual void AddMsg(const TCHAR* szFormat,...) =0;
		virtual void SetProgress(int curVal,int maxVal) =0;
	};
}//namespace Cool3D