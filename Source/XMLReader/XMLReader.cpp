#include "stdafx.h"
#include "XMLReader.h"

#include "tinyxml.h"

//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
XMLReader::XMLReader()
{
}

XMLReader::~XMLReader()
{
}

//-----------------------------------------------------------------------------
// ����ַ���Ԫ��
//-----------------------------------------------------------------------------
BOOL XMLReader::Add(LPCTSTR pStr, LPCTSTR szName)
{
	assert(szName);
	std::map<tstring, tstring>::iterator it;
	tstring title = szName;

	it = m_mapData.find(title);
	if( it != m_mapData.end() )
	{
		return FALSE;
	}
	
	tstring str = pStr;
	m_mapData.insert(std::pair<tstring, tstring>(title, str));
	return TRUE;
}


//-----------------------------------------------------------------------------
// ���DWORDԪ��
//-----------------------------------------------------------------------------
BOOL XMLReader::Add(DWORD dwData, LPCTSTR szName)
{
	TCHAR mess[MAX_STR_LEN];
	_itot(dwData, mess, 10);
	return this->Add(mess, szName);
}


//-----------------------------------------------------------------------------
// ���FLOATԪ��
//-----------------------------------------------------------------------------
BOOL XMLReader::Add(FLOAT fData, LPCTSTR szName)
{
	CHAR mess[MAX_STR_LEN];
	_gcvt(fData, 10, mess);
	return this->Add(Unicode8ToUnicode( mess ), szName);
}



//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR XMLReader::GetString(LPCTSTR szName)
{
	assert(szName);
	std::map<tstring, tstring>::iterator it;
	tstring title = szName;

	it = m_mapData.find(title);
	if( it == m_mapData.end() )
	{
		return NULL;	// �Ҳ���
	}

	LPCTSTR pReturn = it->second.c_str();
	return pReturn;
}


//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD XMLReader::GetDword(LPCTSTR szName)
{
	tstring str = this->GetString(szName);
	CutSpaceOfStringHeadAndTail(str);	// ȥ��ͷβ�Ŀո�
	return _tcstol(str.c_str(), NULL, 10);
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT XMLReader::GetFloat(LPCTSTR szName)
{
	tstring str = this->GetString(szName);
	CutSpaceOfStringHeadAndTail(str);	// ȥ��ͷβ�Ŀո�
	return (FLOAT)_tstof(str.c_str());
}


//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR XMLReader::GetString(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str += _T(" ");
	str += szField;
	return GetString(str.c_str());
}

//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD XMLReader::GetDword(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str += _T(" ");
	str += szField;
	return GetDword(str.c_str());
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT XMLReader::GetFloat(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str += _T(" ");
	str += szField;
	return GetFloat(str.c_str());
}



//-----------------------------------------------------------------------------
// ��ȡ�ַ���Ԫ��
//-----------------------------------------------------------------------------
LPCTSTR XMLReader::GetString(LPCTSTR szName, LPCTSTR szField, LPCTSTR szDefault)
{
	std::map<tstring, tstring>::iterator it;
	tstring title = szName;
	if( NULL != szField )
	{
		title += _T(" ");
		title += szField;
	}

	it = m_mapData.find(title);
	if( it == m_mapData.end() )
	{
		return szDefault;	// �Ҳ���
	}

	LPCTSTR pReturn = it->second.c_str();
	return pReturn;
}


//-----------------------------------------------------------------------------
// ��ȡ����Ԫ��
//-----------------------------------------------------------------------------
DWORD XMLReader::GetDword(LPCTSTR szName, LPCTSTR szField, DWORD dwDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szField, NULL);
	if( NULL == szReturn )
		return dwDefault;
	
	tstring str = szReturn;
	CutSpaceOfStringHeadAndTail(str);	// ȥ��ͷβ�Ŀո�
	return _tcstol(str.c_str(), NULL, 10);
}


//-----------------------------------------------------------------------------
// ��ȡ��������Ԫ��
//-----------------------------------------------------------------------------
FLOAT XMLReader::GetFloat(LPCTSTR szName, LPCTSTR szField, FLOAT fDefault)
{
	LPCTSTR szReturn = this->GetString(szName, szField, NULL);
	if( NULL == szReturn )
		return fDefault;

	tstring str = szReturn;
	CutSpaceOfStringHeadAndTail(str);	// ȥ��ͷβ�Ŀո�
	return (FLOAT)_tstof(str.c_str());
}



//-----------------------------------------------------------------------------
// ɾ��ָ��Ԫ��
//-----------------------------------------------------------------------------
BOOL XMLReader::Erase(LPCTSTR szName)
{
	assert(szName);
	std::map<tstring, tstring>::iterator it;
	tstring title = szName;

	it = m_mapData.find(title);
	if( it == m_mapData.end() )
	{
		return FALSE;	// �Ҳ���
	}
	m_mapData.erase(it);
	return TRUE;
}


//-----------------------------------------------------------------------------
// ɾ��ָ��Ԫ��
//-----------------------------------------------------------------------------
BOOL XMLReader::Erase(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str += _T(" ");
	str += szField;
	return Erase(str.c_str());
}


//-----------------------------------------------------------------------------
// ��ȡ����Ԫ�ص����Ʊ�ʾ
//-----------------------------------------------------------------------------
std::list<tstring>& XMLReader::GetVarName()
{
	m_listName.clear();
	std::map<tstring, tstring>::iterator it;

	DWORD dwNum = 0;
	for( it=m_mapData.begin(); it!=m_mapData.end(); ++it )
	{
		m_listName.push_back(it->first);
		dwNum++;
	}

	return m_listName;
}


