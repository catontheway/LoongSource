
#pragma once
#include "..\Cool3D.h"
#include "..\Math\CameraEuler.h"
#include "..\Math\Color.h"
namespace Cool3D
{
	class NullSceneGraph;
	class EntityNode;

	/**	\class SimpleView
		\brief �򵥵�ģ�����,�����������ƹ۲�Ƕ�
	*/
	class Cool3D_API SimpleView
	{
	public:
		SimpleView(void);
		virtual ~SimpleView(void);

		/**	��Ⱦ��ָ���Ĵ���
			\param hWnd,Ŀ�괰��
			\param bAdaptive,�Ƿ�ʹ����Ⱦͼ�������������
		*/
		void Render(HWND hWnd,bool bAdaptive=true);
		/**	������Ҫ�۲��ģ��
		*/
		virtual void LoadMdl(const TCHAR* szMdlName);

		//--��������
		void OnLButtonDown(UINT nFlags, POINT point);
		void OnLButtonUp(UINT nFlags, POINT point);
		void OnMouseMove(UINT nFlags, POINT point);
		void OnRButtonDown(UINT nFlags, POINT point);
		void OnRButtonUp(UINT nFlags, POINT point);
		void OnMouseWheel(UINT nFlags, short zDelta, POINT pt);

		EntityNode* GetEntityNode() {	return m_pEntity;}
		CameraEuler* GetCamera()	{	return &m_camera;}
		void SetBkColor(Color4ub bk)	{	m_bkColor=bk;}
	protected:
		void Begin();
		virtual void RenderView(bool bAdaptive);
		void End(HWND hWnd);
	protected:
		DWORD			m_lastRender;	//ʹ�������ļ�ʱ��
		NullSceneGraph	*m_pSG;
		EntityNode		*m_pEntity;
		CameraEuler		m_camera;
		Color4ub		m_bkColor;

		//--ctrl
		bool	m_bDrag,
				m_bRightDrag;
		POINT	m_DragPt;
	};
}//namespace Cool3D