//-----------------------------------------------------------------------------
// File: sysinfo.h
// Auth: Lyp
// Date: 2004/02/03
// Last: 2004/06/22
// Desc: �õ�ϵͳ��Ϣ
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine{


class VENGINE_API SysInfo
{
public:

	// �õ�Windows�汾�����ַ���
	BOOL GetWinVer(LPSTR pszVersion, LPSTR pszBuild);
	// ����Ƿ�NT����ϵͳ
	BOOL IsNT();

	// �õ�DX�汾
	DWORD GetDXVersion();

	// �����쳣��õ��쳣�����ַ���
	CONST CHAR *GetExceptionDescription(DWORD ExceptionCode);

	// FormatTime ��ʱ��������˿����Ķ��ĸ�ʽ
	VOID FormatTime(LPSTR pDest, FILETIME TimeToPrint);

	// GetFilePart
	CHAR* GetFilePart(LPCSTR pSource);

	// �ر�ϵͳ
	BOOL SystemShutdown();

	// �õ�CPUָ�����
	DWORD GetCpuCaps();
	// �õ�CPU����
	INT GetProcessorNum();

};




}