//-----------------------------------------------------------------------------
//!\file gui_patch.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2007-06-15
//!
//!\brief ����ϵͳ patch �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳwindow�ؼ�(�ɱ��С��ƴͼ����)
//-----------------------------------------------------------------------------
class VENGINE_API GUIPatch : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();

	GUIPatch();

protected:

	tagGUIImage*	m_pTopPic;			// �ϲ�ͼƬhandle
	tagGUIImage*	m_pMidPic;			// �в�ͼƬhandle
	tagGUIImage*	m_pDownPic;			// �²�ͼƬhandle
	tagGUIImage*	m_pLeftPic;			// ��ͼƬhandle
	tagGUIImage*	m_pRightPic;		// �Ҳ�ͼƬhandle
	tagGUIImage*	m_pLeftTopPic;		// ����ͼƬhandle
	tagGUIImage*	m_pRightTopPic;		// ����ͼƬhandle
	tagGUIImage*	m_pLeftDownPic;		// ����ͼƬhandle
	tagGUIImage*	m_pRightDownPic;	// ����ͼƬhandle

	//-----------------------------------------------------------------------------
	// �ɱ༭����
	//-----------------------------------------------------------------------------
	tstring			m_strTopPic;		// �ϲ�ͼƬ�ļ���
	tagRect			m_rcTopPic;			// �ϲ�ͼƬʹ������
	tstring			m_strMidPic;		// �в�ͼƬ�ļ���
	tagRect			m_rcMidPic;			// �в�ͼƬʹ������
	tstring			m_strDownPic;		// �²�ͼƬ�ļ���
	tagRect			m_rcDownPic;		// �²�ͼƬʹ������
	tstring			m_strLeftPic;		// ��ͼƬ�ļ���
	tagRect			m_rcLeftPic;		// ��ͼƬʹ������
	tstring			m_strRightPic;		// �Ҳ�ͼƬ�ļ���
	tagRect			m_rcRightPic;		// �Ҳ�ͼƬʹ������
	tstring			m_strLeftTopPic;	// ����ͼƬ�ļ���
	tagRect			m_rcLeftTopPic;		// ����ͼƬʹ������
	tstring			m_strRightTopPic;	// ����ͼƬ�ļ���
	tagRect			m_rcRightTopPic;	// ����ͼƬʹ������
	tstring			m_strLeftDownPic;	// ����ͼƬ�ļ���
	tagRect			m_rcLeftDownPic;	// ����ͼƬʹ������
	tstring			m_strRightDownPic;	// ����ͼƬ�ļ���
	tagRect			m_rcRightDownPic;	// ����ͼƬʹ������

};


}	// namespace vEngine {
