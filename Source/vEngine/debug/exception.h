//-----------------------------------------------------------------------------
// File: exception
// Auth: Lyp
// Date: 2003/12/11
// Desc: ��¼�쳣�����Ļ���
//-----------------------------------------------------------------------------
#pragma once
/*
namespace vEngine {
typedef unsigned long ULONG3264, *PULONG3264, DWORD3264, *PDWORD3264;

#define EXCEPTION_PROTECT_START	_set_se_translator( vEngine::SEH_Translation ); try{
#define EXCEPTION_PROTECT_END	} catch(vEngine::Exception){}

// ��EXCEPTION_PROTECT_START��EXCEPTION_PROTECT_END��ϲ�����ʹ��ʱ������
#define EXCEPTION_PROTECT ::SetUnhandledExceptionFilter(vEngine::SEH_ExceptionFilter);

enum EBasicType  // ���� CVCONST.H (Microsoft DIA 2.0 SDK)
{
	EBT_NoType = 0,
	EBT_Void = 1,
	EBT_Char = 2,
	EBT_WChar = 3,
	EBT_Int = 6,
	EBT_UInt = 7,
	EBT_Float = 8,
	EBT_BCD = 9,
	EBT_Bool = 10,
	EBT_Long = 13,
	EBT_ULong = 14,
	EBT_Currency = 25,
	EBT_Date = 26,
	EBT_Variant = 27,
	EBT_Complex = 28,
	EBT_Bit = 29,
	EBT_BSTR = 30,
	EBT_Hresult = 31
};

//-----------------------------------------------------------------------------
// Exception ���˼��:
// 1.ʹ�����ܹ�������׳��쳣
// 2.�ܹ�����Windows�ṹ���쳣,��ת��ΪC++�쳣
// 3.���κ��쳣�׳�ʱ(�����Ƿ�������),�ܼ�¼���ö�ջ,�Ĵ���״̬����Ϣ

/* ��׼�÷�: 
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, INT nCmdShow)
{
	INT nResult = -1;
	_set_se_translator( vEngine::SEH_Translation );

	try
	{
		CApp app;
		app.Initialize(hInst, lpCmdLine, nCmdShow);
		nResult = app.MainLoop();
	}
	catch(vEngine::Exception)
	{
	}
	return nResult;
}*/

// �������ֶ��׳��쳣ʱ
// throw( "your exception's description" );
/*
//-----------------------------------------------------------------------------
class VENGINE_API Exception
{
public:

	Exception(UINT unCode, EXCEPTION_POINTERS* pExp);
	Exception(PCONTEXT pContext);	// �ֶ���¼�������ö�ջʱʹ��
	~Exception();

	static BOOL				m_bSendLog;
	static BOOL				m_bSendLogWindow;

private:
	VOID InitDebugHelp();
	BOOL GetFunctionName(LPSTR szDest, PVOID pFunction);
	
	INT  DumpAll(EXCEPTION_POINTERS *pEx);
	VOID DumpExceptionInfo(EXCEPTION_POINTERS *pEx);	// �쳣��Ϣ
	VOID DumpSystemInfo();								// ��ͨϵͳ��Ϣ
	VOID DumpCallStack(PCONTEXT pContext);				// ���ö�ջ
	VOID DumpRegisters(PCONTEXT pContext);				// �Ĵ���״̬
	VOID DumpInstructions(PCONTEXT pContext);			// �����쳣����ָ��
	VOID DumpVariable(PCONTEXT pContext);				// ����

	VOID SendLog();	// ���������Log��Ϣ
	VOID GetSymbolPath(std::string& strPath);
	
	static BOOL CALLBACK SymEnumSymbolsProc(PSYMBOL_INFO, ULONG, PVOID);
	static BOOL FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME * sf);
	static BOOL DumpTypeIndex(DWORD64, DWORD, DWORD3264, INT nCycle);
	static VOID FormatOutputValue(EBasicType, DWORD64, PVOID);
	static EBasicType GetBasicType( DWORD, DWORD64);

	static UINT				m_unExceptionCode;
	static HANDLE			m_hProcess;

	static ExceptionLog*	m_pLog;
	static SysInfo*			m_pSysInfo;
	static Util*			m_pUtil;
	static DiscIOMgr*		m_pDiscIO;

};


}	// namespace vEngine {
*/
/*
	Finally, a hint if you use VC++ 6.0 and exception handling. For VC++, 
	a "synchronous" exception is one raised by a "throw" statement; everything 
	else is an asynchronous exception (i.e., one which the compiler cannot 
	anticipate). If you think your code may throw Structured Exceptions -- even 
	if you have a translator function! -- you must enable support for asynchronous 
	exceptions. To that end, I strongly recommend unchecking the "enable exception 
	handling" checkbox in the C++ page of the Project Options dialog and typing the 
	option "/EHsa" into the options text box. (The checkbox enabling exception 
	handling sets the equivalent of /EHcs -- no asynchronous exception support, 
	and any 'extern "C"'-funtion is assumed to never throw an exception. Now there 
	is what I call an optimistic assumption.)
*/