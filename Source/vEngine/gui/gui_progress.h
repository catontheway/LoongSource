//-----------------------------------------------------------------------------
//!\file gui_progress.h
//!\author Lyp
//!
//!\date 2004-12-08
//! last 2004-12-28
//!
//!\brief ����ϵͳ progress �ؼ�
//-----------------------------------------------------------------------------
#pragma once


namespace vEngine {
//-----------------------------------------------------------------------------
// ����ϵͳ progress �ؼ�(������)
//-----------------------------------------------------------------------------
class VENGINE_API GUIProgress : public GUIStatic
{
public:
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();

	VOID SetValue(FLOAT fValue,bool bForceSet=false);
	VOID SetMaxValue(FLOAT fMaxValue);
	VOID SetSpeed(FLOAT fSpeed) { m_fSpeed = fabs(fSpeed); } //��ǰֵ���趨ֵ�仯���ٶ�

	FLOAT GetCurrentValue() {	return m_fCurrentValue;}
	FLOAT GetMaxValue()	{ return m_fMaxValue; }

	VOID SetForePicColor(DWORD dwForePicColor){ m_dwForePicColor = dwForePicColor; }

	GUIProgress();

protected:
	FLOAT			m_fMaxValue;
	FLOAT			m_fTargetValue;
	FLOAT			m_fCurrentValue;
	FLOAT			m_fSpeed;
	DWORD			m_dwLastTime;
	tagGUIImage*	m_pForePic;


	BOOL		m_bHorizon;					// �Ƿ�ˮƽ
	tstring		m_strForePicFileName;		// ǰ��ͼƬ�ļ���
	tagRect		m_rcForePic;				// ǰ��ͼƬʹ������
	DWORD		m_dwForePicColor;			// ǰ��ͼƬ������ɫ
};



}	// namespace vEngine {

