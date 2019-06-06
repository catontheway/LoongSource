#pragma once
#include "math.h"

namespace Cool3D
{
	/**	\class ArcBall
		\brief ʹ��ArcBall��������϶�����ת����
		\remarks ֻ���ر����϶�����������ת
	*/
	class Cool3D_API ArcBall
	{
	public:
		ArcBall(void);
		~ArcBall(void);

		void ClearRotate();
		void SetWindowSize(UINT w,UINT h);
		void OnMouseBtnDown(UINT x,UINT y);
		void OnMouseMove(UINT x,UINT y);
		void OnMouseBtnUp();
		void GetRotateMat(Matrix4 *pOut);

	protected:
		void MapToBall(UINT x,UINT y,Vector3 *pOut);

		float	m_radius;
		float	m_wndWidth,
				m_wndHeight;

		Quaternion	m_quat;
		Quaternion	m_lastQuat;

	private:
		bool	m_bMousePressed;
		Vector3	m_savedPt;
	};
}//namespace Cool3D