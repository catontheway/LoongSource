#pragma once
#include "..\Util\DebugMisc.h"
#include "AsyncResIO.h"

	class IFS;
namespace Cool3D
{
	/**	\class DefaultResFactory
	*/
	template <class ResType>
	class DefaultResFactory
	{
		public:
			/**	��Դ����û��ͬ����Դʱ����
			*/
			ResType* NewRes(const TCHAR* szName)
			{
				return new ResType(szName);
			}
	};

#if 0
	template<class ResType, class ResFactory = DefaultResFactory<ResType> >
	class ResPool
	{
		typedef map<tstring,ResType*> ResMap;
	private:
		ResMap	m_activeList;
		UINT	m_nCurSize;//��ǰ����Res��Size���ܺ�
	public:
		ResPool(UINT size)
		{			
			m_nCurSize = 0;
		}
		
		virtual ~ResPool(void)
		{			
		}

		ResType *NewRes(const TCHAR* _szName,IFS *pFS,DWORD param=0,bool bAsync=false)
		{
			if(_szName == NULL)
				return NULL;

			TCHAR szName[MAX_PATH] = {0};
			_tcscpy(szName, _szName);
			_tcslwr(szName);

			//-- find
			ResType *pRet = FindInPool(szName);
			if(pRet != NULL)
			{
				pRet->IncRef();				
				return pRet;
			}

			//-- create new
			ResFactory factory;
			pRet = factory.NewRes(szName);
			if(pRet == NULL)
				return NULL;

			pRet->AttachFS(pFS);			
			m_nCurSize += pRet->CreateContent(param);
			pRet->IncRef();

			//-- add to map
			m_activeList[pRet->GetName()]=pRet;

			return pRet;
		}

		void DelRes(ResType *pRes)
		{
			if(pRes == NULL)
				return;

			pRes->DecRef();

			if(pRes->GetRef() <= 0)
			{
				ResMap::iterator fIter = m_activeList.find(pRes->GetName());
				
				if(fIter != m_activeList.end())
				{
					delete pRes;
					m_activeList.erase(fIter);
					
				}//endof if(in active list)
				else
				{
					ASSERT(0 && _T("ResPool del resource failed"));
				}
			}			
		}

		void DoGC()
		{}

		int GetCurSize() const	{	return m_nCurSize;}
		void UpdateLoadingList() {}

		void ExportInfo(vector<pair<tstring,int> >& resInfoArray)
		{
			for(ResMap::iterator iter=m_activeList.begin();
				iter!=m_activeList.end();++iter)
			{
				ResType *pRes=(*iter).second;
				pair<tstring,int> resInfo;
				resInfo.first=pRes->GetName();
				resInfo.second=pRes->GetSize();
				resInfoArray.push_back(resInfo);
			}
		}

	protected:
		ResType *FindInPool(const TCHAR* szName)
		{
			ResMap::iterator iter=m_activeList.find(szName);
			if(iter != m_activeList.end())
			{
				return (*iter).second;
			}
			
			return NULL;
		}
	};
#else
	/**	\class ResPool
		\brief ��ԴPool
		\par ����������л����Դ, ������һ����������Cache��Դ
	*/
	template<class ResType, class ResFactory = DefaultResFactory<ResType> >
	class ResPool
	{
		typedef map<tstring,ResType*> ResMap;
	public:
		ResPool(UINT size)
		{
			m_nMaxSize=size;
			m_nCurSize=0;
		}
		virtual ~ResPool(void)
		{
			FinalRelease();
		}

		/**	����һ���µ���Դ
			\remarks �п��ܷ��ؿ�(��Factory���ؿ�ʱ)
		*/
		ResType *NewRes(const TCHAR* _szName,IFS *pFS,DWORD param=0,bool bAsync=false)
		{
			if(_szName == NULL)
				return NULL;
			TCHAR szName[MAX_PATH]={0};
			_tcscpy_s(szName,MAX_PATH,_szName);
			_tcslwr_s(szName,MAX_PATH);

			ResType *pRet=FindInPool(szName,true);
			if(pRet!=NULL)
			{
				pRet->IncRef();
				
				return pRet;
			}

			ResFactory factory;
			pRet=factory.NewRes(szName);
			if(pRet==NULL)
				return NULL;
			pRet->AttachFS(pFS);
			if(bAsync)
			{
				pRet->CreateContentAsync(param);
			}
			else
			{
				m_nCurSize+=pRet->CreateContent(param);
			}
			pRet->IncRef();

			

			int size=m_nCurSize-m_nMaxSize;
			if(size>0)
				MakeRoom(size);

			if(bAsync)
				m_loadingList[pRet->GetName()]=pRet;
			else
				m_activeList[pRet->GetName()]=pRet;
			return pRet;
		}
		/**	�ⲿ�ͷ�һ����Դ
			\remarks ��û��������delete res object,���Ǽ������ü������߷���DeadList��,
					ֻ�����ڳ��ռ�ʱ������delete
			\see MakeRoom()
		*/
		void DelRes(ResType *pRes)
		{
			if(pRes->GetRef() == 1)
			{
				ResMap::iterator fIter=m_activeList.find(pRes->GetName());
				
				if(fIter != m_activeList.end())
				{
					m_activeList.erase(fIter);				
					//--dead list�е�����Res�����ü�������0
					if(m_nCurSize<m_nMaxSize)//�������,�ռ�,���ݴ�����
					{
						pRes->DecRef();
						m_deadList.insert(make_pair(pRes->GetName(),pRes));
					}
					else
					{
						m_nCurSize-=pRes->GetSize();
						AsyncResIO::Inst()->QueueRequest(EResIO_Delete,pRes,0);
					}
				}//endof if(in active list)
				else
				{
					//must in loading list
					fIter=m_loadingList.find(pRes->GetName());
					ASSERT(fIter!=m_loadingList.end());

					m_nCurSize-=pRes->GetSize();
					AsyncResIO::Inst()->QueueRequest(EResIO_Delete,pRes,0);
					m_loadingList.erase(fIter);
				}
			}
			else
			{
				ASSERT(pRes->GetRef() > 1);
				pRes->DecRef();
			}
		}

