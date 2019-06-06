#pragma once

struct tagGuildAffairInfo
{
	DWORD		dwID;			// ����ID
	INT32		nGuildLv;		// ���ɵȼ�
	INT32		nCity;			// ����ID
	INT32		nFund;			// ��Ҫ�ʽ�
	INT32		nMaterial;		// ��Ҫ�ʲ�
	tstring		strName;		// ��������
	tstring		strDesc;		// ��������
};

struct tagGuildSkillInfo
{
	DWORD dwSkillID;			// ����ID
	tstring strName;			// ������
	INT32 n16Fulfill;			// ������ܵĽ���
	INT32 nResearchFund;		// �о������ʽ�
	INT32 nResearchMaterial;	// �о������ʲ�
	INT32 nLearnSilver;			// ѧϰ��������
	INT32 nLearnContribution;	// ѧϰ������ɹ���
	INT32 nLearnFund;			// ѧϰ��������ʽ�
	INT32 nLearnMaterial;		// ѧϰ��������ʲ�
};

struct tagCommerceInfo
{
	INT32	nLevelRange;	// �ȼ���Χ(��ߵȼ�*1000+��͵ȼ�)
	INT32	nDeposit;		// ��֤��
	INT32	nBeginningTael;	// ��ʼ����
	INT32	nPurposeTael;	// ָ������
	INT32	nMaxTael;		// ��������
};

struct tagGoodInfo
{
	DWORD dwID;				// �̻�ID
	tstring strName;		// �̻�����
	tstring strIconPath;	// ͼ����
	tstring strDisplayType;	// �̻���ʾ����
	tstring strDesc;		// �̻�����
};

struct tagCOCInfo
{
	DWORD dwID;				// �̻�ID
	tstring strName;		// �̻�����
	tstring strDesc;		// �̻�����
};