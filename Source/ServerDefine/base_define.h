#pragma once

//----------------------------------------------------------------
// Ϊ���ڴ��
//----------------------------------------------------------------
class ServerMemPool
{
public:
	LPVOID	Alloc(DWORD dwSize);
	VOID	Free(LPVOID pMem);
};

extern ServerMemPool g_MemPoolSafe;

#pragma pack(push, 1)

//----------------------------------------------------------------
// ����
//----------------------------------------------------------------
const INT X_ROLE_BUFF_SIZE			= (1024 * 1024);		// 1M.��ȡ�����ɫ����ʱ����Ϣbuff��С

const INT64 MIN_ITEM_SERIAL_INTERNAL	= 500000000000;
const INT64 MAX_ITEM_SERIAL_OUTTER		= 400000000000;
const INT64 MIN_ITEM_SERIAL_OUTTER		= 300000000001;
const INT64	MAX_ITEM_SERIAL_BILL		= 300000000000;

//----------------------------------------------------------------
// �ַ����޹صļ����ַ������ַ�������
//----------------------------------------------------------------
#define MCalTCharArrayNum(szArray)		(sizeof(TCHAR) == 1 ? sizeof(szArray) : (sizeof(szArray) >> 1))

//----------------------------------------------------------------
// �ж�װ���Ƿ��Ѽ���
//----------------------------------------------------------------
#define MIsIdentified(pEquip)			(pEquip->equipSpec.byQuality != (BYTE)EIQ_Null)

//----------------------------------------------------------------
// �ж�װ���Ƿ�Ϊring
//----------------------------------------------------------------
#define MIsRing(pEquip)				(EEP_Finger1 == pEquip->pEquipProto->eEquipPos)
#define MIsWeapon(pEquip)			(EIT_Weapon == pEquip->pProtoType->eType)
#define MIsFashion(pEquip)			(EIT_Fashion == pEquip->pProtoType->eType)

#define MIsWeaponByDisplayPos(nPos)	(EAE_RWeapon == nPos || EAE_LWeapon == nPos)
#define MIsWeaponByEquipPos(nPos)	(EEP_RightHand == nPos || EEP_LeftHand == nPos)

//----------------------------------------------------------------
// �ж�װ���Ƿ���Ч��
//----------------------------------------------------------------
//#define MIsEquipIneffective(pEquip)	\
//		(((tagEquipProto *)pEquip->pProtoType)->n16Newness > 0 \
//		&& pEquip->nUseTimes / 100 > ((tagEquipProto *)pEquip->pProtoType)->n16Newness)
//
//#define MIsEquipMainWeapon(pEquip)	\
//		(((tagEquipProto*)pEquip->pProtoType)->eEquipPos == EEP_RightHand)
//
//#define MIsEquipAsstWeapon(pEquip)	\
//		(((tagEquipProto*)pEquip->pProtoType)->eEquipPos == EEP_LeftHand)
//
//#define MIsEquipNeedCalEffect(pEquip)	\
//		(!MIsEquipAsstWeapon(pEquip) && !MIsEquipIneffective(pEquip))


//----------------------------------------------------------------
// �ж��Ƿ�Ϊ�ٷ���
//----------------------------------------------------------------
#define MIsValuePct(nNum)			((nNum) > 100000 || (nNum) < -100000)

//----------------------------------------------------------------
// ���ٷ���ת��Ϊ��ֵ
//----------------------------------------------------------------
#define MValuePctTrans(nNum)		((nNum) > 100000 ? (nNum) - 100000 : (nNum) + 100000)

