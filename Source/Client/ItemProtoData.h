#pragma once

#include ".\BaseData.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\suit_define.h"
#include "..\WorldDefine\pet_equip_define.h"
#include "..\WorldDefine\longhun_define.h"

//! \struct tagTreasureProto
//! \brief �����䱦�б�,��itemid�������п�������ƷҲ�п�����װ��
//! \author hyu
struct tagTreasureProto
{
    DWORD		dwTreasureID;	//!< �����䱦id,����Ʒid�޹�
    DWORD		dwItemID;		//!< ��Ʒid,��itemid����
    INT			nConNeed;		//!< �����������幱��ֵ
    ECLanType	eCtype;			//!< ������������
    DWORD		dwShopID;		//!< �̵�ID

    tagTreasureProto() {ZeroMemory(this, sizeof(*this));}
};
//! \struct tagEquipQltyEffect
//! \brief װ��Ʒ��Ӱ���
//! \author hqzhang
struct tagEquipQltyEffect
{
	// ��������Ӱ��ϵ��
	FLOAT fWeaponFactor;
	FLOAT fArmorFactor;

	// һ������
	INT32 nAttAFactor;
	FLOAT fAttAFactor;
	INT32 nAttANumEffect;

	// Ǳ��ֵ
	FLOAT fPotFactor;

	// ��Ƕ������ -- ��¼���ֵļ���
	INT32 nHoleNumPct[MAX_EQUIPHOLE_NUM + 1];

	// �������� -- ����
	INT32 nLonghunPct[X_LONGHUN_TYPE_NUM];

	// ��������
	INT32 nSpecAttPct;
};

/** \class	ItemProtoData
	\brief	��Ʒ��װ��ԭʼ���Խṹ�洢
*/
class ItemProtoData
	: public BaseData
{
public:
	ItemProtoData(void);
	~ItemProtoData(void);

	static ItemProtoData* Inst();

	/** ���������ļ�
	*/
	virtual void LoadFromFile();
	/** ��ȡ��Ʒ�����ṹ
	*/
	const tagItemProto* FindItemProto(DWORD dwID)
	{
		if(MIsEquipment(dwID))
			return FindInMap(m_mapEquipProto, dwID);
	
		return FindInMap(m_mapItemProto, dwID);
	}
	/** ��ȡװ�������ṹ
	*/
	const tagEquipProto* FindEquipProto(DWORD dwID)
	{
		return FindInMap(m_mapEquipProto, dwID);
	}
	/** ��ȡ��װ�����ṹ
	*/
	const tagSuitProtoClient* FindSuitProto(DWORD dwID)
	{
		return FindInMap(m_mapSuitProto, dwID);
	}
	/** ��ȡ��Ʒ����ʾ�ṹ
	*/
	const tagItemDisplayInfo* FindItemDisplay(DWORD dwID)
	{
		const tagItemProto* pProto = FindItemProto(dwID);
		if(P_VALID(pProto))
		{
			return pProto->pDisplayInfo;
		}
		return NULL;
	}
	//! ����䱦
	const tagTreasureProto* FindTreasureByTrID(DWORD dwID)
	{
		return FindInMap(m_mapTreasureProto, dwID);
	}

    //! ͨ��itemid���������ͻ�õ�һ���䱦ԭ��
	const tagTreasureProto* FindTreasureByItemID(DWORD dwID, ECLanType eType)
	{
        multimap<DWORD, tagTreasureProto>::_Paircc range;
        multimap<DWORD, tagTreasureProto>::const_iterator it;

        range = m_mapTreasureProtoByItemID.equal_range(dwID);
        for (it = range.first; it != range.second; ++it)
        {
            if (it->second.eCtype == eType)
            {
                return &it->second;
            }
        }

        return NULL;
	}

	const map<DWORD, tagTreasureProto>* GetTreasureProto()
	{
		return &m_mapTreasureProto;
	}

    //! ͨ����Ʒԭ��id��ó���װ��ԭ��
    const tagPetEquipProto* FindPetEquipByItemID(DWORD dwID);
    //! ͨ������װ��ԭ��id��ó���װ��
    const tagPetEquipProto* FindPetEquipByEquipID(DWORD dwID);

	const tagEquipQltyEffect* FindEquipQltyEffect(INT nQlty)
	{
		return FindInMap(m_mapEquipQlty, nQlty);
	}

private:
	map<DWORD, tagItemProto>		    m_mapItemProto;		// ��Ʒԭʼ�ṹ
	map<DWORD, tagEquipProto>		    m_mapEquipProto;	// װ��ԭʼ�ṹ
	map<DWORD, tagSuitProtoClient>	    m_mapSuitProto;		// ��װԭʼ�ṹ
	map<DWORD, tagTreasureProto>	    m_mapTreasureProto;	// �����䱦ԭʼ����
	multimap<DWORD, tagTreasureProto>	m_mapTreasureProtoByItemID;	//ͨ��itemid���������䱦
    map<DWORD, tagPetEquipProto>        m_mapPetEquipProtoByID;     //ͨ������װ��id����װ��
	map<INT, tagEquipQltyEffect>		m_mapEquipQlty;		// װ��Ʒ��Ӱ���
};