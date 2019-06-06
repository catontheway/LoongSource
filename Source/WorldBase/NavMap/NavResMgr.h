#pragma once
#include "..\WorldBase.h"

#define NAV_RES_MGR_RELEASE_RES(x)	if(x!=NULL) {NavResMgr::Inst()->ReleaseRes(x);x=NULL;}

class IFS;
namespace WorldBase
{
	class NavResBase;

	/** \class NavResMgr
	\brief ��Դϵͳ����Ľӿ�
	\remarks
	\par ֧���첽ģʽ,���ǲ�û���ڽ�Resource Proxy,���ⲿ��ʹ����Դʱ��ȷ��NavResBase::IsCreated()
	\par ����ģ�͵���ͼ·����Ӧ������Ӧ�ó�������Ŀ¼Ϊ�������·��
	\see NavResBase::IsCreated()
	\todo:ʵ���ļ���׺��ResType��ע��,�����ⲿ��ResType�Ϳ���ע�����. Ŀǰ��������NavResBase���캯���в���,��RTTIObj������
	*/
	class WORLD_BASE_API NavResMgr
	{
	public:
		enum EFSType
		{
			EFST_Model,			//ģ��
			EFST_System,		//����ϵͳ����
			EFST_Map,			//����
			EFST_Num
		};
	public:
		NavResMgr(void);
		~NavResMgr(void);

		/**	�����Ƿ������첽IOģʽ
		\param	�Ƿ���첽ģʽ
		\return void
		\remarks ���ʹ���첽ģʽ,��Resource��Create����������,��ֻ��ResBase::IsCreate()����Trueʱ,��Resource�ſ���
		\see ResBase::IsCreated()
		*/
		void EnableIOAsync(bool bAsync);
		bool IsIOAsync() const;
		/**	�����ڲ�״̬,��Ҫ���ڼ����첽IO��״̬
		*/
		void Update();
		/**	�õ��ж��ٸ�Resource����Load,��ָ�첽IOģʽ
		*/
		UINT GetNumLoading();
		/** �õ�Resource���ؼ���
		*/
		void GetLoadCount(UINT& finishedCount,UINT& reqCount);
		/** ���Resource���ؼ���
		*/
		void ZeroLoadCount();

		/**	�����ڲ�����Դ��
		*/
		void Init(UINT poolSize,bool bPoolThreadSync=false);
		/**	����
		*/
		void Destroy();

		void SetFS(EFSType type,IFS* pFS);
		IFS* GetFS(EFSType type);
		IFS* GetFS(const TCHAR* szResName);
		IFS* DefaultFS();
		/** ���FSΪNULL������DefaultFS
		*/
		IFS* SafeGetFS(EFSType type);
		IFS* SafeGetFS(const TCHAR* szResName);
		void OnFSChanged(EFSType type);

		/**	�½�NavResSceneMesh���������Դ
		*/
		NavResBase* NewRes(const TCHAR* szName,DWORD param=0);
		void ReleaseRes(NavResBase *pRes);

		//!	�ֶ�ִ�������ռ�
		void DoGC();

		//!	����ǰ��ResPool��״̬�г�����Log�ļ���,����Ӧ�ó��������Դʹ��״̬
		void Report();
		static NavResMgr *Inst();

		int GetAsyncQueueSize();

	private:
		struct Member;
		Member	*m_p;
	};
}//namespace WorldBase