		/**	ִ�������ռ�,������dead res���
		*/
		void DoGC()
		{
			ResMap::iterator iter=m_deadList.begin();
			for(;iter!=m_deadList.end();iter++)
			{
				ResType *pRes=(*iter).second;
				m_nCurSize-=pRes->GetSize();
				AsyncResIO::Inst()->QueueRequest(EResIO_Delete,pRes,0);
			}
			m_deadList.clear();
		}

		int GetCurSize() const	{	return m_nCurSize;}

		void UpdateLoadingList()
		{
			for(ResMap::iterator iter=m_loadingList.begin();
				iter!=m_loadingList.end();)
			{
				ResType *pRes=(*iter).second;
				if(pRes->IsCreated())
				{
					//Load���
					m_nCurSize+=pRes->GetSize();
					//�ƶ���Activelist
					iter=m_loadingList.erase(iter);
					m_activeList[pRes->GetName()]=pRes;
				}
				else
					++iter;
			}//endof for
		}

		void ExportInfo(vector<pair<tstring,int> >& resInfoArray)
		{
			for(ResMap::iterator iter=m_activeList.begin();
				iter!=m_activeList.end();++iter)
			{
				ResType *pRes=(*iter).second;
				pair<tstring,int> resInfo;
				resInfo.first=pRes->GetName();
				resInfo.second=pRes->GetSize();
				resInfoArray.push_back(resInfo);
			}

			for(ResMap::iterator iter=m_deadList.begin();
				iter!=m_deadList.end();++iter)
			{
				ResType *pRes=(*iter).second;
				pair<tstring,int> resInfo;
				resInfo.first=pRes->GetName();
				resInfo.second=pRes->GetSize();
				resInfoArray.push_back(resInfo);
			}
		}
	private:
		/**	�ͷ�������Դ,�����ⲿ�Ƿ��Դ�������,�ڳ���������
		*/
		void FinalRelease()
		{
			DoGC();
			//--
			ResMap::iterator iter=m_activeList.begin();
			for(;iter!=m_activeList.end();iter++)
			{
				ResType *pRes=(*iter).second;
				m_nCurSize-=pRes->GetSize();
				AsyncResIO::Inst()->QueueRequest(EResIO_Delete,pRes,0);
			}
			m_activeList.clear();

			//--free loading list
			iter=m_loadingList.begin();
			for(;iter!=m_loadingList.end();iter++)
			{
				ResType *pRes=(*iter).second;
				m_nCurSize-=pRes->GetSize();
				AsyncResIO::Inst()->QueueRequest(EResIO_Delete,pRes,0);
			}
			m_loadingList.clear();
			
		}
		/**	�����е�����list�в���
			\param	szName:��Դ����
					reactive:�����dead list�з���,�Ƿ����ƶ���active list
		*/
		ResType *FindInPool(const TCHAR* szName,bool reactive)
		{
			ResMap::iterator iter=m_activeList.find(szName);
			if(iter!=m_activeList.end())
			{
				return (*iter).second;
			}

			iter=m_loadingList.find(szName);
			if(iter!=m_loadingList.end())
			{
				return (*iter).second;
			}
			
			iter=m_deadList.find(szName);
			if(iter!=m_deadList.end())
			{
				ResType *pRet=(*iter).second;
				if(reactive)//���¼���
				{
					m_deadList.erase(iter);
					m_activeList.insert(make_pair(pRet->GetName(),pRet));
				}
				return pRet;
			}
			
			return NULL;
		}
		/**	�ڳ�ָ����С�Ŀռ䱸��
		*/
		bool MakeRoom(int size)//ɾ��deadList�е���Դ���ڳ��ռ�,������m_nCurSize
		{
			if(m_deadList.empty())
				return false;

			int total=0;
			
			ResMap::iterator iter=m_deadList.begin();
			for(;iter!=m_deadList.end();)
			{
				ResType *pRes=(*iter).second;
				total+=pRes->GetSize();
				iter=m_deadList.erase(iter);
				AsyncResIO::Inst()->QueueRequest(EResIO_Delete,pRes,0);
				if(total>=size)
					break;
			}
			
			m_nCurSize-=total;

			return total > size;
		}
	private:
		ResMap	m_deadList;
		ResMap	m_activeList;
		ResMap	m_loadingList;//����Loading����Դ,load��ɺ����size

		UINT	m_nMaxSize;
		UINT	m_nCurSize;//��ǰ����Res��Size���ܺ�
	};
#endif
}//namespace Cool3D