#pragma once



class CLoongOutputCtrl
{
public:
	CLoongOutputCtrl(void);
	~CLoongOutputCtrl(void);
public:
	VOID Init(LPVOID pOutputWnd);

	VOID Output(LPCTSTR szStr); //��ӡ��Ϣ
private:
	LPVOID m_pOutputWnd;

	CRITICAL_SECTION m_csOutput;

};
extern CLoongOutputCtrl g_clsOutput;

