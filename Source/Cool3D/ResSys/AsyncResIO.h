#pragma once
#include "ResRequestQueueTS.h"

namespace Cool3D
{
	/**	\class AsyncResIO
		\brief ʵ���첽��Դ��ȡ
		\remarks 
		\par Ϊ�˽��һ����ԴLoad������Ȼ�ڶ�����,��ResPool������Ҫdelete resource���������,
		ResourcePool�����delete����ʵ��Ĳ���Ҳ�ӵ�����������Ŷ�.
	*/
	class AsyncResIO
	{
	public:
		AsyncResIO(void);
		~AsyncResIO(void);

		static AsyncResIO *Inst();
		void Create();
		void Destroy();
		void QueueRequest(EResIOCmd cmd,ResBase *pRes,DWORD param);

		void WorkerThreadFunc();
		UINT GetQueueSize();

		void CancleByFS(IFS* pFS)
		{
			m_requestQueue.CancleByFS(pFS);
		}

		UINT GetQueuePushCount();
		UINT GetQueuePopCount();
		void ZeroQueuePushPopCount();

	private:
		bool				m_bTerminate;
		ResRequestQueueTS	m_requestQueue;
		HANDLE				m_hWorkThread;
	};
}//namespace Cool3D