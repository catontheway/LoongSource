#pragma once
#pragma pack(push,1)
//-----------------------------------------------------------------
// ������������ ���鶨������ʹ�ô�
//-----------------------------------------------------------------
enum ECLanType
{
	ECLT_NULL			= -1,
	ECLT_BEGIN			= 0,

	ECLT_XuanYuan		= 0,	//��ԯ
	ECLT_ShenNong		= 1,	//��ũ
	ECLT_FuXi			= 2,	//����
	ECLT_SanMiao		= 3,	//����
	ECLT_JiuLi			= 4,	//����
	ECLT_YueZhi			= 5,	//����
	ECLT_NvWa			= 6,	//Ů�
	ECLT_GongGong		= 7,	//����

	ECLT_END				= 8,	
	ECLT_NUM				= ECLT_END - ECLT_BEGIN,
};

//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
enum EReputationType
{
	ERT_NULL			= -1,
	ERT_BEGIN			= 20,

	ERT_XuanYuan		= 20,	//��ԯ
	ERT_ShenNong		= 21,	//��ũ
	ERT_FuXi			= 22,	//����
	ERT_SanMiao			= 23,	//����
	ERT_JiuLi			= 24,	//����
	ERT_YueZhi			= 25,	//����
	ERT_NvWa			= 26,	//Ů�
	ERT_GongGong		= 27,	//����

	ERT_END				= 28,
	ERT_NUM				= ERT_END - ERT_BEGIN,
};
//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
enum ERContributionType
{
	ERCT_NULL			= -1,
	ERCT_BEGIN			= 120,

	ERCT_XuanYuan		= 120,	//��ԯ
	ERCT_ShenNong		= 121,	//��ũ
	ERCT_FuXi			= 122,	//����
	ERCT_SanMiao		= 123,	//����
	ERCT_JiuLi			= 124,	//����
	ERCT_YueZhi			= 125,	//����
	ERCT_NvWa			= 126,	//Ů�
	ERCT_GongGong		= 127,	//����

	ERCT_END			= 128,
	ERCT_NUM			= ERCT_END - ERCT_BEGIN,
};

//-----------------------------------------------------------------
// �����׶�
//-----------------------------------------------------------------
enum EReputationLevel
{
	ERL_NULL		= -1,
	ERL_BEGIN		= 0,

	ERL_Hostile		= 0,	//����
	ERL_Hated		= 1,	//�Ӷ�
	ERL_Callous		= 2,	//��Į
	ERL_Neutrality	= 3,	//����
	ERL_Friendly	= 4,	//����
	ERL_Intimate	= 5,	//����
	ERL_Reliable	= 6,	//����
	ERL_Respected	= 7,	//��
	ERL_Worshipped	= 8,	//���
	ERL_Legend		= 9,	//����

	ERL_END			= 10,
	ERL_NUM			= ERL_END - ERL_BEGIN,

};

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
struct tagReputationData
{
	INT32			nrValue[ECLT_NUM];				//����ֵ
	INT32			ncValue[ECLT_NUM];				//����ֵ
	BYTE			nActiveCount[ECLT_NUM];			//ʣ�������䱦�������
	BYTE			bisFame[ECLT_NUM];              //1������
	tagReputationData()	{	ZeroMemory(this, sizeof(*this));	}
};

#define MTRANS_ERT2ECLT(n)	((ECLanType)((n) - ERT_BEGIN))

#define JDG_VALID( ENUMTYPE,enumtype )	\
	((enumtype) >= ENUMTYPE##_BEGIN && (enumtype) < ENUMTYPE##_END)



#pragma pack(pop)