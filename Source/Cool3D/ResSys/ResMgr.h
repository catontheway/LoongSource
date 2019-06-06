#pragma once
#include "..\Cool3D.h"

#define RES_MGR_RELEASE_RENDERABLE(x)	if(x!=NULL) {ResMgr::Inst()->ReleaseRenderable(x);x=NULL;}
#define RES_MGR_RELEASE_ALPHAMAP(x)		if(x!=NULL) {ResMgr::Inst()->ReleaseDyncAlphaMap(x);x=NULL;}
#define RES_MGR_RELEASE_VS(x)			if(x!=NULL) {ResMgr::Inst()->ReleaseVS(x);x=NULL;}
#define RES_MGR_RELEASE_PS(x)			if(x!=NULL) {ResMgr::Inst()->ReleasePS(x);x=NULL;}

	class IFS;
namespace Cool3D
{
	class UserResMgr;
	class ResRenderable;
	class ResTexture;
	class IDeviceTex;
	class ResSkeletonAction;
	class ResSpecialEffect;
	class ResDynaAlphaMap;
	class ResVertShader;
	class ResPixelShader;
	/** \class ResMgr
		\brief ��Դϵͳ����Ľӿ�
		\remarks
		\par ֧���첽ģʽ,���ǲ�û���ڽ�Resource Proxy,���ⲿ��ʹ����Դʱ��ȷ��ResBase::IsCreated()
		\par ����ģ�͵���ͼ·����Ӧ������Ӧ�ó�������Ŀ¼Ϊ�������·��
		\see ResBase::IsCreated()
		\todo:ʵ���ļ���׺��ResType��ע��,�����ⲿ��ResType�Ϳ���ע�����. Ŀǰ��������ResBase���캯���в���,��RTTIObj������
	*/
	class Cool3D_API ResMgr
	{
	public:
		enum EFSType
		{
			EFST_Animation=0,	//����
			EFST_Model,			//ģ��
			EFST_Effect,		//��Ч
			EFST_System,		//����ϵͳ����
			EFST_Map,			//����
			EFST_Num
		};
	public:
		ResMgr(void);
		~ResMgr(void);

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
		/**	Attachһ���û���Դ������,���ڲ�RenderableFactory���ؿյ�ʱ�������
		*/
		void AttachUserResMgr(UserResMgr *pMgr);
		/**	�����ڲ������֧��
		*/
		UserResMgr* GetUserResMgr() const;

		/**	�����ڲ�����Դ��
			\param	��ͼ�ص��������,����Ⱦ��Դ���������(��Ҫ��Index��vertex buffer)
		*/
		void Init(UINT texPoolSizeInBytes,UINT renderableSize,UINT skeletonSize,UINT specialEffectSize,UINT vsNum,UINT psNum);
		/**	����
		*/
		void Destroy();
		
		void SetFS(EFSType type,IFS* pFS);
		IFS* GetFS(EFSType type);
		IFS* GetFS(const TCHAR* szResName);
		/** ���FSΪNULL������DefaultFS
		*/
		IFS* SafeGetFS(EFSType type);
		IFS* SafeGetFS(const TCHAR* szResName);
		void OnFSChanged(EFSType type);

		/**
			\param szName,��ͼ�ļ���,Ӧʹ�����·��
			\param numLevels,mip map����,0Ϊ�Զ�����������
		*/
		ResTexture*	NewTexture(const TCHAR* szName,UINT numLevels=0);
		void ReleaseTexture(ResTexture *pTex);
		static IDeviceTex *GetTexFallBack();

		/**	�½�ResRenderable���������Դ
		*/
		ResRenderable* NewRenderable(const TCHAR* szName,DWORD param=0);
		void ReleaseRenderable(ResRenderable *pMesh);

		/** �½�ResSkeleton��Դ
		*/
		ResSkeletonAction* NewSkeleton(const TCHAR* szName,DWORD param=0,bool bAsync=true);
		void ReleaseSkeleton(ResSkeletonAction *pSkeleton);

		/** �½�ResSpecialEffect��Դ
		*/
		ResSpecialEffect* NewSpecialEffect(const TCHAR* szName,DWORD param=0);
		void ReleaseSpecialEffect(ResSpecialEffect *pSpecialEffect);

		ResDynaAlphaMap* NewDyncAlphaMap(const TCHAR* szName,int initVal,UINT size);
		void ReleaseDyncAlphaMap(ResDynaAlphaMap* pAlphaMap);

		/** �½�ResVertShader��Դ
		*/
		ResVertShader* NewVS(const TCHAR* szName,DWORD ver);
		void ReleaseVS(ResVertShader* pVS);

		/** �½�ResPixelShader��Դ
		*/
		ResPixelShader* NewPS(const TCHAR* szName,DWORD ver);
		void ReleasePS(ResPixelShader* pPS);

		void DrawInfo(int x,int y);

		//!	�ֶ�ִ�������ռ�
		void DoGC();

		//!	����ǰ��ResPool��״̬�г�����Log�ļ���,����Ӧ�ó��������Դʹ��״̬
		void Report();
		static ResMgr *Inst();

		int GetAsyncQueueSize();
		
	private:
		struct Member;
		Member	*m_p;

		static IDeviceTex *s_pTexFallback;
	};
}//namespace Cool3D