#define MMSGINIT(ptr, TYPE)				\
	{											\
	TYPE tmp##TYPE;								\
	memcpy(ptr, &tmp##TYPE, sizeof(tagNetCmd));	\
	}

//----------------------------------------------------------------
// ��ȡ��Ϣ
//----------------------------------------------------------------
#define MGET_MSG(PRECV, PMSG, MSG_KIND)	\
	tag##MSG_KIND* PRECV = (tag##MSG_KIND*)PMSG;

//----------------------------------------------------------------
// ָ������ת��
//----------------------------------------------------------------
#define MTRANS_POINTER(PNEWPOINTER, PDATA, DST_POINTER_TYPE)	\
	DST_POINTER_TYPE* PNEWPOINTER = (DST_POINTER_TYPE*)PDATA;

//----------------------------------------------------------------
// ����Ч�򷵻�
//----------------------------------------------------------------
#define MTRANS_ELSE_RET(PNEWPOINTER, PDATA, DST_POINTER_TYPE, RETVALUE)		\
	MTRANS_POINTER(PNEWPOINTER, PDATA, DST_POINTER_TYPE);		\
	if(!P_VALID(PNEWPOINTER))	return RETVALUE;

//----------------------------------------------------------------
// ��������Ϣ�ڴ����봦�� -- ���ⲿ����������Ϣ��С
//----------------------------------------------------------------
//#define MCREATE_MSG(PSEND, SIZE, MSG_KIND)	\
//	BYTE* temp_p_##MSG_KIND = new BYTE[SIZE];					\
//	ZeroMemory(temp_p_##MSG_KIND, SIZE);						\
//	tag##MSG_KIND* PSEND = new(temp_p_##MSG_KIND) tag##MSG_KIND;\
//	PSEND->dwSize = SIZE;
//
//
//#define MDEL_MSG(PSEND)		delete [](BYTE*)PSEND;

//----------------------------------------------------------------
// ��������Ϣ�ڴ����봦�� -- ���ⲿ����������Ϣ��С
// �ŵ㣺���ù����ڴ棻ȱ�㣺����ʱ��������Ӧ��Ϣ��ģ�庯��
//----------------------------------------------------------------
class MsgSend
{
public:
	MsgSend(INT nSize)
	{
		m_nSize = nSize;

		// ������СΪ4��������
		m_nSize = (((m_nSize - 1) >> 2) + 1) << 2;

		m_pBuff = (BYTE*)g_MemPoolSafe.Alloc(m_nSize);
	}

	~MsgSend()
	{
		if(m_pBuff != NULL)
		{
			g_MemPoolSafe.Free(m_pBuff);
		}
	}

	template<typename T>
	VOID GetData(T* &pT)
	{
		ASSERT(m_nSize >= sizeof(T));
		ZeroMemory(m_pBuff, m_nSize);
		MMSGINIT(m_pBuff, T);
		pT = (T*)m_pBuff;
	}

private:
	BYTE	*m_pBuff;
	INT		m_nSize;
};

// �������õĺ����¶�����
#define MCREATE_MSG(PSEND, SIZE, MSG_KIND)		\
	MsgSend tmp##PSEND(SIZE);	\
	tag##MSG_KIND* PSEND;		\
	tmp##PSEND.GetData(PSEND);	\
	PSEND->dwSize = SIZE

#define MDEL_MSG(PSEND)		(0)

//----------------------------------------------------------------
// ��������ͻ����ַ������� -- ȷ�����ַ���������
//----------------------------------------------------------------
#define MGET_MSG_STRING(TSTR, PMSGSTR, MSGSZ, MSG_KIND, STRFIELD)						\
	INT32 tmpStrSz = MSGSZ - FIELD_OFFSET(tag##MSG_KIND, STRFIELD) / sizeof(TCHAR);		\
	if(tmpStrSz < 0)	tmpStrSz = 0;													\
	tstring TSTR(PMSGSTR, tmpStrSz)

//----------------------------------------------------------------
// �����������̨���;�����Ϣ
//----------------------------------------------------------------
#define MAttResCaution(FileName, KeyName, KeyVal)	\
	IMSG(_T("\n\nCaution:\n"));						\
	IMSG(_T("\tThere is a critical error in proto <%s,%s:%u>!!!!!\n\n"), (FileName), (KeyName), (KeyVal));

#define ASSERT_P_VALID( ptr ) ASSERT(P_VALID(ptr))

#pragma pack(pop)

