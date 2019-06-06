#pragma once
//#include "DebugMisc.h"

namespace Cool3D
{

	class CRTAlloc
	{
	public:
		void *Alloc(UINT size)
		{
			return malloc(size);
		}
		void Free(void *pMem)
		{
			free(pMem);
		}
	};
	/**	\class MemPool
		\brief �ڴ��
		\remarks һ���Դ��������ͬ��С���ڴ�鱸��,��������Ƶ������/�ͷ�С���ڴ�ĳ���
	*/
	template<typename Allocer = CRTAlloc>
	class MemPool
	{
		struct tagBlock
		{
			void	*pMem;			//������ڴ�ָ��,������NULL
			DWORD	dwBlockSize;	//��ϵͳ������ڴ�Ĵ�С
			DWORD	dwUsedSize;		//ʹ�õĴ�С,С�ڵ���block size
			bool	bInUsing;		//�Ƿ����ڱ�ʹ��

			DWORD	dwMaxUsedSize;	//����ʹ�ô�С
			int		nUseCount;		//��ʹ�õĴ���
		};
	public:
		MemPool()
		{
			m_maxNotHitSize = 0;
			m_notHitTimes = 0;
		}
		virtual ~MemPool()
		{
			Destroy();
		}

		void *Alloc(DWORD size)
		{
			//--find in pool
			for(size_t i=0;i<m_blocks.size();i++)
			{
				tagBlock& block=m_blocks[i];
				if(block.bInUsing)
					continue;
				if(block.dwBlockSize>=size)//size����
				{
					block.bInUsing=true;
					if(size>block.dwMaxUsedSize)
						block.dwMaxUsedSize=size;
					block.dwUsedSize=size;
					block.nUseCount++;
					return block.pMem;
				}
			}

			if(size>m_maxNotHitSize)
			{
				m_maxNotHitSize=size;
			}
			m_notHitTimes++;

			//--alloc new
			Allocer a;
			return a.Alloc(size);
		}
		
		void Free(void *pMem)
		{
			//--�Ƿ�˵pool�����?
			for(size_t i=0;i<m_blocks.size();i++)
			{
				tagBlock& block=m_blocks[i];
				if(block.pMem==pMem)
				{
					block.bInUsing=false;
					return;
				}
			}

			//--free
			Allocer a;
			a.Free(pMem);
		}

		/**	�������÷��������ڴ�
			\param �ж��ٿ�,ÿ�����ж����ֽ�
		*/
		void Init(const vector<unsigned int>& blocksCfg)
		{
			size_t numBlocks=blocksCfg.size();
			Allocer a;
			for(size_t i=0;i<numBlocks;i++)
			{
				tagBlock block;
				memset(&block,0,sizeof(tagBlock));
				block.pMem=a.Alloc(blocksCfg[i]);
				block.dwBlockSize=blocksCfg[i];
				m_blocks.push_back(block);
			}
		}
		void Destroy()
		{
			Allocer a;
			for(size_t i=0;i<m_blocks.size();i++)
			{
				tagBlock& block=m_blocks[i];
				a.Free(block.pMem);
			}
			m_blocks.clear();
		}

		void Report(tstring& out)
		{
			out=_T("MemPool report----\r\n");
			TCHAR tmp[512];
			DWORD total=0;
			for(size_t i=0;i<m_blocks.size();i++)
			{
				tagBlock& block=m_blocks[i];
				_stprintf(tmp,_T("block size=%dk,use count=%d,max usedsize=%dk\r\n"),
					block.dwBlockSize/1024,block.nUseCount,block.dwMaxUsedSize/1024);
				out+=tmp;
				total+=block.dwBlockSize;
			}
			_stprintf(tmp,_T("--total block size=%dk.\r\n"),total/1024);
			out+=tmp;
			_stprintf(tmp,_T("--not hit times=%d, max not hit size=%dk.\r\n"),m_notHitTimes, m_maxNotHitSize/1024);
			out+=tmp;

		}

	private:
		vector<tagBlock>	m_blocks;
		DWORD				m_notHitTimes;	// δ���д���
		DWORD				m_maxNotHitSize;// ����δ���������С

	private://noncopyable
		MemPool(const MemPool& m);
		const MemPool& operator = (const MemPool& m);
	};
}//namespace Cool3D