// Cool3DViewDoc.h :  CCool3DViewDoc ��Ľӿ�
//


#pragma once
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;

class CCool3DViewDoc : public CDocument
{
protected: // �������л�����
	CCool3DViewDoc();
	DECLARE_DYNCREATE(CCool3DViewDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCool3DViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	AniMsgTable* GetAniMsgTable();
	AniMsgTable m_aniMsgTable;
	CString m_cstrCSVFilePath;
	CString GetCSVFilePath();
	void SetCSVFilePath(const TCHAR *szPath);
};


