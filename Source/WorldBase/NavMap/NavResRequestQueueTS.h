#pragma once

class IFS;

namespace WorldBase
{
	class NavResBase;

	enum EResIOCmd
	{
		EResIO_Load,
		EResIO_Delete
	};

	struct tagResReq
	{
		EResIOCmd	cmd;
		NavResBase*	pRes;
		DWORD		param;

		tagResReq(EResIOCmd	eCmd,NavResBase* pResbase,DWORD dwParam)
		{
			cmd		= eCmd;
			pRes	= pResbase;
			param	= dwParam;	
		}

		tagResReq(const tagResReq& other)
		{
			Copy(other);
		}

		//����һ��copy���캯��
		const tagResReq& operator =(const tagResReq& other)
		{
			if(this!=&other)
				Copy(other);
			return *this;
		}

	private:
		void Copy(const tagResReq& other)
		{
			cmd		= other.cmd;
			pRes	= other.pRes;
			param	= other.param;
		}
	};

	/**	\class NavResRequestQueueTS
	\brief �̰߳�ȫ����Դ�������
	*/
	class NavResRequestQueueTS
	{
	public:
		NavResRequestQueueTS(void);
		virtual ~NavResRequestQueueTS(void);

		/**	�����������һ������
		\remarks delete����ֻ����ResPool����,�������ResPool��ͻ
		\see ResPool
		*/
		void Push(EResIOCmd cmd,NavResBase *pRes,DWORD param);
		/**	�Ӷ�����ȡ��һ������,�������Ӷ�����ɾ��
		*/
		tagResReq Pop();
		/**	�����Ƿ�Ϊ��
		*/
		bool IsEmpty();
		UINT GetSize();
		/**	ȡ���¼����������м�����Ϣʱ�����¼�������
		*/
		HANDLE	GetEvent() { return m_hEvent; }

		/**	ȡ��Push����
		*/
		UINT GetPushCount() { return m_pushCount; }
		/**	ȡ��Pop����
		*/
		UINT GetPopCount()  { return m_popCount;  }
		/** Push&Pop��������
		*/
		void ZeroPushPopCount();

		void CancleByFS(IFS* pFS);

	private:
		CRITICAL_SECTION	m_Lock;
		HANDLE				m_hEvent;

		deque<tagResReq>	m_queue;

		volatile UINT		m_pushCount;
		volatile UINT		m_popCount;
	};
}//namespace WorldBase