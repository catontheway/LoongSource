#pragma once


// TWaterPage �Ի���

class TWaterPage : public CPropertyPage
{
	DECLARE_DYNAMIC(TWaterPage)

public:
	TWaterPage();
	virtual ~TWaterPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_Water };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
