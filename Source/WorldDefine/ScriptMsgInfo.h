//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: ScriptMsgInfo.h
// author: wjiang
// actor:
// data: 2009-04-27
// last:
// brief: �ű�ͨ����Ϣ��
//-----------------------------------------------------------------------------
#include "msg_script.h"

#pragma once

#pragma pack(push, 1)

//----------------------------------------------------------------
// ��Ϣ�������ݿ������
//----------------------------------------------------------------
enum EMsgUnitType
{
	EMUT_NULL				=	0,
	EMUT_MsgID				=	1,
	EMUT_RoleID				=	2,
	EMUT_RoleName			=	3,	
	EMUT_ItemTypeID			=	4,
	EMUT_NPCTypeID			=	5,
	EMUT_MapCrc				=	6,
	EMUT_TriggerID			=	7,
	EMUT_Pos				=	8,
	EMUT_Integer			=	9,
	EMUT_TotalLoop			=	10,		// ��������ܻ��� 10
	EMUT_QuestID			=	11,		// ����ɣ�
	EMUT_Num				=	12,		// ����
	EMUT_ChatInfo			=	13,		// ����Ƶ����ʾ
	EMUT_Score				=	14,		// ����
	EMUT_Rank				=	15,		// ����
	EMUT_Time				=	16,		// ʱ��
	EMUT_RebornTime			=	17,		// �������
	EMUT_TeamID				=	18,		// С��ID
	EMUT_GuildID			=   19,		// ����ID
	/************************************************************************/
	/*							���������ƽ���                              */
	/************************************************************************/
	EMUT_DlgWithSelOpt		=	20,		// ��ѡ�����
	EMUT_DlgOption			=	21,		
	EMUT_DlgTarget_Quest	=	22,
	EMUT_DlgTarget_Item		=	23,
	EMUT_DlgTarget_Creature	=	24,	
	EMUT_DlgTarget_Activity	=	25,

	EMUT_ErrorCode			=	26,		// ������

	EMUT_DlgQuiz			=	40,		// �������

	EMUT_DlgArrange			=   50,		// ��������
	EMUT_PVPActivity		=	60,		// pvp����桡���������Ͷ�Ӧ��ֵΪ-1ʱ��֪ͨ�ͻ��˹رջ���棩

	EMUT_Request_Activity	=	70,		// �̶����صĿͻ�������

	EMUT_Boardcast			=	100,	// �㲥
	EMUT_END,
};

//----------------------------------------------------------------
// EMUT_DlgOption��Ӧ��ѡ������
//----------------------------------------------------------------
enum EDlgOption
{
	EDO_A					=	0,
	EDO_B					=	1,
	EDO_C					=	2,
	EDO_D					=	3,
	EDO_Accept				=	4,
	EDO_Cancel				=	5,
	EDO_END,
};

