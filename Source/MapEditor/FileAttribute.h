#pragma once
#include "afxwin.h"
#include ".\ColorButton.h"

// FileAttribute �Ի���

class FileAttribute : public CDialog
{
	DECLARE_DYNAMIC(FileAttribute)

public:
	FileAttribute(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FileAttribute();

// �Ի�������
	enum { IDD = IDD_MapProperty };
	virtual BOOL OnInitDialog();

	Vector3 GetSunPos()const{return m_vecSunPos;}
	Color4f GetSunDiffuse()const{return m_SunCol;}
	Color4f GetAmbient()const{return m_AmbientCol;}
	Color4f GetSunSpecular()const{return m_Specular;}
	void GetSkyBox(CString& szTopTex, CString& szFrontRightTex, CString& szBackLeftTex, 
		Vector3& drawSize, Vector3& offSize)const;
	FLOAT GetSunModulus()const { return m_fSunModulus;}
	FLOAT GetSkyYaw()const { return m_fSkyYaw;}
	VOID GetFogAttribute(FLOAT& start, FLOAT& end, FLOAT& den){start=m_fstart; end=m_fend; den=m_fdensity;}
	Color4f GetFogColor()const { return m_FogColor; }
	Color4f GetSkyCol()const { return m_SkyCol; }
	DWORD GetSkyShadeCol()const { return m_dwSkyShadeCol; }
	bool IsRenderSkyShade()const { return TRUE == m_bRenderSkyShade; }
	DWORD GetDynamicDiffCol()const { return m_dwDynamicDiffCol; }
	DWORD GetDynamicAmbCol()const { return m_dwDynamicAmbCol; }
	DWORD GetDynamicSpecCol()const { return m_dwDynamicSpecCol; }

	void SetOperationType(bool bVal){m_bSave=bVal;}
	void SetSunPos(const Vector3& sunPos){m_vecSunPos = sunPos;}
	void SetSunDiffuse(const Color4f& sunCol){m_SunCol = sunCol;}
	void SetAmbient(const Color4f& echoCol){m_AmbientCol = echoCol;}
	void SetSunSpecular(const Color4f& specular){m_Specular = specular;}
	void SetSkyBox(const CString& szTopTex, const CString& szFrontRightTex, const CString& szBackLeftTex, 
		const Vector3& drawSize, const Vector3& offSize);
	void SetSunModulus(FLOAT fVal) { m_fSunModulus = fVal; }
	void SetSkyYaw(FLOAT fVal) { m_fSkyYaw = fVal; }
	void SetFogAttribute(FLOAT start, FLOAT end, FLOAT den) {m_fstart=start; m_fend=end; m_fdensity=den;}
	void SetFogColor(const Color4f& col) { m_FogColor = col; }
	void SetSkyCol(const Color4f& skyCol) { m_SkyCol = skyCol;}
	void SetSkyShadeCol(const DWORD dwSkyShadeCol) { m_dwSkyShadeCol = dwSkyShadeCol;}
	void SetRenderSkyShade(const bool bRenderSkyShade) { m_bRenderSkyShade = bRenderSkyShade; }
	void SetDynamicDiffCol(const DWORD dwCol) { m_dwDynamicDiffCol = dwCol; }
	void SetDynamicAmbCol(const DWORD dwCol) { m_dwDynamicAmbCol = dwCol; }
	void SetDynamicSpecCol(const DWORD dwCol) { m_dwDynamicSpecCol = dwCol; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HBRUSH m_brMine;
private:
	bool m_bSave;							// �Ǳ��滹�Ƕ���
	Vector3 m_vecSunPos;					// ̫�����λ��
	Color4f m_SunCol;						// ̫������ɫ
	Color4f m_AmbientCol;					// ���ɢ�����ɫ
	Color4f m_Specular;						// �߹�
	DWORD   m_dwDynamicDiffCol;             // ��̬ƽ�й�diffuse
	DWORD   m_dwDynamicAmbCol;				// ��̬ƽ�й�Ambient
	DWORD   m_dwDynamicSpecCol;             // ��̬ƽ�й�Specular
	Color4f m_SkyCol;						// ���
	DWORD   m_dwSkyShadeCol;				// ���������ɫ
	BOOL    m_bRenderSkyShade;              // �Ƿ���Ⱦ�������
	CString m_szSkyBoxTopTex;				//��պ�������
	CString m_szSkyBoxFrontRightTex;		//��պ�ǰ������
	CString m_szSkyBoxBackLeftTex;			//��պк�������
	Vector3 m_vecSkyBoxSize;				//��պд�С
	Vector3 m_vecSkyBoxOffSize;				//��պ�ƫ��
	FLOAT	m_fSunModulus;					//̫������ǿϵ��
	FLOAT	m_fSkyYaw;						//��պ���ת�Ƕ�
	Color4f m_FogColor;						//����ɫ
	FLOAT	m_fstart;						//��start					
	FLOAT	m_fend;							//��end	
	FLOAT	m_fdensity;						//��Ũ��
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeMapid();
	CEdit m_EditSunPosX;					// ̫�����λ��X��
	CEdit m_EditSunPosY;					// ̫�����λ��Y��
	CEdit m_EditSunPosZ;					// ̫�����λ��Z��
	CColorButton m_btnSunCol;
	CColorButton m_btnAmbientCol;
	CColorButton m_btnSpecular;
	CColorButton m_btnSkyCol;
	CColorButton m_btnSkyShadeCol;
	CColorButton m_btnDynamicDiffCol;
	CColorButton m_btnDynamicAmbCol;
	CColorButton m_btnDynamicSpecCol;
	afx_msg void OnBnClickedButtonSuncol();	//̫������ɫ��ť�¼�
	afx_msg void OnBnClickedButtonEchocol();//���ɢ�����ɫ��ť�¼�
	afx_msg void OnBnClickedButtonSpecular();//����ⰴť�¼�
	CEdit m_editSkyBoxX;		//��պ�X��size
	CEdit m_editSkyBoxY;		//��պ�Y��size
	CEdit m_editSkyBoxZ;		//��պ�Z��size
	afx_msg void OnBnClickedBtnTop();
	afx_msg void OnBnClickedBtnFntrgt();
	afx_msg void OnBnClickedBtnBckleft();
	CEdit m_EditSkyBoxOffX;
	CEdit m_EditSkyBoxOffY;
	CEdit m_EditSkyBoxOffZ;
	CEdit m_editSunModulus;		//̫������ǿϵ��
	CEdit m_editSkyYaw;			//��պ���ת�Ƕ�
	CColorButton m_btnFogColor;	
	CEdit m_editFogDen;
	CEdit m_editFogStart;
	CEdit m_editFogEnd;
	afx_msg void OnBnClickedButtonFogcolor();
	int m_dist;
	afx_msg void OnEnChangeEditDist();
	afx_msg void OnBnClickedButtonSkycol();//���
	afx_msg void OnBnClickedSkyshadecol();
	afx_msg void OnBnClickedButtonDynamicdiff();
	afx_msg void OnBnClickedButtonDynamicamb();
	afx_msg void OnBnClickedButtonDynamicspec();
};
