
#pragma once
#include "..\F3D.h"
#include "..\Math\CameraEuler.h"
namespace F3D
{
	class NullSceneGraph;
	class EntityNode;

	/**	\class F3DView
		\brief �򵥵�ģ�����,�����������ƹ۲�Ƕ�
	*/
	class F3D_API F3DView
	{
	public:
		F3DView(void);
		virtual ~F3DView(void);

		/**	��Ⱦ��ָ���Ĵ���
			\param hWnd,Ŀ�괰��
			\param bAdaptive,�Ƿ�ʹ����Ⱦͼ�������������
		*/
		void Render(HWND hWnd,bool bAdaptive=true);
		/**	������Ҫ�۲��ģ��
		*/
		virtual void LoadMdl(const char* szMdlName);

		//--��������
		void OnLButtonDown(UINT nFlags, POINT point);
		void OnLButtonUp(UINT nFlags, POINT point);
		void OnMouseMove(UINT nFlags, POINT point);
		void OnRButtonDown(UINT nFlags, POINT point);
		void OnRButtonUp(UINT nFlags, POINT point);
		void OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	protected:
		void Begin();
		virtual void RenderView(bool bAdaptive);
		void End(HWND hWnd);
	protected:
		DWORD			m_lastRender;	//ʹ�������ļ�ʱ��
		NullSceneGraph	*m_pSG;
		EntityNode		*m_pEntity;
		CameraEuler		m_camera;

		//--ctrl
		bool	m_bDrag,
				m_bRightDrag;
		POINT	m_DragPt;
	};
}//namespace F3D