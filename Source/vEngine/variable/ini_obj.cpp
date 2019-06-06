//-----------------------------------------------------------------------------
//!\file ini_obj.cpp
//!\brief ini�ļ�����
//!
//!\date 2004-04-03
//! last 2004-04-03
//!\author Lyp
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "ini_obj.h"

// ʹ����������
#include "..\vfs\vfs.h"
#include "var_container.h"

namespace vEngine {
//-----------------------------------------------------------------------------
//!	construction
//-----------------------------------------------------------------------------
INIObj::INIObj()
{
	m_mapOffset.clear();
	m_mapSectionName.clear();
	m_pData = NULL;
	m_dwDataSize = 0;
}

//-----------------------------------------------------------------------------
//!	destruction
//-----------------------------------------------------------------------------
INIObj::~INIObj()
{
	Unload();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL INIObj::Load( LPCSTR szVFSName, LPCTSTR szFileName, const INT nMethod)
{
	if( m_pData )
		Unload();

	m_pVFS = szVFSName;

	// ʹ���ļ�ϵͳ��INIһ���Զ���
	m_dwDataSize = m_pVFS->GetSize(szFileName);
	if( GT_INVALID == m_dwDataSize )
		return FALSE;

	m_pData = (BYTE*)malloc(m_dwDataSize+sizeof(TCHAR)*2);
	if( NULL == m_pData )
		return FALSE;

	m_dwDataSize = m_pVFS->LoadFile(m_pData, szFileName);
	if( GT_INVALID == m_dwDataSize )
		return FALSE;

	// ���ڼ��ܵ�ini�ļ���ǰ�ĸ��ֽ�Ϊ��Կֵ��������Ҫ����Կ����һ�ν���
	if( GT_VALID(nMethod) )
	{
		ASSERT(1 == nMethod || 0 == nMethod);
		m_pUtil->Decrypt(m_pData+sizeof(INT), m_pData, m_dwDataSize-sizeof(INT), *(INT*)m_pData, nMethod);
		*(DWORD*)( m_pData + m_dwDataSize - sizeof(TCHAR)*2) = 0x0a000d00; // little-endian	
	}

	// ���ļ������ӻس����з�(�ܶ��ļ���������0d0a����)
	*(DWORD*)( m_pData + m_dwDataSize ) = 0x0a000d00; // little-endian

	// ���濪ʼ��������INI,��¼�����м��ͼ�ֵ��ƫ��,������m_mapOffset
	std::map<DWORD, VEC_DWORD> mapOneSection;
	tstring strTemp, strSectionName;
	INT nSectionOffset = GT_INVALID, nValueOffset = GT_INVALID, nKeyOffset = 0; 
	INT	nKeySize = 0;
	INT nCommentStart = GT_INVALID;
	TCHAR* pChar = (TCHAR*)m_pData;

	for( DWORD n=0; n<(m_dwDataSize+4)/sizeof(TCHAR); n++ )
	{
		switch( *(pChar+n) )
		{
		case _T('['):	// ������Ϣ��ʼ
			if( GT_INVALID != nCommentStart )
				continue;
			if( false == strSectionName.empty() )
			{
				// ����һ�������Ϣ��������
				DWORD dwID = m_pUtil->Crc32( strSectionName.c_str() );
				// ����û���ظ���ID(���ʷǳ��ǳ�С������Ϊ������������Ǽ��һ��)
				ASSERT( m_mapSectionName.find( dwID ) == m_mapSectionName.end() );
				m_mapSectionName.insert(
					std::pair<DWORD,tstring>(dwID, strSectionName) );
				m_mapOffset.insert( 
					std::pair<DWORD, MAP_VEC_DWORD>(dwID, mapOneSection) );
				mapOneSection.clear();
			}
			nSectionOffset = n+1;
			nKeyOffset = GT_INVALID;
			nValueOffset = GT_INVALID;
			break;

		case _T(']'):	// ������Ϣ����
			if( GT_INVALID != nCommentStart )
				continue;

			// ��¼���������
			if( nSectionOffset != GT_INVALID && n - nSectionOffset > 0 )
				strSectionName.assign(pChar+nSectionOffset,n-nSectionOffset);

			nSectionOffset = GT_INVALID;
			break;

		case _T('='):	// ����ֵ�ļ��
			if( GT_INVALID != nCommentStart )
				continue;
			if( GT_INVALID != nKeyOffset )
			{
				if( n - nKeyOffset > 0 )	// key�ĳ��Ȳ���Ϊ��
				{
					nKeySize = n - nKeyOffset;
					nValueOffset = n+1;
				}
				else
					nKeyOffset = GT_INVALID;
			}
			break;

		case _T(';'):	// ����ע��
			nCommentStart = n;
			break;

		case 0x0a:	// ��β
			if( GT_INVALID != nKeyOffset && GT_INVALID != nValueOffset )
			{
				// ��¼���еļ���ֵ��ƫ����
				std::vector<DWORD> vecTemp;	// �洢һ���е����е�Ԫ���ƫ��
				vecTemp.push_back( nKeyOffset );
				vecTemp.push_back( nKeySize );
				vecTemp.push_back( nValueOffset );
				if( GT_INVALID != nCommentStart )
					vecTemp.push_back( nCommentStart - nValueOffset );
				else
					vecTemp.push_back( n-1 - nValueOffset );
				// ��
				strTemp.assign(pChar+nKeyOffset, nKeySize);
				// tab���ɿո�
				while( strTemp.find(_T('\t')) != strTemp.npos )
					strTemp.replace(strTemp.find(_T('\t')), 1, 1, _T(' '));
				// ȥ��ͷβ�Ŀո����id
				if( strTemp.find_first_not_of(_T(' ')) != -1 )
					strTemp.assign(strTemp,strTemp.find_first_not_of(_T(' ')),
						strTemp.find_last_not_of(_T(' '))-strTemp.find_first_not_of(_T(' '))+1);
				DWORD dwID = m_pUtil->Crc32((LPCTSTR)strTemp.c_str());
				// ����û���ظ���ID(���ʷǳ��ǳ�С������Ϊ������������Ǽ��һ��)
				ASSERT( mapOneSection.find( dwID ) == mapOneSection.end() );
				mapOneSection.insert(std::pair<DWORD, VEC_DWORD>(dwID, vecTemp));
			}

			nSectionOffset = nValueOffset = GT_INVALID;
			nKeyOffset = n+1;
			nCommentStart = GT_INVALID;
			break;
		}
	}

	if( false == strSectionName.empty() )
	{
		// ����һ�������Ϣ��������
		DWORD dwID = m_pUtil->Crc32( strSectionName.c_str() );
		m_mapSectionName.insert(std::pair<DWORD,tstring>(dwID, strSectionName));
		m_mapOffset.insert(std::pair<DWORD, MAP_VEC_DWORD>(dwID, mapOneSection));
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID INIObj::Unload()
{
	free(m_pData);

	m_mapOffset.clear();
	m_mapSectionName.clear();
	m_pData = NULL;
	m_dwDataSize = 0;
}


//-----------------------------------------------------------------------------
//!\param dwSize ����value���ݴ�С
//!\return ָ��value���ݵ�PBYTEָ��
//-----------------------------------------------------------------------------
PBYTE INIObj::Read(DWORD& dwSize, LPCTSTR szKeyName, LPCTSTR szSectionName)
{
	ASSERT( szKeyName );
	ASSERT( szSectionName );

	if( NULL == m_pData )
		return NULL;

	std::map<DWORD, MAP_VEC_DWORD>::iterator it = m_mapOffset.begin();
	it = m_mapOffset.find(m_pUtil->Crc32(szSectionName));
	if( it == m_mapOffset.end() )
		return NULL;

	std::map<DWORD, VEC_DWORD>::iterator real_it;
	real_it = it->second.find(m_pUtil->Crc32(szKeyName));
	if( real_it == it->second.end() )
			return NULL;

	if( real_it->second.size() < 4 )	// ���������
		return NULL;

	// (real_it->second)[0]: key name offset
	// (real_it->second)[1]: key name size
	dwSize = (real_it->second)[3];	// (real_it->second)[3]: value size
	return (m_pData+(real_it->second)[2]); // (real_it->second)[2]: value offset
}


//-----------------------------------------------------------------------------
//! �����ݴ�������
//!\param ����������
//!\return �Ƿ�ɹ�
//-----------------------------------------------------------------------------
BOOL INIObj::PutToContainer(VarContainer* pVarContainer)
{
	if( !pVarContainer )
		return FALSE;

	INT nTemp = 0;
	DWORD dwKeySize = 0, dwValueSize = 0;
	tstring strKey, strValue;
	std::map<DWORD, MAP_VEC_DWORD>::iterator it;
	std::map<DWORD, VEC_DWORD>::iterator real_it;
	std::map<DWORD, tstring>::iterator section_it;
	TCHAR* pText = (TCHAR*)m_pData;

	for( it=m_mapOffset.begin(); it!=m_mapOffset.end(); ++it )
	{
		section_it = m_mapSectionName.find( it->first );
		if( section_it == m_mapSectionName.end() )
			return FALSE;	// �Ҳ�����Ӧ�����������ַ���

		for( real_it=it->second.begin(); real_it!=it->second.end(); ++real_it )
		{
			// �õ�key������
			// (real_it->second)[1]: key name size
			dwKeySize = (real_it->second)[1];	
			// (real_it->second)[0]: key name offset
			strKey.assign((LPCTSTR)(pText+(real_it->second)[0]), dwKeySize);

			// tab���ɿո�
			while( strKey.find(_T('\t')) != strKey.npos )
				strKey.replace(strKey.find(_T('\t')), 1, 1, _T(' '));
			// ȥ��ͷβ�Ŀո�
			m_pUtil->CutSpaceOfStringHeadAndTail(strKey);
			// ��������������
			strKey += _T(" ") + section_it->second;	
			
			// (real_it->second)[3]: value size
			dwValueSize = (real_it->second)[3];	
			strValue.assign((LPCTSTR)(pText+(real_it->second)[2]), dwValueSize);

			// tab���ɿո�
			while( strValue.find(_T('\t')) != strValue.npos )
				strValue.replace(strValue.find(_T('\t')), 1, 1, _T(' '));
			// ȥ��ͷβ�Ŀո�
			m_pUtil->CutSpaceOfStringHeadAndTail(strValue);
			// ��ӵ�������ȥ
			pVarContainer->Add(strValue.c_str(), strKey.c_str());
		}
	}

	return TRUE;
}




}	// namespace vEngine {