//----------------------------------------------------------------
// ��������Ϣ�ڴ����봦�� -- ���ⲿ����������Ϣ��С
//----------------------------------------------------------------
#define MCREATE_SMSG(PSEND, SIZE, MSG_KIND, ALLOCATOR)	\
	BYTE* temp_p_##MSG_KIND = (BYTE*)ALLOCATOR.Alloc(SIZE);	\
	ZeroMemory(temp_p_##MSG_KIND, SIZE);				\
	tag##MSG_KIND* PSEND = (tag##MSG_KIND* )temp_p_##MSG_KIND;	\
	tag##MSG_KIND tmp##tag##MSG_KIND;							\
	memcpy(PSEND, &tmp##tag##MSG_KIND, sizeof(tagNetCmd));				\
	PSEND->dwSize = SIZE;
// #define MCREATE_SMSG(PSEND, SIZE, MSG_KIND)	\
// 	BYTE* temp_p_##MSG_KIND = new BYTE[SIZE];					\
// 	ZeroMemory(temp_p_##MSG_KIND, SIZE);						\
// 	tag##MSG_KIND* PSEND = new(temp_p_##MSG_KIND) tag##MSG_KIND;\
// 	PSEND->dwSize = SIZE;

#define MDEL_SMSG(PSEND, ALLOCATOR)		ALLOCATOR.Free((BYTE*)PSEND)

class MsgUnit
{
public:
	MsgUnit():m_eMsgUnitType(EMUT_NULL)  {}
	virtual ~MsgUnit() {}

	virtual VOID	SetData(EMsgUnitType eMsgUnitType, LPVOID pData) = 0;
	virtual LPVOID	GetData() = 0;
	virtual INT		GetDataSize() = 0;
	EMsgUnitType	GetMsgUnitType()	{ return m_eMsgUnitType; }

protected:
	EMsgUnitType	m_eMsgUnitType;
};

class UnitINT:public MsgUnit
{
public:
	UnitINT():MsgUnit(),m_nData(0) {}
	virtual ~UnitINT() {}

	inline VOID		SetData(EMsgUnitType eMsgUnitType, LPVOID pData)
	{
		m_eMsgUnitType = eMsgUnitType;
		m_nData = *(INT*)pData;
	}	

	inline LPVOID	GetData()	{ return &m_nData; }
	inline INT		GetDataSize() { return sizeof(INT); }

private:			
	INT				m_nData;
};

class UnitDWORD:public MsgUnit
{
public:
	UnitDWORD():MsgUnit(),m_dwData(GT_INVALID) {}
	~UnitDWORD() {}

	inline	VOID	SetData(EMsgUnitType eMsgUnitType, LPVOID pData)
	{
		m_eMsgUnitType = eMsgUnitType;
		m_dwData = *(DWORD*)pData;
	}

	inline	LPVOID	GetData()	{ return &m_dwData; }
	inline	INT		GetDataSize()	{ return sizeof(DWORD); }

private:
	DWORD			m_dwData;
};

class UnitTCHAR:public MsgUnit
{
public:
	UnitTCHAR():MsgUnit(),m_pData(NULL)	{}
	~UnitTCHAR() { SAFE_DEL(m_pData); }

	inline	VOID	SetData(EMsgUnitType eMsgUnitType, LPVOID pData)
	{
		INT nSize = _tcsclen((TCHAR*)pData);
		m_pData = new TCHAR[nSize + 1];
		m_eMsgUnitType = eMsgUnitType;
		_tcsncpy(m_pData, (TCHAR*)pData, (nSize + 1) * sizeof(TCHAR));
		m_pData[nSize] = _T('\0');
	}

	inline	LPVOID	GetData()	{ return m_pData; }
	inline	INT		GetDataSize()	{ return (_tcsclen((TCHAR*)m_pData) + 1)*sizeof(TCHAR); }

private:
	TCHAR*			m_pData;
};

class UnitFLOAT:public MsgUnit
{
public:
	UnitFLOAT():MsgUnit(),m_fData(0.0) {}
	~UnitFLOAT() {}

	inline	VOID	SetData(EMsgUnitType eMsgUnitType, LPVOID pData)
	{
		m_eMsgUnitType = eMsgUnitType;
		m_fData = *(FLOAT*)pData;
	}

	inline	LPVOID	GetData()	{ return &m_fData; }
	inline	INT		GetDataSize()	{ return sizeof(m_fData); }

private:
	FLOAT			m_fData;
};


class MsgInfo
{
public:
	MsgInfo(DWORD dwMsgID = 0):m_dwMsgID(dwMsgID),m_dwBlobSize(0) {}
	~MsgInfo()
	{
		//���m_vecMsgUnit
		std::vector<MsgUnit*>::iterator it = m_vecMsgUnit.begin();
		while (it != m_vecMsgUnit.end())
		{
			SAFE_DEL(*it);
			++it;
		}

		m_vecMsgUnit.clear();
	}

	VOID ReSetMsgInfo()
	{
		//���m_vecMsgUnit
		std::vector<MsgUnit*>::iterator it = m_vecMsgUnit.begin();
		while (it != m_vecMsgUnit.end())
		{
			SAFE_DEL(*it);
			++it;
		}

		m_vecMsgUnit.clear();
		m_dwBlobSize = 0;
	}

	template<typename Allocator>
	LPVOID CreateMsgByMsgInfo(Allocator &allocator)
	{
		m_dwMsgSize = m_dwBlobSize + sizeof(tagNS_MsgBlob) - 1;
		MCREATE_SMSG(pSend, m_dwMsgSize, NS_MsgBlob, allocator);
		BYTE*	pTempt = pSend->byData;

		std::vector<MsgUnit*>::iterator it = m_vecMsgUnit.begin();
		while (it != m_vecMsgUnit.end())
		{
			EMsgUnitType	eMsgUnitType = (*it)->GetMsgUnitType();
			memcpy(pTempt, &eMsgUnitType, sizeof(EMsgUnitType));
			pTempt += sizeof(EMsgUnitType);

			memcpy(pTempt, (*it)->GetData(), (*it)->GetDataSize());
			pTempt += (*it)->GetDataSize();
			++it;
		}

		return pSend;
	}

	template<typename Allocator>
	VOID DeleteMsg(PVOID pToDel, Allocator &allocator)
	{
		MDEL_SMSG(pToDel, allocator);
	}

	static VOID ChangeBlob2MsgInfo(LPVOID pData, DWORD dwSize, std::vector<MsgUnit*> &veMsgUnit)
	{
		MsgUnit *pMsgUnit = NULL;
		BYTE	*pTemp = (BYTE*)pData;

		while(dwSize > 0)
		{
			EMsgUnitType eMsgUnitType = *(EMsgUnitType*)pTemp;
			pTemp = pTemp + 4;
			dwSize -= 4;

			switch (eMsgUnitType)
			{
			case EMUT_MsgID:
			case EMUT_RoleID:
			case EMUT_ItemTypeID:
			case EMUT_NPCTypeID:
			case EMUT_MapCrc:
			case EMUT_TriggerID:
			case EMUT_DlgTarget_Quest:
			case EMUT_DlgTarget_Item:
			case EMUT_ChatInfo:
			case EMUT_DlgQuiz:
			case EMUT_DlgTarget_Creature:
			case EMUT_Time:
			case EMUT_TeamID:
			case EMUT_GuildID:
			case EMUT_DlgTarget_Activity:
			case EMUT_ErrorCode:
				{	
					pMsgUnit = new UnitDWORD();
					if(!P_VALID(pMsgUnit))
						return;

					pMsgUnit->SetData(eMsgUnitType, pTemp);
					pTemp = (BYTE*)pTemp + 4;
					dwSize -= 4;
				}
				break;
			case EMUT_RoleName:
				{
					pMsgUnit = new UnitTCHAR();
					if(!P_VALID(pMsgUnit))
						return;

					pMsgUnit->SetData(eMsgUnitType,(LPVOID)pTemp);
					INT nDataSize = pMsgUnit->GetDataSize();
					pTemp = (BYTE*)pTemp + nDataSize;
					dwSize -= nDataSize;
				}
				break;
			case EMUT_Pos:
			case EMUT_Num:
			case EMUT_DlgWithSelOpt:
			case EMUT_DlgOption:
			case EMUT_QuestID:
			case EMUT_TotalLoop:
			case EMUT_Score:
			case EMUT_Rank:
			case EMUT_RebornTime:
			case EMUT_DlgArrange:
			case EMUT_PVPActivity:
			case EMUT_Request_Activity:
				{
					pMsgUnit = new UnitINT();
					if(!P_VALID(pMsgUnit))
						return;
					
					pMsgUnit->SetData(eMsgUnitType,(LPVOID)pTemp);
					INT nDataSize = pMsgUnit->GetDataSize();
					pTemp = (BYTE*)pTemp + nDataSize;
					dwSize -= nDataSize;
				}
				break;
			default:
				return;
			}

			veMsgUnit.push_back(pMsgUnit);
		}
	}
	// �ͷŴ�ChangeBlob2MsgInfo�õ����ڴ�
	static VOID ReleaseMsgInfo(std::vector<MsgUnit*> &veMsgUnit)
	{
		std::vector<MsgUnit*>::iterator iter = veMsgUnit.begin();
		std::vector<MsgUnit*>::iterator end = veMsgUnit.end();

		for (; iter != end; ++iter)
		{
			SAFE_DELETE((*iter));
		}

		veMsgUnit.clear();
	}

	VOID AddMsgUnit(EMsgUnitType eMsgUnitType, LPVOID pData)
	{
		MsgUnit *pMsgUnit = CreateMsgUnit(eMsgUnitType, pData);
		if(!P_VALID(pMsgUnit))
			return;

		m_vecMsgUnit.push_back(pMsgUnit);
		m_dwBlobSize += sizeof(EMsgUnitType);
		m_dwBlobSize += pMsgUnit->GetDataSize();
	}

	DWORD GetMsgID()	{ return m_dwMsgID; }
	DWORD GetMsgSize() { return m_dwMsgSize; }
	DWORD GetBlobSize() { return m_dwBlobSize; }

private:
	MsgUnit* CreateMsgUnit(EMsgUnitType eMsgUnitType, LPVOID pData)
	{
		MsgUnit *pMsgUnit = NULL;

		switch (eMsgUnitType)
		{
		case EMUT_MsgID:
		case EMUT_RoleID:
		case EMUT_ItemTypeID:
		case EMUT_NPCTypeID:
		case EMUT_MapCrc:
		case EMUT_TriggerID:
		case EMUT_DlgTarget_Quest:
		case EMUT_DlgTarget_Item:
		case EMUT_ChatInfo:
		case EMUT_DlgQuiz:
		case EMUT_Boardcast:
		case EMUT_DlgTarget_Creature:
		case EMUT_Time:
		case EMUT_TeamID:
		case EMUT_GuildID:
		case EMUT_DlgTarget_Activity:
		case EMUT_ErrorCode:
			{	
				pMsgUnit = new UnitDWORD();
				if(!P_VALID(pMsgUnit))
					return NULL;

			}
			break;
		case EMUT_RoleName:
			{
				pMsgUnit = new UnitTCHAR();
				if(!P_VALID(pMsgUnit))
					return NULL;
			}
			break;
		case EMUT_Pos:
		case EMUT_Num:
		case EMUT_Integer:
		case EMUT_DlgWithSelOpt:
		case EMUT_DlgOption:
		case EMUT_QuestID:
		case EMUT_TotalLoop:
		case EMUT_Score:
		case EMUT_Rank:
		case EMUT_RebornTime:
		case EMUT_DlgArrange:
		case EMUT_PVPActivity:
		case EMUT_Request_Activity:
			{
				pMsgUnit = new UnitINT();
				if(!P_VALID(pMsgUnit))
					return NULL;
			}
			break;
		default:
			return NULL;
		}

		pMsgUnit->SetData(eMsgUnitType, pData);

		return pMsgUnit;
	}

protected:
	DWORD					m_dwMsgID;
	DWORD					m_dwBlobSize;
	DWORD					m_dwMsgSize;
	std::vector<MsgUnit*>	m_vecMsgUnit;	
};

#pragma pack(pop)