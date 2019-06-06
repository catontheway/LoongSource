#include "StdAfx.h"
#include "GuildMembers.h"
#include "PlayerNameTab.h"
#include "..\WorldDefine\guild_define.h"

namespace
{
	/** \�����������
	*/
	class MembersNameSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			LPCTSTR szName1 = PlayerNameTab::Inst()->FindNameByID(pMember1->sInfo.dwRoleID);
			LPCTSTR	szName2 = PlayerNameTab::Inst()->FindNameByID(pMember2->sInfo.dwRoleID);

			return _tcscmp(szName1,szName2) < 0;
		}
	};
	/** \�ȼ��������
	*/
	class MembersLevelSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			return pMember1->sInfo.byLevel > pMember2->sInfo.byLevel;
		}
	};
	/** \ְλ�������
	*/
	class MembersPosSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			return pMember1->sInfo.n8GuildPos < pMember2->sInfo.n8GuildPos;
		}
	};
	/** \�Ա��������
	*/
	class MembersSexSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			return pMember1->sInfo.bySex > pMember2->sInfo.bySex;
		}
	};
	/** \ְҵ�������
	*/
	class MembersClassSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			return FALSE;
		}
	};
	/** \�Ƿ������������
	*/
	class MembersOnlineSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			if(pMember1->sInfo.bOnline == FALSE && pMember2->sInfo.bOnline == TRUE)
				return FALSE;

			if(pMember1->sInfo.bOnline == TRUE && pMember2->sInfo.bOnline == FALSE)
				return TRUE;
			
			if(pMember1->sInfo.bOnline == TRUE)
				return FALSE;
			
			return pMember1->sInfo.dwTimeLastLogout > pMember2->sInfo.dwTimeLastLogout;
		}
	};
	/** \�����������
	*/
	class MembersContributionSort
	{
	public:
		bool operator()(const tagGuildMemberClient* pMember1,const tagGuildMemberClient* pMember2)
		{
			return pMember1->sInfo.nCurContrib > pMember2->sInfo.nCurContrib;
		}
	};
}

GuildMembers::GuildMembers(void)
{
}

GuildMembers::~GuildMembers(void)
{
	FreeArray();
}

