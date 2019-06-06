#pragma once


// CPicProgress

class CPicProgress : public CStatic
{
	DECLARE_DYNAMIC(CPicProgress)

public:
	CPicProgress();
	virtual ~CPicProgress();

private:
	int m_nLValue;  //���ֵ
	int m_nHValue;   //���ֵ
	int m_nPos;	  //��ǰֵ
	float m_fLength;	//��ǰ����ֵ��ʵ����ʾ����
	CRect m_rect;
	CBitmap * m_bmpOld;
	CBitmap m_bmpBack;  //����ͼƬ
	CBitmap m_bmpFore;  //ǰ��ͼƬ
	CDC * m_pDCBack;
	CDC * m_pDCFore;
	BITMAP m_bmback;
	BITMAP m_bmfore;

public:
	int GetPos();
	int	OffsetPos( int nPos );
	//���õ�ǰֵ
	BOOL SetPos( int nPos );
	//���ý�������ֵ��Χ
	void SetRange( int nLower, int nHigh );
	//ȡ�ý�������ֵ��Χ
	void GetRange(int &nLower, int& nHigh);
	//����ǰ��ɫ
	void SetForceBmp(LPCTSTR szPicPath);
	//���ñ���ɫ
	void SetBackBmp(LPCTSTR szPicPath);	

protected:

	afx_msg void OnPaint();		//������Ⱦ

	DECLARE_MESSAGE_MAP()
};


