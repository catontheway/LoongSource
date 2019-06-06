//-----------------------------------------------------------------------------
// File: stdio_redir
// Auth: Lyp
// Date: 2007/11/14
// Last: 2007/11/14
//-----------------------------------------------------------------------------
#pragma once

namespace vEngine{
//-----------------------------------------------------------------------------
//!\brief stdio �ض���
//!
//!
//-----------------------------------------------------------------------------
class VENGINE_API StdioRedir
{
public:
	BOOL Init();
	VOID Destroy();

	LPBYTE	Recv(DWORD& dwSize);
	BOOL	Send(LPVOID pMsg, DWORD dwMsgSize);

	//! ��Ӧ��Recv(),��Ϊ�ڴ����ڲ�����ģ�����Ӧ�ý����ڲ����ͷ�
	VOID FreeRecved(LPBYTE pMsg);

private:

	HANDLE		m_hProcess;
	HANDLE		m_hStdoutRdDup;
	HANDLE		m_hStdinWrDup;
};

} // namespace vEngine {