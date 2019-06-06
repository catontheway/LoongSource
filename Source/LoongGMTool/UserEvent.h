struct tagUserEvent
{
	DWORD		dwNameCrc;
	tstring		strEventName;

	tagUserEvent(LPCTSTR szEventName)
	{
		strEventName	= szEventName;
		dwNameCrc		= Crc32(szEventName);
	}
};

//��¼
struct tagLoginEvent : public tagUserEvent
{
	DWORD		dwErrorCode;

	tagLoginEvent(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

struct tagServerStatInfoEvent : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagServerStatInfoEvent(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//GM����
struct tagGMCreate : tagUserEvent
{
	DWORD		dwErrorCode;
	tagGMCreate(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//GM�޸�
struct tagGMModify : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagGMModify(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//GMɾ��
struct tagGMDelete : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagGMDelete(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//��ɫ�ָ�
struct tagRoleResume : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagRoleResume(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//��ɫ�����޸�
struct tagRoleProModify : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagRoleProModify(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

struct tagTimeLimited : public tagUserEvent
{
	DWORD		dwType;
	DWORD		dwErrorCode;
	DWORD		dwWorldID;
	tagTimeLimited(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//˫��
//struct tagDouble : public tagUserEvent
//{
//	DWORD			dwErrorCode;
//	DWORD			dwWorldID;
//	tagDouble(LPCTSTR szEventName) : tagUserEvent(szEventName)
//	{
//		dwErrorCode		= 0;
//		dwWorldID		= 0;
//	}
//};

//���½ǹ���
//struct tagNotice : public tagUserEvent
//{
//	DWORD			dwErrorCode;
//	DWORD			dwWorldID;
//	tagNotice(LPCTSTR szEventName) : tagUserEvent(szEventName)
//	{
//		dwErrorCode		= 0;
//		dwWorldID		= 0;
//	}
//};

//�Զ�ѭ������
//struct tagAutoNotice : public tagUserEvent
//{
//	DWORD			dwErrorCode;
//	DWORD			dwWorldID;
//	tagAutoNotice(LPCTSTR szEventName) : tagUserEvent(szEventName)
//	{
//		dwErrorCode		= 0;
//		dwWorldID		= 0;
//	}
//};

//�����
//struct tagCastRun : public tagUserEvent
//{
//	DWORD		dwErrorCode;
//	tagCastRun(LPCTSTR szEventName) : tagUserEvent(szEventName)
//	{
//		dwErrorCode		= 0;
//	}
//};	

//��������
struct tagItemCreate : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagItemCreate(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//����ɾ��
struct tagItemDelete : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagItemDelete(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//���߻ָ�
struct tagItemResume : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagItemResume(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};


//�˺ŷ�ͣ/���
struct tagAccountSuspend : public tagUserEvent
{
	DWORD		dwErrorCode;
	TCHAR		szAccount[32];
	tagAccountSuspend(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//����/ɾ���������е�IP
struct tagIPBlacklist : public tagUserEvent
{
	DWORD		dwErrorCode;
	tagIPBlacklist(LPCTSTR szEventName) : tagUserEvent(szEventName)
	{
		dwErrorCode		= 0;
	}
};

//�˺Ž��
//struct tagAccountUnsuspend : public tagUserEvent
//{
//	DWORD		dwErrorCode;
//	LPCTSTR		szAccount;
//	tagAccountUnsuspend(LPCTSTR szEventName) : tagUserEvent(szEventName)
//	{
//		szAccount = NULL;
//		dwErrorCode		= 0;
//	}
//};