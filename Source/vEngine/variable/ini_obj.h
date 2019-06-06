//-----------------------------------------------------------------------------
//!\file ini_obj.h
//!\brief ini�ļ�����
//!
//!\date 2004-04-03
//! last 2004-04-03
//!\author Lyp
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {


//------------------------------------------------------------------------------
//! \brief ini�ļ�����.
//!
//! - ��ƹ���
//!  -#	ini�ļ���һ���������ʽ����������ѵ�һ�ж�ΪID(�����CRC32��ΪID)
//!  -#	����ÿһ����ID���ʴ˼�
//!  -#	��ini�ļ�loadʱ��ͻ������section,key,value��ƫ����
//------------------------------------------------------------------------------
class INIObj
{
public:
	BOOL Load(LPCSTR szVFSName, LPCTSTR szFileName, const INT nMethod = GT_INVALID);	//!< װ��ini�ļ�
	VOID Unload();	//!< ж��ini�ļ�

	//! �õ�ָ����������
	PBYTE Read(DWORD& dwSize, LPCTSTR szKeyName, LPCTSTR szSectionName);
	//! �õ��ܹ�����
	INT GetRowNum() { return m_mapOffset.size(); }

	//! �����ݴ���ָ��container
	BOOL PutToContainer(VarContainer* pVarContainer);

	INIObj();~INIObj();

private:
	TObjRef<VirtualFileSys>			m_pVFS;
	TObjRef<Util>					m_pUtil;

	typedef std::vector<DWORD>	VEC_DWORD;
	typedef	std::map<DWORD, VEC_DWORD>	MAP_VEC_DWORD;

	std::map<DWORD, MAP_VEC_DWORD>	m_mapOffset;
	std::map<DWORD, tstring>		m_mapSectionName;
	PBYTE							m_pData; //!< ini ���ݿ�
	DWORD							m_dwDataSize; //!< ini ���ݿ��С

};







}	// namespace vEngine
