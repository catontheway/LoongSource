#pragma once
#include "NavResRequestQueueTS.h"

namespace WorldBase
{
	/**	\class NavAsyncResIO
		\brief ʵ���첽��Դ��ȡ
		\remarks 
		\par Ϊ�˽��һ����ԴLoad������Ȼ�ڶ�����,��ResPool������Ҫdelete resource���������,
			ResourcePool�����delete����ʵ��Ĳ���Ҳ�ӵ�����������Ŷ�.
	*/
	class NavAsyncResIO
	{
	public:
		NavAsyncResIO(void);
		~NavAsyncResIO(void);

		static NavAsyncResIO *Inst();
		void Create();
		void Destroy();
		void QueueRequest(EResIOCmd cmd,NavResBase *pRes,DWORD param);

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
		bool					m_bTerminate;
		NavResRequestQueueTS	m_requestQueue;
		HANDLE					m_hWorkThread;
	};
}//namespace WorldBase