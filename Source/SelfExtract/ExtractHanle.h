#pragma once

#include "CPKCore/CPKVfs.h"
class CTSMemMgr;
#include "MemMgr/ts_memmgr.h"

//�����ֶ������ļ��������ļ�������cpk�ļ�����Ϣ
class CExtractHanle : public CPKVfs
{
public:
	CExtractHanle(CTSMemMgr*);

public:

	//�����ֶ������ļ�����ȡ�ϲ����ļ����cpk�ļ���ͷ��Ϣ
	BOOL LoadExtractFile( const char* szFileName );

	//���汾�Ƿ��ڴ��ֶ����³���ķ�Χ��
	BOOL IsVersionCorrect( std::string& strVersion );

	// ȥ��tstring��β�Ŀո�
	VOID CutSpaceOfStringHeadAndTail(std::string& string);

	const std::string& GetVersionDest() { return m_strVersionDest; }

private:

	BOOL	ReadCPkHeader();

	DWORD	m_dwCPKStartAddress;			//cpk�ļ�����ʼ��ַ

	std::string m_strVersionSrc;
	std::string m_strVersionDest;
};
