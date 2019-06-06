#include "stdafx.h"
#include "pet_exchange.h"
#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/msg_pet_exchange.h"

//-----------------------------------------------------------------------------
// ����&��������
//-----------------------------------------------------------------------------
PetExchangeMgr::PetExchangeMgr()
{
	m_pExData		= NULL;
	m_dwTgtRoleID	= GT_INVALID;
}

PetExchangeMgr::~PetExchangeMgr()
{
	SAFE_DEL(m_pExData);
}

//-----------------------------------------------------------------------------
// �������׿ռ� -- ����Ѵ��ڣ������³�ʼ��
//-----------------------------------------------------------------------------
VOID PetExchangeMgr::CreateData()
{
	if(NULL == m_pExData)
	{
		m_pExData = new tagPetExchangeData;
	}

	ASSERT(m_pExData != NULL);

	ZeroMemory(m_pExData, sizeof(tagPetExchangeData));
}

//-----------------------------------------------------------------------------
// �������ݽṹ����ӽ�����Ʒ -- ���� -- �ɹ�:����λ���±ꣻʧ��:GT_INVALID
//-----------------------------------------------------------------------------
INT32 PetExchangeMgr::AddPet(DWORD dwPetID)
{
	ASSERT(m_pExData != NULL);

	INT32 nInsIndex = GT_INVALID;
	for(INT32 i=0; i<MAX_EXCHANGE_ITEM; ++i)
	{
		if(m_pExData->dwPetIDs[i] == dwPetID)
		{
			IMSG(_T("Add the same pet<id: %lld> to exchange!!!!\r\n"), dwPetID);
			return GT_INVALID;
		}
		else if(0 == m_pExData->dwPetIDs[i] && GT_INVALID == nInsIndex)
		{
			nInsIndex = i;
		}
	}

	if(nInsIndex != GT_INVALID)
	{
		m_pExData->dwPetIDs[nInsIndex] = dwPetID;

		++m_pExData->byPetNum;
	}

	return E_Pets_Success;
}

//-----------------------------------------------------------------------------
// �ӽ�����Ʒ��ȡ��ָ����Ʒ -- �ɹ�����E_Success��ʧ�ܷ�����Ӧ������
//-----------------------------------------------------------------------------
DWORD PetExchangeMgr::DecPet(DWORD dwPetID)
{
	ASSERT(m_pExData != NULL);

	for(INT32 i=0; i<MAX_EXCHANGE_ITEM; ++i)
	{
		if(m_pExData->dwPetIDs[i] == dwPetID)
		{
			m_pExData->dwPetIDs[i] = 0;
			--m_pExData->byPetNum;
			return E_Success;
		}
	}

	return GT_INVALID;
}