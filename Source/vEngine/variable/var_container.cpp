//-----------------------------------------------------------------------------
// File: var_container.cpp
// Auth: Lyp
// Date: 2004/04/02
// Last: 2004/11/17
//
// Desc: ��������
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "var_container.h"

#include "ini_obj.h"
#include "..\console\console_gui.h"

namespace vEngine {
//-----------------------------------------------------------------------------
// ɾ��ָ��Ԫ��
//-----------------------------------------------------------------------------
BOOL VarContainer::Erase(LPCTSTR szName)
{
	std::map<DWORD, tstring>::iterator it;
	DWORD dwID = m_pUtil->Crc32(szName);

	it = m_mapData.find(dwID);
	if( it == m_mapData.end() )
		return FALSE;	// �Ҳ���

	m_mapData.erase(it);

	if( m_bSupportSave )
	{
		it = m_mapName.find(dwID);
		m_mapName.erase(it);
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// ɾ��ָ��Ԫ��
//-----------------------------------------------------------------------------
BOOL VarContainer::Erase(LPCTSTR szName, LPCTSTR szField)
{
	tstring str = szName;
	str.append(_T(" "));
	str.append(szField);
	return Erase(str.c_str());
}


//-----------------------------------------------------------------------------
// ��ȡ����Ԫ�ص����Ʊ�ʾ
//-----------------------------------------------------------------------------
std::list<tstring>& VarContainer::GetVarName()
{
	m_listName.clear();
	std::map<DWORD, tstring>::iterator it;

	for( it=m_mapName.begin(); it!=m_mapName.end(); ++it )
		m_listName.push_back(it->second);

	return m_listName;
}


//-----------------------------------------------------------------------------
// ��ȡ�ļ�
//-----------------------------------------------------------------------------
BOOL VarContainer::Load(LPCSTR szVFS, LPCTSTR szFileName, LPCSTR szID_KeyName/* = "name"*/,
						std::list<tstring>* pFieldList/* = NULL*/)
{
	DWORD dwTime = timeGetTime();
	tstring strFileName = szFileName;
	if( strFileName.find(_T(".xml")) != tstring::npos )
	{
		XmlDocument doc;
		if( !doc.LoadFile(szVFS, szFileName) )
			return FALSE;

		XmlHandle docHandle( &doc );
		XmlHandle eleHandle = docHandle.FirstChildElement("root").FirstChildElement();
		XmlElement* pElement = eleHandle.Element();
		if( !pElement )	// �Ƿ���ϴ��̸�ʽ
			return FALSE;

		TCHAR szField[X_HUGE_STRING];
		TCHAR szValue[X_HUGE_STRING];
		TCHAR szName[X_HUGE_STRING];
		TCHAR szFullName[X_HUGE_STRING];
		while( pElement )	// ��������element
		{
			// ����element�ļ���Ҳ����field
			LPCSTR szKeyName = pElement->Attribute(szID_KeyName);
			if( !P_VALID(szKeyName) )
			{
				pElement = pElement->NextSiblingElement();
				continue;
			}

			m_pUtil->Unicode8ToUnicode(szKeyName, szField);
			XmlAttribute* pAttrib = pElement->FirstAttribute();
			if( pFieldList && pAttrib )	// ���Field��Ҫ����
				pFieldList->push_back(szField);

			while( pAttrib ) // ������field�е���������
			{
				if( pAttrib->ValueStr().empty() )	// ���������û��ֵ������
				{
					pAttrib = pAttrib->Next();
					continue;
				}

				m_pUtil->Unicode8ToUnicode(pAttrib->Value(), szValue);
				m_pUtil->Unicode8ToUnicode(pAttrib->Name(), szName);
				_tcscpy(szFullName, szName);
				_tcscat(szFullName, _T(" "));
				_tcscat(szFullName, szField);

				this->Add(szValue, szFullName);

				// �����value�������һ��ֱ����nameΪ���ı���
				if(    szName[0] == _T('v') 
					&& szName[1] == _T('a') 
					&& szName[2] == _T('l') 
					&& szName[3] == _T('u') 
					&& szName[4] == _T('e') 
					&& szName[5] == 0 )
					this->Add(szValue, szField);

				pAttrib = pAttrib->Next();
			}

			pElement = pElement->NextSiblingElement();
		}
	}
	else if (strFileName.find(_T(".inc")) != tstring::npos )
	{
		CreateObj("TempINI", "IniObj");
		if( FALSE == TObjRef<INIObj>("TempINI")->Load(szVFS, szFileName, 1) )
		{
			KillObj("TempINI");
			return FALSE;
		}
		TObjRef<INIObj>("TempINI")->PutToContainer(this);
		KillObj("TempINI");
	}
	else if (strFileName.find(_T(".ins")) != tstring::npos )
	{
		CreateObj("TempINI", "IniObj");
		if( FALSE == TObjRef<INIObj>("TempINI")->Load(szVFS, szFileName, 0) )
		{
			KillObj("TempINI");
			return FALSE;
		}
		TObjRef<INIObj>("TempINI")->PutToContainer(this);
		KillObj("TempINI");
	}
	else 
	{
		CreateObj("TempINI", "IniObj");
		if( FALSE == TObjRef<INIObj>("TempINI")->Load(szVFS, szFileName) )
		{
			KillObj("TempINI");
			return FALSE;
		}
		TObjRef<INIObj>("TempINI")->PutToContainer(this);
		KillObj("TempINI");
	}

	// IMSG(_T("load xml time=%d:%s\r\n"), timeGetTime()-dwTime, szFileName);
	return TRUE;
}



//-----------------------------------------------------------------------------
// �����ļ�
//-----------------------------------------------------------------------------
BOOL VarContainer::Save(LPCTSTR szFileName)
{
	if( !m_bSupportSave )
		return FALSE;

	tstring strFileName = szFileName;
	if( strFileName.find(_T(".xml")) != tstring::npos )
	{
		XmlDocument doc;
		XmlDeclaration declaration("1.0", "UTF-8", "");
		doc.InsertEndChild(declaration);
		
		XmlElement eleRoot("root");
		std::map<DWORD, tstring>::iterator it;
		std::map<DWORD, tstring>::iterator itName;
		for( it=m_mapData.begin(); it!=m_mapData.end(); ++it )
		{
			itName = m_mapName.find(it->first);
			std::string strName = m_pUtil->UnicodeToUnicode8(itName->second.c_str());
			std::string strValue = m_pUtil->UnicodeToUnicode8(it->second.c_str());
			
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
				if( 0 == strcmp(pElement->Attribute("name"), strField.c_str()) )
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
				ele.SetAttribute("name", strField);
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
BOOL VarContainer::Set( LPCTSTR szValue, LPCTSTR szName )
{
	BOOL bResult = this->Erase(szName);
	this->Add(szValue, szName);
	return bResult;
}


//-----------------------------------------------------------------------------
// set
//-----------------------------------------------------------------------------
BOOL VarContainer::Set( DWORD dwValue, LPCTSTR szName )
{
	BOOL bResult = this->Erase(szName);
	this->Add(dwValue, szName);
	return bResult;
}


//-----------------------------------------------------------------------------
// set
//-----------------------------------------------------------------------------
BOOL VarContainer::Set( FLOAT fValue, LPCTSTR szName )
{
	BOOL bResult = this->Erase(szName);
	this->Add(fValue, szName);
	return bResult;
}

} // namespace vEngine {
