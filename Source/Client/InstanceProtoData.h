#pragma once

#include "..\WorldDefine\MapAttDefine.h"

/*!
\brief �������Ա������ԣ��ͻ���ͨ����Щ���Խ���һЩԤ���ж� (�����)
*/

#define INSTANCE_CONDITION_NUM 3		//�����Ѷȼ�������

// �����������ö��
enum EInstanceComplteCondition
{
	EICC_Time		= 0,	// ���ʱ��
	EICC_People		= 1,	// �������
	EICC_Monster	= 2,	// ��չ���
	EICC_Boss		= 3,	// ��ɱָ������
	EICC_Item		= 4		// ���ָ����Ʒ
};

// ������������ṹ,�ͻ���������
struct tagInstanceCompleteCondition
{
	EInstanceComplteCondition	eCondition;		// �������
	DWORD						dwCompleteVal1; // ����ֵ1
	DWORD						dwCompleteVal2; // ����ֵ2
};

// �������Ա��ͻ������貿�֣�
struct tagInstanceProtoData
{
	DWORD				dwInstanceID;			// ����ID
	tstring				strName;				// ��������
	EInstanceMapType	eType;					// ������ͼ����
	BOOL				bAskEnter;				// �Ƿ�ѯ�ʽ���
	BOOL				bSelectHard;			// �Ƿ��ѡ�Ѷ�
	BOOL				bSelectNormal;			// �Ƿ��ѡ��ͨ
	BOOL				bSelectElite;			// �Ƿ��ѡ��Ӣ
	BOOL				bSelectDevil;			// �Ƿ��ѡħ��
	INT					nNumLowerLimit;			// ��������
	INT					nNumUpperLimit;			// ��������
	INT					nLevelLowerLimit;		// �ȼ�����
	INT					nLevelUpperLimit;		// �ȼ�����
	BOOL				bCombat;				// �ܷ�ս��
	BOOL				bPK;					// �ܷ�PK
	BOOL				bLoseSafeGuard;			// �Ƿ񱣻�ģʽʧЧ

	tagInstanceCompleteCondition Condition[INSTANCE_CONDITION_NUM];
};

class InstanceProtoData
{
public:
	friend class InstanceMgr;
	BOOL LoadFromFile(LPCTSTR szPath);
	const tagInstanceProtoData* FindInstanceProtoData(DWORD dwInstanceID);
private:
	typedef map<DWORD, tagInstanceProtoData> InstanceProtoDataMap;
	InstanceProtoDataMap m_mapInstanceProtoData;
};

