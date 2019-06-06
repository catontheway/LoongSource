#pragma once
#include "..\Device\IDevice.h"
#include "MtlRenderCache.h"
#include "RenderSysFixed.h"
#include "..\Math\Transform.h"
#include "..\Util\Noncopyable.h"

namespace Cool3D
{
	/**	\struct tagTexStage
	*/
	struct tagTexStage
	{
		IDeviceTex		*pTex;
		IRenderTexture	*pRTex;
		ETextureAddress	addressU;
		ETextureAddress	addressV;
		ETextureAddress	addressW;
		ETextrueFilter	magFilter;
		ETextrueFilter	minFilter;
		ETextrueFilter	mipFilter;
		ETextureOp		texOp;
		ETextureArg		texArg1;
		ETextureArg		texArg2;
		ETextureOp		alphaOp;
		ETextureArg		alphaArg1;
		ETextureArg		alphaArg2;
		bool			bAutoUVIndex;//�Ƿ��Զ�ʹ��stage�����ΪuvIndex
		int				uvIndex;//ʹ�õڼ���uv
		DWORD			uvIndexFlags;
		Matrix2			matBump;
		Transform		uvTrans;//Ŀǰֻ֧�ֶ�UV0~1���б任
		EUVTrans		uvTransType;

		tagTexStage()
		{
			pTex=NULL;
			pRTex=NULL;
			addressU=addressV=addressW=ETAddress_Wrap;
			magFilter=minFilter=ETFilter_LINEAR;
			mipFilter=ETFilter_NONE;
			texOp=ETOP_Disable;
			texArg1=ETA_Texture;
			texArg2=ETA_Diffuse;
			alphaOp=ETOP_Disable;
			alphaArg1=ETA_Texture;
			alphaArg2=ETA_Diffuse;
			uvIndex=0;
			matBump._11=matBump._12=matBump._21=matBump._22=0.0f;
			uvTransType=EUVT_Disable;
			uvIndexFlags=0;
			bAutoUVIndex=false;
		}
	};
	typedef vector<tagTexStage> TexStageArray;

	/**	\class DevPixelStageFixed
		\brief �豸�����عܵ�״̬,��Թ̶���Ⱦ�ܵ����
	*/
	class DevPixelStageFixed : private Noncopyable
	{
	public:
		typedef pair<ERStateType,DWORD>	RenderState;
	public:
		DevPixelStageFixed(void);
		virtual ~DevPixelStageFixed(void);

		void AddStage(const tagTexStage& newState);
		void Set2Device();
	
		void AppendState(ERStateType state,DWORD value);
		void ModifyState(ERStateType state,DWORD value);

		bool IsTexUsed(IDeviceTex* pTex);
		bool IsTexUsed(IRenderTexture* pRTex);
		int CalcTexUsed(const tagTexStage& newStage);

		static void SetStage2Device(UINT i,const tagTexStage& stage,bool bSetTex=true);
	public:
		TexStageArray		m_stages;
		vector<RenderState>	m_states;
		tagRenderColor		m_color;
		bool				m_bMultiPass;
		int					m_numTexUsed;//ͬʱʹ�õ�texture�ļ���
	};
}//namespace Cool3D