//-----------------------------------------------------------------------------
// ��ȡ�ļ�
//-----------------------------------------------------------------------------
BOOL XMLReader::Load(IFS *pFS, LPCTSTR szFileName, LPCSTR szKeyName/* = "name"*/, std::list<tstring>* pFieldList/* = NULL*/)
{
	tstring strFileName = szFileName;
	if( strFileName.find(_T(".xml")) != tstring::npos )
	{
		XmlDocument doc;
		if( !doc.LoadFile(pFS, szFileName) )
			return FALSE;

		XmlHandle docHandle( &doc );
		XmlHandle eleHandle = docHandle.FirstChildElement("root").FirstChildElement();
		XmlElement* pElement = eleHandle.Element();
		if( NULL == pElement )	// �Ƿ���ϴ��̸�ʽ
			return FALSE;

		while( NULL != pElement )	// ������������
		{
			LPCSTR szField = pElement->Attribute(szKeyName);
			if( NULL == szField )
				continue;

			tstring strField = Unicode8ToUnicode(szField);
			XmlAttribute* pAttrib = pElement->FirstAttribute();
			if( pFieldList && pAttrib )	// ���Field��Ҫ����
			{
				pFieldList->push_back(strField);
			}

			while(pAttrib)
			{
				tstring strName = Unicode8ToUnicode(pAttrib->Name());
				tstring strValue = Unicode8ToUnicode(pAttrib->Value());
				tstring strFullName = strName + _T(" ") + strField;

				this->Add(strValue.c_str(), strFullName.c_str());

				// �����value�������һ��ֱ����nameΪ���ı���
				if( strName == _T("value") )
					this->Add(strValue.c_str(), strField.c_str());

				pAttrib = pAttrib->Next();
			}

			pElement = pElement->NextSiblingElement();
		}
	}


	return TRUE;
}



//-----------------------------------------------------------------------------
// �����ļ�
//-----------------------------------------------------------------------------
BOOL XMLReader::Save(LPCTSTR szFileName,LPCSTR szID_KeyName)
{
	tstring strFileName = szFileName;
	if( strFileName.find(_T(".xml")) != tstring::npos )
	{
		XmlDocument doc;
		XmlDeclaration declaration("1.0", "UTF-8", "");
		doc.InsertEndChild(declaration);
		
		XmlElement eleRoot("root");
		
		std::map<tstring, tstring>::iterator it;
		for( it=m_mapData.begin(); it!=m_mapData.end(); ++it )
		{
			std::string strName = UnicodeToUnicode8(it->first.c_str());
			std::string strValue = UnicodeToUnicode8(it->second.c_str());
			
			if( strName.find(" ") == std::string::npos )
				continue;

			std::string strField;
			strField.assign(strName, strName.find(" ")+1, strName.size()-strName.find(" ")+1);
			strName.assign(strName, 0, strName.find(" "));

			// �ҵ���Ӧ��ele
			BOOL bFound = FALSE;
			XmlElement* pElement = eleRoot.FirstChildElement();
			while( pElement )
			{
				if( 0 == strcmp(pElement->Attribute(szID_KeyName), strField.c_str()) )
				{
					pElement->SetAttribute(strName, strValue);
					bFound = TRUE;
					break;
				}

				pElement = pElement->NextSiblingElement();
			}
			
			if( !bFound )
			{
				XmlElement ele("item");
				ele.SetAttribute(szID_KeyName, strField);
				ele.SetAttribute(strName, strValue);
				eleRoot.InsertEndChild(ele);
			}
		}

		doc.InsertEndChild(eleRoot);
		doc.SaveFile(szFileName);

	}
	else
	{
		return FALSE; // INIObj��֧��д
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// set
//-----------------------------------------------------------------------------
BOOL XMLReader::Set( LPCTSTR szValue, LPCTSTR szName )
{
	BOOL bResult = this->Erase(szName);
	this->Add(szValue, szName);
	return bResult;
}


//-----------------------------------------------------------------------------
// set
//-----------------------------------------------------------------------------
BOOL XMLReader::Set( DWORD dwValue, LPCTSTR szName )
{
	BOOL bResult = this->Erase(szName);
	this->Add(dwValue, szName);
	return bResult;
}


//-----------------------------------------------------------------------------
// set
//-----------------------------------------------------------------------------
BOOL XMLReader::Set( FLOAT fValue, LPCTSTR szName )
{
	BOOL bResult = this->Erase(szName);
	this->Add(fValue, szName);
	return bResult;
}

//-----------------------------------------------------------------------------
// ȥ��tstring��β�Ŀո�
//-----------------------------------------------------------------------------
VOID XMLReader::CutSpaceOfStringHeadAndTail(tstring& string)
{
	// ������:����ַ�����ֻ�пո�,���ܽ��д˲���
	if( string.find_first_not_of(_T(" ")) != -1 )
	{
		string.assign(string, string.find_first_not_of(_T(" ")),
			string.find_last_not_of(_T(" "))-string.find_first_not_of(_T(" "))+1);
	}
	else
	{
		string.clear();
	}
}

//-----------------------------------------------------------------------------
// ��unicodeת��unicode8
//-----------------------------------------------------------------------------
LPCSTR XMLReader::UnicodeToUnicode8(LPCTSTR szSrc)
{
	m_pCharPool[0] = 0;
	WideCharToMultiByte(CP_UTF8, 0, szSrc, -1, m_pCharPool, MAX_STR_LEN, NULL, NULL);
	return m_pCharPool;
}

//-----------------------------------------------------------------------------
// ��unicode8ת��unicode
//-----------------------------------------------------------------------------
LPCTSTR	XMLReader::Unicode8ToUnicode(LPCSTR szSrc)
{
	m_pTCharPool[0] = 0;
	MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, m_pTCharPool, MAX_STR_LEN);
	return m_pTCharPool;
}