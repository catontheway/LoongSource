#pragma once

struct tagGuildMemberClient;
struct tagGuildMemInfo;
struct tagGuildMemInfoEx;

class GuildMembers
{
	friend class GuildMgr;

public:
	typedef vector<tagGuildMemberClient*> MembersArray;

	enum EListColType
	{
		ELCT_Name = 0,		// �������
		ELCT_Pos,			// ���ְλ
		ELCT_Level,			// ��ҵȼ�
		ELCT_Sex,			// ����Ա�
		ELCT_Class,			// ���ְҵ
		ELCT_Cont,			// ��ҹ���
		ELCT_Online,		// ����Ƿ�����

		ELCT_End,
	};

public:
	GuildMembers(void);
	~GuildMembers(void);
	
	/** \���ݲ�ͬ����������
		\param eTypeΪ������
	*/
	VOID SortMember(EListColType eType);
	/** \����һ����Ա
		\param sInfoΪ�ó�Ա�Ļ�����Ϣ
	*/
	VOID AddMember(const tagGuildMemInfo& sInfo);
	/** \����һ����Ա
		\param sInfoΪ�ó�Ա�Ļ�����Ϣ
		\param sInfoExΪ�ó�Ա����չ��Ϣ
	*/
	VOID AddMember(const tagGuildMemInfo& sInfo, const tagGuildMemInfoEx& sInfoEx);
	/** \�޸�һ����Ա
		\param dwRoleIDΪ�ó�ԱID
		\param sInfoExΪ�ó�Ա����չ��Ϣ
	*/
	VOID ModifyMember(DWORD dwRoleID, const tagGuildMemInfoEx& sInfoEx);
	/** \�޸�һ����Ա
		\param dwRoleIDΪ�ó�ԱID
		\param sInfoExΪ�ó�Ա����չ��Ϣ
		\param nIndexΪ�ó�Ա��λ��������������Ҹó�Ա��
	*/
	VOID ModifyMember(DWORD dwRoleID, const tagGuildMemInfoEx& sInfoEx, INT nIndex);
	/** \�޸�һ����Ա
		\param sInfoΪ�ó�Ա�Ļ�����Ϣ
		\param sInfoExΪ�ó�Ա����չ��Ϣ
	*/
	VOID ModifyMember(const tagGuildMemInfo& sInfo, const tagGuildMemInfoEx& sInfoEx);
	/** \�޸�һ����Ա
		\param sInfoΪ�ó�Ա�Ļ�����Ϣ
		\param sInfoExΪ�ó�Ա����չ��Ϣ
		\param nIndexΪ�ó�Ա��λ��������������Ҹó�Ա��
	*/
	VOID ModifyMember(const tagGuildMemInfo& sInfo, const tagGuildMemInfoEx& sInfoEx, INT nIndex);
	/** \ɾ��һ����Ա
		\param dwRoleIDΪ�ó�Ա��ID
	*/
	VOID RemoveMember(DWORD dwRoleID);
	/** \��ȡ��Ա�б�
		\param membersΪ����ĳ�Ա�б�
	*/
	VOID GetMembersList(const MembersArray** members) const;

private:
	/** \����ID����һ����Ա
		\param dwRoleIDΪ�ó�Ա��ID
	*/
	tagGuildMemberClient* FindMember(DWORD dwRoleID);
	/** \����ID��������
		\param dwRoleIDΪ�ó�Ա��ID
	*/
	INT FindIndex(DWORD dwRoleID);
	/** \�����Ա�б�
	*/
	VOID FreeArray();

private:
	MembersArray	m_Members;		// ��Ա�б�
	
};