VOID GuildMembers::SortMember( EListColType eType )
{
	// ���ݸ�����������
	switch(eType)
	{
	case ELCT_Name:
		{
			MembersNameSort sortObj;
			sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	case ELCT_Pos:
		{
			//MembersPosSort sortObj;
			//sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	case ELCT_Level:
		{
			MembersLevelSort sortObj;
			sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	case ELCT_Sex:
		{
			MembersSexSort sortObj;
			sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	case ELCT_Class:
		{
			//MembersClassSort sortObj;
			//sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	case ELCT_Cont:
		{
			MembersContributionSort sortObj;
			sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	case ELCT_Online:
		{
			MembersOnlineSort sortObj;
			sort(m_Members.begin(), m_Members.end(), sortObj);
		}
		break;
	}
}

VOID GuildMembers::AddMember( const tagGuildMemInfo& sInfo )
{
	// ����һ���µĳ�Ա
	tagGuildMemberClient* pMember = new tagGuildMemberClient(sInfo);
	m_Members.push_back(pMember);
}

VOID GuildMembers::AddMember( const tagGuildMemInfo& sInfo, const tagGuildMemInfoEx& sInfoEx )
{
	// ����һ���µĳ�Ա
	tagGuildMemberClient* pMember = new tagGuildMemberClient(sInfo);
	// ������չ��Ϣ
	pMember->Modify(sInfoEx);
	// �����б�	
	m_Members.push_back(pMember);
}

VOID GuildMembers::ModifyMember( DWORD dwRoleID, const tagGuildMemInfoEx& sInfoEx )
{
	// ����ID���ҳ�Ա���޸ĸó�Ա����չ��Ϣ
	tagGuildMemberClient* pMember = FindMember(dwRoleID);
	if(P_VALID(pMember))
	{
		pMember->Modify(sInfoEx);
	}
}

VOID GuildMembers::ModifyMember( DWORD dwRoleID, const tagGuildMemInfoEx& sInfoEx, INT nIndex )
{
	// �����������ҳ�Ա
	tagGuildMemberClient* pMember = m_Members[nIndex];
	// У��������Ա�Ƿ���ͬ
	if(P_VALID(pMember) && pMember->sInfo.dwRoleID == dwRoleID)
	{
		// ��ͬ���޸ĳ�Ա����չ��Ϣ
		pMember->Modify(sInfoEx);
	}
	else
	{	
		// ��ͬ�����ID�޸ĳ�Ա
		ModifyMember(dwRoleID, sInfoEx);
	}
}

VOID GuildMembers::ModifyMember( const tagGuildMemInfo& sInfo, const tagGuildMemInfoEx& sInfoEx )
{
	// ����ID���ҳ�Ա���޸ĳ�Ա����ͨ��Ϣ����չ��Ϣ
	tagGuildMemberClient* pMember = FindMember(sInfo.dwRoleID);
	if(P_VALID(pMember))
	{
		pMember->Modify(sInfo, sInfoEx);
	}
	else
	{
		// ���û�ҵ��ó�Ա���������ó�Ա
		AddMember(sInfo);
		ModifyMember(sInfo.dwRoleID, sInfoEx);
	}
}

VOID GuildMembers::ModifyMember( const tagGuildMemInfo& sInfo, const tagGuildMemInfoEx& sInfoEx, INT nIndex )
{
	// �����������Ҹó�Ա
	tagGuildMemberClient* pMember = m_Members[nIndex];
	// У���Ա�Ƿ���ͬһ��
	if(P_VALID(pMember) && pMember->sInfo.dwRoleID == sInfo.dwRoleID)
	{
		// ��ͬ���޸ĸó�Ա����ͨ��Ϣ����չ��Ϣ
		pMember->Modify(sInfo, sInfoEx);
	}
	else
	{
		// ��ͬ�����ID�޸ĳ�Ա
		ModifyMember(sInfo, sInfoEx);
	}
}

VOID GuildMembers::RemoveMember( DWORD dwRoleID )
{
	for(MembersArray::iterator it = m_Members.begin(); 
		it != m_Members.end(); ++it)
	{
		// ����IDѰ�ҳ�Ա��ɾ���ó�Ա
		tagGuildMemberClient* pMember = *it;
		if(P_VALID(pMember) && pMember->sInfo.dwRoleID == dwRoleID)
		{
			m_Members.erase(it);
			SAFE_DEL(pMember);
			break;
		}
	}
}

tagGuildMemberClient* GuildMembers::FindMember( DWORD dwRoleID )
{
	// ѭ��������Ա�б�Ѱ��ID��ͬ�ĳ�Ա
	for(MembersArray::iterator it = m_Members.begin(); 
		it != m_Members.end(); ++it)
	{
		tagGuildMemberClient* pMember = *it;
		if(P_VALID(pMember) && pMember->sInfo.dwRoleID == dwRoleID)
			return pMember;
	}
	return NULL;
}

VOID GuildMembers::GetMembersList( const MembersArray** members ) const
{
	// ��ȡ��Ա�б�
	*members = &m_Members;
}

VOID GuildMembers::FreeArray()
{
	// �����Ա�б�
	for(MembersArray::iterator it = m_Members.begin(); 
		it != m_Members.end(); ++it)
	{
		tagGuildMemberClient* pMember = *it;
		SAFE_DEL(pMember);
	}
	m_Members.clear();
}

INT GuildMembers::FindIndex( DWORD dwRoleID )
{
	INT nIndex = 0;
	// ѭ��������Ա�б�Ѱ��ID��ͬ�ĳ�Ա
	for(MembersArray::iterator it = m_Members.begin(); 
		it != m_Members.end(); ++it)
	{
		tagGuildMemberClient* pMember = *it;
		if(P_VALID(pMember) && pMember->sInfo.dwRoleID == dwRoleID)
			return nIndex;
		// �ۼӼ�������
		++nIndex;
	}
	// û���򷵻�-1
	return GT_INVALID;
}