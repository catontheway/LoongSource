// EffectAttacherDoc.h :  CEffectAttacherDoc ��Ľӿ�
//
#pragma once
#include "..\Cool3D\Cool3DEngine.h"
using namespace Cool3D;


class CEffectAttacherDoc : public CDocument
{
protected: // �������л�����
	CEffectAttacherDoc();
	DECLARE_DYNCREATE(CEffectAttacherDoc)

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
	virtual ~CEffectAttacherDoc();
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
	CString GetCSVFilePath();
	AniMsgTable m_aniMsgTable;
	CString m_cstrCSVFilePath;
	virtual void OnCloseDocument();
};


