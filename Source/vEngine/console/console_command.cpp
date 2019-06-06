//-----------------------------------------------------------------------------
//!\file console_command.cpp
//!\author Lyp
//!
//!\date 2004-06-21
//! last 2004-06-21
//!
//!\brief ����̨���
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "console_command.h"



namespace vEngine {
typedef DWORD (WINAPI* FP0)();
typedef DWORD (WINAPI* FP1)(DWORD);
typedef DWORD (WINAPI* FP2)(DWORD, DWORD);
typedef DWORD (WINAPI* FP3)(DWORD, DWORD, DWORD);
typedef DWORD (WINAPI* FP4)(DWORD, DWORD, DWORD, DWORD);
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
ConsoleCommand::ConsoleCommand():m_Trunk(this)
{
	m_fpOutputFun = m_Trunk.sfp1(&vEngine::ConsoleCommand::DefaultOutput);
	m_fpUnhandleFun = 0;
	
	// ��ע�Ἰ���Դ��Ļ�������
	Register( _T("help"), m_Trunk.sfp1(&vEngine::ConsoleCommand::Help), _T("this command") );
	Register( _T("quit"), m_Trunk.sfp1(&vEngine::ConsoleCommand::QuitApplication), _T("quit application") );
	Register( _T("err"),  m_Trunk.sfp1(&vEngine::ConsoleCommand::ShowLastError), _T("show last error") );
	// Register( _T("exit"), m_Trunk.sfp1(&vEngine::ConsoleCommand::ExitApplication), _T("shut down application") );
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
ConsoleCommand::~ConsoleCommand()
{
}


//-----------------------------------------------------------------------------
// ִ�� command
//-----------------------------------------------------------------------------
DWORD ConsoleCommand::Excute(LPCTSTR szCommand)
{
	std::vector<tstring> command;
	m_pUtil->StringToToken(command, szCommand);
	command.resize(10);	// �����ܳ���10������
	std::vector<DWORD> vp;

	if( command.size() == 0 )
		return GT_INVALID;

	// �õ�ʵ�ʵĲ���
	for( INT n=0; n<(INT)command.size(); n++ )
		vp.push_back((DWORD)command[n].c_str());

	transform(command[0].begin(), command[0].end(), command[0].begin(), tolower);	
	DWORD dwID = m_pUtil->Crc32(command[0].c_str());	// תΪСд������

	std::map<DWORD, LPVOID>::iterator iter;
	iter = m_mapCommandTable.find(dwID);	// ���Ҷ�Ӧ����
	if( iter != m_mapCommandTable.end() )
	{
		// ���Ҷ�Ӧ������������
		std::map<DWORD, INT>::iterator iter2;
		iter2 = m_mapCommandParam.find(dwID);
		INT nResult = 0;
		if( iter2 != m_mapCommandParam.end() )
		{
			switch(iter2->second)
			{
			case 1: 
				if( vp.size() <= 2 )
					nResult = ((FP1)iter->second)(vp[1]);
				else
				{
					tstring strParam;	// ������в�����һ��
					for(INT n=1; n<(INT)vp.size(); n++)
					{
						strParam += (LPCTSTR)(vp[n]);
						strParam += _T(" ");
					}
					nResult = ((FP1)iter->second)((DWORD)strParam.c_str());
				}
				break;
			case 2: nResult = ((FP2)iter->second)(vp[1], vp[2]);
				break;
			case 3: nResult = ((FP3)iter->second)(vp[1], vp[2], vp[3]); 
				break;
			case 4: nResult = ((FP4)iter->second)(vp[1], vp[2], vp[3], vp[4]);
				break;
			default: nResult = ((FP0)iter->second)();
				break;
			}

			if( command[0] != _T("cls") )	// cls ����ʾ����ֵ
			{
				tstringstream stream;
				stream << command[0] << _T(" returned ") << nResult << std::endl;
				(m_fpOutputFun)((DWORD)stream.str().c_str());
			}

			return nResult;
		}
	}

	if( P_VALID(m_fpUnhandleFun) )
	{
		(m_fpUnhandleFun)((DWORD)szCommand);
	}
	else
	{
		command[0].append( _T(" is unknow command\r\n"));
		(m_fpOutputFun)((DWORD)command[0].c_str());
	}

	return GT_INVALID;	// �Ҳ���ָ������
}

//-----------------------------------------------------------------------------
// register command functions
//-----------------------------------------------------------------------------
VOID ConsoleCommand::Register(LPCWSTR szCommandName, LPVOID fpCommandFunction,
							   LPCWSTR szCommandDesc, INT nCommandParam)
{
	DWORD dwID = m_pUtil->Crc32(szCommandName);
	m_mapCommandTable.insert(std::pair<DWORD, LPVOID>(dwID, fpCommandFunction));
	m_mapCommandParam.insert(std::pair<DWORD, INT>(dwID, nCommandParam));

	// �������Լ��������������б�
	tstringstream stream;
	stream << m_listCommand.size() + 1 << _T(" ") << szCommandName << _T(" : ");

	if( szCommandDesc )
		stream << szCommandDesc << std::endl;
	else
		stream << _T("no description\r\n");
	
	m_listCommand.push_back(stream.str());
}



//-----------------------------------------------------------------------------
// ��ʾ���е������Լ�����
//-----------------------------------------------------------------------------
DWORD ConsoleCommand::Help(DWORD)
{
	m_it = m_listCommand.begin();
	for(; m_it!=m_listCommand.end(); ++m_it)
		(m_fpOutputFun)((DWORD)(*m_it).c_str());
	
	return m_listCommand.size();
}


//-----------------------------------------------------------------------------
// ����Ӧ�ó���
//-----------------------------------------------------------------------------
DWORD ConsoleCommand::QuitApplication(DWORD)
{
	PostQuitMessage(0);
	return 0;
}


//-----------------------------------------------------------------------------
// ǿ�н���Ӧ�ó���
//-----------------------------------------------------------------------------
DWORD ConsoleCommand::ExitApplication(DWORD)
{
	exit(1);
	return 0;
}



//-----------------------------------------------------------------------------
// ��ʾ���Ĵ���
//-----------------------------------------------------------------------------
DWORD ConsoleCommand::ShowLastError(DWORD)
{
	TCHAR szBuf[1024];
	
	// ���Ȼ��ϵͳ�������
	DWORD dwErr = ::GetLastError();

	LPTSTR lpMsgBuf;
	if( dwErr != 0 )	// ��ϵͳ����õ�������Ϣ
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 
			| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf, 0, NULL);
		wsprintf(szBuf, _T("system code:%lu %s"), dwErr, lpMsgBuf );
		LocalFree( lpMsgBuf );

		(m_fpOutputFun)((DWORD)szBuf);
	}
	return 0;
}



//-----------------------------------------------------------------------------
// Ĭ�ϵ��������
//-----------------------------------------------------------------------------
DWORD ConsoleCommand::DefaultOutput(LPCTSTR szText)
{
	OutputDebugString(szText);
	return 0;
}

} // namespace vEngine {
