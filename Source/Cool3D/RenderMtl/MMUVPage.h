#pragma once
#include "mmuvtrans.h"

namespace Cool3D
{
	/**	\class MMUVPage 
		\brief һ����ͼ�����������֡,ʹ��uv������������Щ֡
	*/
	class MMUVPage : public MMUVTrans
	{
	public:
		MMUVPage(void);
		virtual ~MMUVPage(void);

		void SetParam(int uDim,int vDim,float fps,bool bPlayOnce);
		virtual void Update(float deltaTime,IMaterial *pOwner);
		virtual void ResetAni(IMaterial *pOwner);

	private:
		void SetFrame(int frame,IMaterial *pOwner);
	private:
		int		m_uDim,
				m_vDim;	//������ͼ����uDim*vDim������֡
		float	m_fps;	//����֡�����ٶ�
		bool	m_bPlayOnce;	//�Ƿ�ֻ����һ��

		int		m_curFrm;
		float	m_curFrmTime;

		DECL_RTTI_DYNC(MMUVPage);
	};
}//namespace Cool3D