//-----------------------------------------------------------------------------
/*	file: simple_task
	auth: Lyp
	date: 2003/1/1
	last: 2004/3/1
	desc: GT System	��������� 1.1

	history: 
	1.1: 
	ʹ�����µ�ģ�����(your_class),�����಻����ҪCallTask��Ա,�ṹ������

	1.0:
	����ǰ������˼����ģ����ʵ��,ʹ�ø�����
*/
//-----------------------------------------------------------------------------
#pragma once
#include "simple_map.h"

namespace vEngine{

//-----------------------------------------------------------------------------
// ���������
//-----------------------------------------------------------------------------
template<typename TaskAddin, typename your_class>
class TSimpleTask
{
/*	�ӿ�:

	// �������񻺳�
	BOOL CreateTaskBuffer(INT nTaskNum, INT nSubTaskNum);

	�����������ͽ�����Ϣ���������Ƿ��������Զ�ɾ����
	VOID EndAllTask(BOOL bSetAutoDel);
	
	FLOAT GetCurTime();
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
public:

	// ���ֽӿ��������̺߳���
	typedef DWORD (your_class::* your_fpTask)(DWORD);
	enum ETaskState	// ����״̬
	{
		ETS_Null = 0,
		ETS_Paused,
		ETS_Ready,
		ETS_FirstRun,
		ETS_Running,
		ETS_Finished,
		ETS_End
	};
	
	struct tag4DWORD
	{
		union{ INT n1; DWORD dw1; FLOAT	f1;	};
		union{ INT n2; DWORD dw2; FLOAT f2; };
		union{ INT n3; DWORD dw3; FLOAT f3; };
		union{ INT n4; DWORD dw4; FLOAT f4; };
	};
	
	template<typename your_class>	// your_fpTaskʵ��������Ҫģ��ָ�����͵�
	struct tagSubTask
	{
		your_fpTask	fp;				// ��������ĺ���ָ��
		ETaskState	eStats;			// ��ǰ״̬
		
		DWORD		dwID;
		FLOAT		fStartTime;		// ��������ʼ��ʱ��
		FLOAT		fWaitTime;		// ������ȴ���һ�����ʱ��
		FLOAT		fExpectTime;	// Ҫ�������ִ��ʱ��
		BOOL		bExpectTimeout;	// �Ƿ�Ҫ��ִ�е�ʱ���Ѿ�����
		tag4DWORD	dw4Param;		// �������
		tag4DWORD	dw4Temp;		// ����ʱʹ�õĲ���
		DWORD		dwRet;			// �����񷵻�ֵ
		
		BOOL		bValid;			// ���������Ƿ����
		BOOL		bRequireToEnd;	// �Ƿ��ⲿ��Ҫǿ����ֹ
	};
	
	// ������ṹ
	template<typename TaskAddin, typename your_class>
	struct tagTask
	{
		tagSubTask<your_class>*	pSubTask;	// ָ��������
		BOOL		bValid;
		DWORD		dwID;					// ����ID
		ETaskState	eStats;					// �������״̬
		INT			nSubTask;				// ��Ч�����������
		DWORD		dwRet;					// ���������շ��ز���
		TaskAddin	addin;					// ���񸽼�����
		BOOL		bAutoDel;				// ��ɺ��Ƿ��Զ�����
		CHAR		szName[256];			// ��������
	};


//-----------------------------------------------------------------------------
// �����ӿ�
//-----------------------------------------------------------------------------
	
	VOID Update(FLOAT fDeltaTime)
	{
		// ����ʱ�����
		m_fDeltaTime = fDeltaTime;
		m_fCurTime += fDeltaTime;

		DoTask(0);
	}

	// �������񻺳�
	BOOL CreateTaskBuffer(INT nTaskNum, INT nSubTaskNum)
	{
		ASSERT(nTaskNum>0 && nSubTaskNum>0);

		// ������
		m_pTask = new tagTask<TaskAddin, your_class>[nTaskNum];
		if( !m_pTask ) 
			return FALSE;
		
		// ������
		for( INT n=0; n<nTaskNum; n++ )
		{
			m_pTask[n].pSubTask = new tagSubTask<your_class>[nSubTaskNum];
			if( !m_pTask[n].pSubTask )
			{
				if( n>0 )
				{
					for( INT m=n-1; m>=0; m-- ) 
						delete[](m_pTask[m].pSubTask);
				}
				delete[](m_pTask);
				return FALSE;
			}
		}

		// �ڴ˴���¼������������������������
		m_nMaxTask = nTaskNum;
		m_nMaxSubTask = nSubTaskNum;

		// �������
		for( n=0; n<m_nMaxTask; n++ )
		{
			m_pTask[n].bValid = FALSE;
			for( INT m=0; m<m_nMaxSubTask; m++ )
				m_pTask[n].pSubTask[m].bValid = FALSE;
		}
		return TRUE;
	}


	//-----------------------------------------------------------------------------
	// ��������, ���ص����������
	//-----------------------------------------------------------------------------
	BOOL AddTask(LPCSTR szTaskName, TaskAddin* pAddin, BOOL bAutoDel)
	{
		ASSERT(m_nTask>=0 && m_nTask<m_nMaxTask);
		DWORD dwTaskNameCrc = m_pUtil->Crc32(szTaskName);

		// ����Ƿ�������������
		if( TRUE == m_mapTaskIndex.IsExist(dwTaskNameCrc) )
		{
			ASSERT(0);
			return FALSE;
		}

		for( INT n=0; n<m_nMaxTask; n++ )
		{
			if( !m_pTask[n].bValid ) // �����񲻴���, ���Բ����
			{
				ASSERT(szTaskName);
				ASSERT(strlen(szTaskName) < 255);

				m_pTask[n].eStats = ETS_Ready;
				m_pTask[n].bValid = TRUE;
				m_pTask[n].bAutoDel = bAutoDel;	// ��ɺ��Ƿ��Զ�ɾ��
				m_pTask[n].nSubTask = 0;	// ������Ϊ��
				m_pTask[n].dwRet = 0;
				strcpy(m_pTask[n].szName, szTaskName);
				
				// ��¼���ֺ���ŵĶ�Ӧ��ϵ
				m_mapTaskIndex.Add(dwTaskNameCrc, n);
				
				// ��������
				if( NULL != pAddin )
					memcpy(&m_pTask[n].addin, pAddin, sizeof(TaskAddin));
				else
					ZeroMemory(&m_pTask[n].addin, sizeof(TaskAddin));
				
				m_nTask++;
				return n;
			}
		}
		ASSERT(0); 
		return GT_INVALID;
	}


	//-----------------------------------------------------------------------------
	// ����һ��������, �Զ��ҵ����Էŵ�λ��"˳��"����
	// ��ʵ������������˳��ִ�У��������м�����Ч������ʱ��ֹͣ������Ķ�����
	//-----------------------------------------------------------------------------
	INT AddSubTask(LPCSTR szTaskName, your_fpTask fp, tag4DWORD* pParam, 
				   FLOAT fWaitTime, FLOAT fExpectTime)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		if( GT_INVALID == nTaskIndex )
		{
			ASSERT(0);
			return GT_INVALID;
		}

		ASSERT( m_nTask>=0 && m_nTask<=m_nMaxTask );
		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );	// ȷ��ָ�����������
		ASSERT( fp!=NULL );
		
		for( INT n=0; n<m_nMaxSubTask; n++ )
		{
			if( !m_pTask[nTaskIndex].pSubTask[n].bValid ) // �ҵ��ɷŵĵط�
			{
				tagSubTask<your_class> &sub = m_pTask[nTaskIndex].pSubTask[n];
				ZeroMemory( &sub, sizeof(sub) );
				
				sub.bValid = TRUE;
				sub.fp = fp;
				sub.eStats = ETS_Ready;
				sub.fWaitTime = fWaitTime;
				sub.fStartTime = 0.0f;
				sub.fExpectTime = fExpectTime;
				sub.bExpectTimeout = FALSE;
				
				if( pParam )	// �Ƿ���Ҫ��������
					memcpy(&sub.dw4Param, pParam, sizeof(sub.dw4Param));
				
				m_pTask[nTaskIndex].nSubTask++;
				return n;
			}
		}
		ASSERT(0); 
		return GT_INVALID;
	}

	//-----------------------------------------------------------------------------
	// ����һ��������ʵ�������������������ͽ�����Ϣ
	//-----------------------------------------------------------------------------
	VOID EndTask(LPCSTR szTaskName, BOOL bSetAutoDel=TRUE)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		if( GT_INVALID == nTaskIndex )
			return;

		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );
		
		// �������������ͽ�����Ϣ
		for( INT n=0; n<m_nMaxSubTask; n++ )
		{
			if( m_pTask[nTaskIndex].pSubTask[n].bValid )
				m_pTask[nTaskIndex].pSubTask[n].bRequireToEnd = TRUE;
		}

		m_pTask[nTaskIndex].bAutoDel = bSetAutoDel;
	}


	//-----------------------------------------------------------------------------
	// ɾ��һ��������
	//-----------------------------------------------------------------------------
	VOID DelTask(LPCSTR szTaskName)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		if( GT_INVALID == nTaskIndex )
			return;

		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );
		
		// Ϊ��֤�߼������ԣ����������ɺ����ɾ��
		ASSERT( m_pTask[nTaskIndex].eStats == ETS_Finished );	
		m_pTask[nTaskIndex].bValid = FALSE;
		
		// �������������
		for( INT n=0; n<m_nMaxSubTask; n++ )
			m_pTask[nTaskIndex].pSubTask[n].bValid = FALSE;
		
		m_pTask[nTaskIndex].nSubTask = 0;
		m_nTask--;

		// ����������ɾ��
		DWORD dwTaskNameCrc = m_pUtil->Crc32(szTaskName);
		m_mapTaskIndex.Erase(dwTaskNameCrc);
	}

	VOID DelTask(INT nTaskIndex)
	{
		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );
		
		// Ϊ��֤�߼������ԣ����������ɺ����ɾ��
		ASSERT( m_pTask[nTaskIndex].eStats == ETS_Finished );	
		m_pTask[nTaskIndex].bValid = FALSE;
		
		// �������������
		for( INT n=0; n<m_nMaxSubTask; n++ )
			m_pTask[nTaskIndex].pSubTask[n].bValid = FALSE;
		
		m_pTask[nTaskIndex].nSubTask = 0;
		m_nTask--;

		// ����������ɾ��
		DWORD dwTaskNameCrc = m_pUtil->Crc32(m_pTask[nTaskIndex].szName);
		m_mapTaskIndex.Erase(dwTaskNameCrc);
	}


	//-----------------------------------------------------------------------------
	// ��ʼһ��������
	//-----------------------------------------------------------------------------
	VOID StartTask(LPCSTR szTaskName)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		if( GT_INVALID == nTaskIndex )
			return;

		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );
		ASSERT( ETS_Ready == m_pTask[nTaskIndex].eStats );
		ASSERT( m_pTask[nTaskIndex].pSubTask[0].bValid );
	
		// ��������״̬
		m_pTask[nTaskIndex].eStats = ETS_Running;
		
		// ��ʼ��һ��������
		m_pTask[nTaskIndex].pSubTask[0].eStats = ETS_FirstRun;
		m_pTask[nTaskIndex].pSubTask[0].fStartTime = m_fCurTime;
	}


	//-----------------------------------------------------------------------------
	// ���ָ���������Ƿ���ڣ�ͨ�����������ɾ���������Ƿ�ִ�����
	//-----------------------------------------------------------------------------
	BOOL IsTaskExist(LPCSTR szTaskName)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		if( GT_INVALID == nTaskIndex )
			return FALSE;

		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		if( FALSE == m_pTask[nTaskIndex].bValid )
			return FALSE;
			
		return TRUE;
	}

	//-----------------------------------------------------------------------------
	// �õ�ָ����������ݽṹ��
	//-----------------------------------------------------------------------------
	tagTask<TaskAddin, your_class>* GetTask(LPCSTR szTaskName)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		if( GT_INVALID == nTaskIndex )
			return GT_INVALID;

		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );
		return &m_pTask[nTaskIndex];
	}

	tagTask<TaskAddin, your_class>* GetCurTask()
	{ return &m_pTask[m_nCurTask]; }

	tagSubTask<your_class>* GetCurSubTask()
	{ return &m_pTask[m_nCurTask].pSubTask[m_nCurSubTask]; }


	//-----------------------------------------------------------------------------
	// �õ�������״̬
	//-----------------------------------------------------------------------------
	VOID GetTaskState(LPCSTR szTaskName, BOOL& bFinished, DWORD& dwRet)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		if( GT_INVALID == nTaskIndex )
			return;
		
		ASSERT( m_pTask[nTaskIndex].bValid );
		
		bFinished = FALSE; dwRet = 0;
		if( ETS_Finished == m_pTask[nTaskIndex].eStats )
		{
			bFinished = TRUE;
			dwRet = m_pTask[nTaskIndex].dwRet;
		}
	}
	
	//-----------------------------------------------------------------------------
	// �õ���̨������ʵ��״̬
	//-----------------------------------------------------------------------------
	ETaskState GetTaskState(LPCSTR szTaskName)
	{
		INT nTaskIndex = GetTaskIndex(szTaskName);
		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		if( GT_INVALID == nTaskIndex )
			return ETS_Null;

		ASSERT( m_pTask[nTaskIndex].bValid );
		return m_pTask[nTaskIndex].eStats;
	}

	//-----------------------------------------------------------------------------
	// �����������ͽ�����Ϣ���������Ƿ��������Զ�ɾ����
	//-----------------------------------------------------------------------------
	VOID EndAllTask(BOOL bSetAutoDel)
	{
		for( INT n=0; n<m_nMaxTask; n++ )
		{
			if( !m_pTask[n].bValid )
				continue;

			EndTask(n, bSetAutoDel);
		}
	}
	


	//-----------------------------------------------------------------------------
	// ִ��������
	//-----------------------------------------------------------------------------
	VOID DoTask(DWORD dwParam)
	{
		for( INT n=0; n<m_nMaxTask; n++ )
		{
			if( m_pTask[n].bValid && ( ETS_Running == m_pTask[n].eStats 
				|| ETS_FirstRun == m_pTask[n].eStats ) )
			{
				// for short
				tagTask<TaskAddin, your_class> &t = m_pTask[n];	
				
				// �ӵ�һ������ʼ����
				for( INT m=0; m<m_nMaxSubTask; m++ )
				{
					// �������񲻴���,����Ϊ�����Ҳ������
					if( FALSE == t.pSubTask[m].bValid )
						break;
					
					// ��һ��������û�����У��Ժ�Ķ������ж�
					if( m>0 && ETS_Ready == t.pSubTask[m-1].eStats )
						break;
					
					switch( t.pSubTask[m].eStats )
					{
					case ETS_Running:	// ��������������
					case ETS_FirstRun:
						ASSERT( NULL != t.pSubTask[m].fp );
						m_nCurTask = n;
						m_nCurSubTask = m;

						// �����Ƿ�Ҫ��ִ�е�ʱ���Ѿ���ȥ
						if( m_fCurTime - t.pSubTask[m].fStartTime 
							>= t.pSubTask[m].fExpectTime )
							t.pSubTask[m].bExpectTimeout = TRUE;

						// ����������
						(m_pObj->*t.pSubTask[m].fp)( dwParam );
						if( ETS_FirstRun == t.pSubTask[m].eStats )
							t.pSubTask[m].eStats = ETS_Running;

						break;

					case ETS_Ready:
						if( m>0 )
						{
							// ���WaitTimeΪ����������ȴ���һ���������
							if( t.pSubTask[m].fWaitTime<0.0f )
							{
								if( ETS_Finished == t.pSubTask[m-1].eStats )
								{
									StartSubTask(n, m);	// ����������
								}
							}
							else
							{	// ����Ƿ�ʱ
								if( m_fCurTime - t.pSubTask[m-1].fStartTime 
									> t.pSubTask[m].fWaitTime )
								{
									StartSubTask(n, m);	// ����������
								}
							}
						}
						break;

						
					case ETS_Finished:
						if( IsAllSubTaskFinish(n) )	// �������������
						{
							t.eStats = ETS_Finished;	// ������������
							
							// ������ķ���ֵ�������������ķ���ֵ
							ASSERT( t.nSubTask>0 && t.nSubTask<=m_nMaxSubTask );
							t.dwRet = t.pSubTask[t.nSubTask-1].dwRet;
							
							// ����Ƿ����Զ�ɾ��������
							if( TRUE == t.bAutoDel )
							{
								DelTask(n);
								goto _next_task;// ��ֹ��̲��ֶԴ��������
							}
						}
						break;
						

					case ETS_Paused:
						break;

					default: ASSERT(0);	
						break;
					}
				}	// for( INT m=0; m<m_nMaxSubTask; m++ )
			}	// if( m_pTask[n].bValid && ETS_Running == m_pTask[n].eStats )
_next_task:;
		}	// for( INT n=0; n<m_nMaxTask; n++ )
	}

	TSimpleTask():m_fCurTime(0.0f), m_fDeltaTime(0.0f), m_pTask(NULL), 
		m_nMaxTask(0), m_nMaxSubTask(0), m_nCurTask(GT_INVALID), 
		m_nCurSubTask(GT_INVALID), m_nTask(0)
	{ m_pObj=(your_class*)this; m_pUtil = "util"; }

	TSimpleTask(your_class* pThis):m_fCurTime(0.0f), m_fDeltaTime(0.0f), 
		m_pTask(NULL), m_nMaxTask(0), m_nMaxSubTask(0), m_nCurTask(GT_INVALID), 
		m_nCurSubTask(GT_INVALID), m_nTask(0), m_pObj(pThis)
	{ m_pUtil = "util"; }

	virtual ~TSimpleTask()	{ DestroyTaskBuffer(); }

	//-----------------------------------------------------------------------------
	// ��׼�ĵȴ�����ֱ��ʱ�䵽
	//-----------------------------------------------------------------------------
	DWORD ProcWait(DWORD)
	{
		tagSubTask<TaskAddin>* pSub = GetCurSubTask();
		
		if( FALSE == pSub->bExecuted )	// �Ƿ��һ��ִ��
			pSub->bExecuted = TRUE;
		
		if( TRUE == pSub->bRequireToEnd || TRUE == pSub->bExpectTimeout )
			pSub->eStats = ETS_Finished;
	
		return 0;
	}


private:

	//-----------------------------------------------------------------------------
	// ���һ���������е������������Ƿ����
	//-----------------------------------------------------------------------------
	BOOL IsAllSubTaskFinish(INT nTaskIndex)
	{
		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( m_pTask[nTaskIndex].bValid );
		
		for( INT n=0; n<m_pTask[nTaskIndex].nSubTask; n++ )
		{
			if( ETS_Finished != m_pTask[nTaskIndex].pSubTask[n].eStats )
				return FALSE;
		}
		
		return TRUE;
	}

	//-----------------------------------------------------------------------------
	// ��ʼ����һ��������
	//-----------------------------------------------------------------------------
	VOID StartSubTask(INT nTaskIndex, INT nSubTaskIndex)
	{
		ASSERT( nTaskIndex>=0 && nTaskIndex<m_nMaxTask );
		ASSERT( nSubTaskIndex>=0 && nSubTaskIndex<m_nMaxSubTask );
		ASSERT( m_pTask[nTaskIndex].pSubTask[nSubTaskIndex].bValid );
		ASSERT( ETS_Finished != m_pTask[nTaskIndex].eStats );
		ASSERT( ETS_Ready == m_pTask[nTaskIndex].pSubTask[nSubTaskIndex].eStats );
		
		m_pTask[nTaskIndex].pSubTask[nSubTaskIndex].eStats = ETS_FirstRun;
		
		// ��¼����ʼʱ��
		m_pTask[nTaskIndex].pSubTask[nSubTaskIndex].fStartTime = m_fCurTime;
	}


	//-----------------------------------------------------------------------------
	// ͨ�����ֵõ�������ţ��Ҳ�������GT_INVALID
	//-----------------------------------------------------------------------------
	INT GetTaskIndex(LPCSTR szName)
	{
		DWORD dwTaskNameCrc = m_pUtil->Crc32(szName);
		return m_mapTaskIndex.Peek(dwTaskNameCrc);
	}

	//-----------------------------------------------------------------------------
	// ɾ�����񻺳�
	//-----------------------------------------------------------------------------
	void DestroyTaskBuffer()
	{
		if( m_nMaxTask==0 && m_nMaxSubTask==0 )
			return;

		ASSERT( m_nMaxTask>0 && m_nMaxSubTask>0 );
		ASSERT( m_pTask != NULL );
		
		// ��ɾ��������
		for( INT n=0; n<m_nMaxTask; n++ )
		{
			ASSERT( m_pTask[n].pSubTask!=NULL );
			delete[](m_pTask[n].pSubTask);
			m_pTask[n].pSubTask = NULL;
		}

		// ��ɾ��������
		delete[](m_pTask);
		m_pTask = NULL;

		m_nMaxTask = 0;
		m_nMaxSubTask = 0;
	}

	tagTask<TaskAddin, your_class>*	m_pTask;	// ָ��TaskBuffer
	your_class* m_pObj;		// ָ��ʵ����ָ��

	TObjRef<Util>		m_pUtil;
	TMap<DWORD, INT>	m_mapTaskIndex;	// �±�TaskNameCrc,ֵTaskIndex

	FLOAT	m_fCurTime;
	FLOAT	m_fDeltaTime;

	INT		m_nMaxTask;
	INT		m_nMaxSubTask;
	INT		m_nTask;
	INT		m_nCurTask;
	INT		m_nCurSubTask;

};

}	// namespace vEngine{
