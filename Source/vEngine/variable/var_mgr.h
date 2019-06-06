//-----------------------------------------------------------------------------
//!\file var_mgr.h
//!\brief var����������
//!
//!\date 2007-04-10
//! last 2007-04-10
//!\author Lyp
//!
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

enum EXDataType
{
	EXDT_Int=0,
	EXDT_Dword,
	EXDT_Float,
	EXDT_String,
	EXDT_Enum,
	EXDT_End
};

struct tagVarDataItem
{
	tstring					strName;	// ����
	EXDataType				eType;		// ��������
	std::vector<tstring>*	pVecEnum;	// ö�������б�
	LPVOID					pVar;		// �ڴ�λ��

	tagVarDataItem():eType(EXDT_Int),pVecEnum(0),pVar(0) {}
};

//------------------------------------------------------------------------------
//! \brief var����������
//!
//! - ��Ƹ���
//!  -#	��¼�������ָ�����ݵ�ָ���Լ������Ϣ
//!  -#	�������ݵ�����ID���ʴ˱�������Ϣ
//!  -#	���̵�ʱ��洢���ݵ�ʵ��ֵ
//------------------------------------------------------------------------------
class VENGINE_API VarMgr
{
public:
	BOOL Register(LPCTSTR szName, INT* pVar, LPCTSTR szEnmuText=NULL); // INT����ö��
	BOOL Register(LPCTSTR szName, DWORD* pVar);	// DWORD
	BOOL Register(LPCTSTR szName, FLOAT* pVar);	// Float
	BOOL Register(LPCTSTR szName, tagPoint* pVar);	// tagPoint
	BOOL Register(LPCTSTR szName, tagRect* pVar);	// tagRect
	BOOL Register(LPCTSTR szName, tstring* pVar); // std::string
	
	std::vector<tstring>* GetEnumText(LPCTSTR szName)
	{ return  (m_mapData.Peek(m_pUtil->Crc32(szName)))->pVecEnum; }

	VarMgr();~VarMgr();
	//------------------------------------------------------------------------------
	// ���������Ĳ���
	//------------------------------------------------------------------------------
	VOID Save(XmlElement* pXmlElement, BOOL bIgnoreNull=FALSE); //!< �洢
	BOOL Load(XmlElement* pXmlElement); //!< ���ȡ

private:
	TObjRef<VirtualFileSys>			m_pVFS;
	TObjRef<Util>					m_pUtil;

	TMap<DWORD, tagVarDataItem*>	m_mapData;
	TList<tagVarDataItem*>			m_listData;	// ������֤˳�������
};





}	// namespace vEngine





