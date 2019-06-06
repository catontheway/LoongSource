#pragma once
#include "..\Util\RTTIObj.h"
#include "..\Util\FArchive.h"
#include "..\RenderMtl\MConstColor.h"
#include "..\RenderMtl\MBitmap.h"
#include "..\Math\CameraBase.h"
#include "..\SceneGraph\SceneGraph.h"

namespace Cool3D
{
	inline Vector3 Deg2Rad3f(Vector3& val)
	{
		Vector3 ret;
		ret.x=_DegToRad(val.x);
		ret.y=_DegToRad(val.y);
		ret.z=_DegToRad(val.z);
		return ret;
	}

	struct Cool3D_API tagMtlEffectDesc//��һ���ṹ������,���ڱ༭
	{
		TCHAR	szTexName[256];
		BYTE	blendType;			// 0disable,1alpha blend,2aditiveblend
		bool	bTestZ;				// �Ƿ����������Zֵ����
		int		frmU,
				frmV;				// һ����ͼ���Դ洢m*n֡����
		float	fps;				// ��ͼ�����Ĳ����ٶ�
		bool	bPlayOnce;			// ��ͼ����ֻ����һ��?

		float   fScrollSpeedU;		// ��������ٶ�
		float   fScrollSpeedV;		// ��������ٶ�
		BOOL	bHeatHaze;			// �Ƿ��ȴ���
		float   fHeatOffsetFator;	// �Ŷ�ϵ��
		bool    bEnableLight;       // �Ƿ��ܹ���Ӱ��
		BYTE    byteReserve[2];
		DWORD	dwReserve[11];

		tagMtlEffectDesc()
		{
			memset(this,0,sizeof(*this));
			bTestZ=true;
			frmU=frmV=1;
			fps=10.0f;
			bPlayOnce=false;

			fHeatOffsetFator = 0.02f;
		}
	};

	class RenderNode;
	class MBitmap;
	class SGEffectNode;
	class MMUVPage;

	/**	\class EffectBase
		\brief ������Ч�Ļ���
	*/
	class Cool3D_API EffectBase : public RTTIObj,public ISerializable
	{
	public:
		static bool s_reloadTex;//�༭��ģʽ��,ÿ�δ�������,������Load��ͼ
		struct tagProp
		{
			int			m_groupID;		//��������Ч�е�id
			TCHAR		m_szName[32];	//�û���ʶ����
			bool		m_bActive;		//�����active,��update,��render
			float		m_startTime;	//������Ч���ж�ú�,�Լ���ʼ����
			bool		m_loop;			//�Ƿ�ѭ������
			float		m_closeTime;	//�ر���Ҫ������
			int         m_nLOD;         //��Чϸ�ڵȼ�

			DWORD	dwReserve[6];

			tagProp()
			{
				m_groupID=0;	
				memset(m_szName,0,sizeof(m_szName));
				_tcscpy_s(m_szName,sizeof(m_szName)/sizeof(TCHAR),_T("none"));
				m_bActive=true;
				m_startTime=0;
				m_loop=true;
				m_closeTime=1.5f;
				m_nLOD=0;

				memset(dwReserve,0,sizeof(dwReserve));
			}
		};
	public:
		EffectBase(void);
		virtual ~EffectBase(void);

		//--���л��ӿ�
		virtual void Serialize(FArchive& ar);
		virtual void Deserialize(FArchive& ar);

		//!ÿ��Effectӵ�е�RenderNode�ĸ�ʽ�ڴ�����Ϻ�Ͳ��ܱ���<<==��Ҫ
		//!��Ҫ��Ϊ��Scene Node��mtl�ö�Ӧ����
		virtual int GetNumRenderNodes() =0;
		virtual RenderNode* GetRenderNode(int i) =0;
		virtual void SetGroupID(int id)=0;
		/** �����ڲ�����
			\param ��֡���ʱ��,������Ч���е�ʱ��
		*/
		virtual void Update(SceneGraph* pSG,SGEffectNode *pSGNode,CameraBase *pCamera,float deltaTime,float runTime) =0;
		/**	��һ����ЧԪ�ر��ֶ��Ĵ�SGEffectNode���Ƴ�ʱ,����
			\remarks ��Ҫ���ڱ༭��,Ԥ�����ڹر�ʱ,֪ͨ��ЧԪ��.��Щ��SGEffectNode���child scene node��
			��ЧԪ��,��ʱ����detach child.��EffectMesh
		*/
		virtual void OnDetachFromSGNode(SGEffectNode *pSGNode);

		/**	������Ч�������е�λ��(���Բ����ƶ���Ч��)
			\remarks ���Effect attach��ĳ���ƶ���SG Node��,��Effect������Ԫ��
				������SG Node��object space,�������Effect������Ԫ��(�����Ӱ,
				���ӷ���������������)������world space,��Effect��Ҫ�ƶ�,����Ҫʹ�ô˺���
		*/
		virtual const TCHAR* GetName() const =0;
		virtual void Rename(const TCHAR* szNewName) =0;
		//!������״̬��Ϊ��ʼ״̬
		virtual void ResetPlayState() =0;
		//!��ʼ����ƽ���ر�״̬
		virtual void Close(float sgNodeRunTime);
		//!�Ƿ���ȫ�ر�,���ɼ���
		virtual bool IsClosed();
		/*��Ч����ʱ��
			\return ����������޲��ţ�����-1
		*/
		virtual float GetPlayTime(){ return -1;}
		
		//--
		virtual void CreateMtl(const tagMtlEffectDesc& desc);
		virtual IMaterial* GetMtl();
		tagMtlEffectDesc GetMtlDesc() const	{	return m_mtlDesc;}
		//!
		static IMaterial *GetMtlFallback();

		virtual bool GetBox(AABBox& out) const =0;
		virtual int GetLOD() const =0;
	protected:
		//!	����desc��������,��������ʶ���
		void CreateBasicMtl(const tagMtlEffectDesc& desc);
		//!���ظ��º�ĵ�ǰ״̬���
		int AdvanceState(int curState,int numState,float curStateLastTime,float& stateTime,bool loop);
		//!
		void UpdateClosingFade(float runTime,float timeLen);
	protected:
		bool	m_bClosing;
		float	m_closeTime;//"�ر�״̬"����ʱ,��ЧSGNode�����˶��
		float	m_closingFade;

		tagMtlEffectDesc	m_mtlDesc;	//�浵Ҫ��
		MBitmap				*m_pMtl;	//ÿ��Effect�����ҽ���һ������
		static MConstColor	s_defaultMtl;//���pMtlΪ��,��ʹ��һ�����

		MMUVPage*           m_mmUVPage;  //����ʱҪ��
		DECL_RTTI(EffectBase);
	};
}//namespace Cool3D