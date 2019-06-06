#pragma once

class IFS;

namespace Cool3D
{
	class ResBase;

	enum EResIOCmd
	{
		EResIO_Load,
		EResIO_Delete
	};

	struct tagResReq
	{
		EResIOCmd	cmd;
		ResBase*	pRes;
		DWORD		param;

		tagResReq(EResIOCmd	eCmd,ResBase* pResbase,DWORD dwParam)
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

	/**	\class ResRequestQueueTS
		\brief �̰߳�ȫ����Դ�������
	*/
	class ResRequestQueueTS
	{
	public:
		ResRequestQueueTS(void);
		virtual ~ResRequestQueueTS(void);

		/**	�����������һ������
			\remarks delete����ֻ����ResPool����,�������ResPool��ͻ
			\see ResPool
		*/
		void Push(EResIOCmd cmd,ResBase *pRes,DWORD param);
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
}//namespace Cool3D