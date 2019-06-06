#pragma once
#include "..\WorldBase.h"
#include "NavResMgr.h"

namespace WorldBase
{
	class WORLD_BASE_API NavKernel
	{
	public:
		NavKernel(void);
		~NavKernel(void);

		static NavKernel *Inst();

		/** ��ʼ����Դ�ص�
			\param ��Դ���������
			\param ��Դ���Ƿ���Ҫ�߳�ͬ��
		*/
		void Init(UINT poolBytes,bool bPoolThreadSync=false);
		void Destroy();

		//--ʱ�䴦��
		/** ÿִ֡��һ��
		*/
		void Tick()				
		{
			NavResMgr::Inst()->Update();
		}

		const TCHAR* GetAppPath() const;
		const TCHAR* GetWorkPath() const;
	
	private:
		class Member;
		Member *m_p;
	};
}//namespace WorldBase