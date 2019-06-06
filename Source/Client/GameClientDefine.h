#pragma once

//�������ťʹ�õ���GUI�¼�
enum EGUI_ShortcutBar_Event 
{
	EGUISCB_Drop = EGUIE_User + 100,		// ��ť��ק���ͷ�
	EGUISCB_DropCheck,						// �ͷż�飬���ڿ�ݰ�ť���Ϸţ�
	EGUISCB_DropSwap,						// ��ݼ��Ľ����¼�
	EGUISCB_RightMouseDown,					// ����Ҽ���� 
	EGUISCB_LeftMouseDown,					// ���������
	EGUISCB_UseSpecFunc,					// ����ʹ��
};

//�϶�����
enum EDragType
{
	EDT_Null = 0,
	EDT_Skill,						//����
	EDT_Item,						//��Ʒ(�����е���Ʒ)

	EDT_ItemSplit,					//��Ʒ���

	EDT_ItemEquip,					//װ�����е���Ʒ
	EDT_ItemWare,					//�ֿ��е���Ʒ
	EDT_ItemShop,					//�̵��е���Ʒ

    EDT_PetEquip,                   //!< ����װ��
    EDT_GuildWare,                  //!< ���ɲֿ�

	EDT_End,
};

//��ݰ�ť����
enum EShortcutBtnType
{
	ESBT_Null = 0,
	ESBT_Skill,						//����
	ESBT_Item,						//��Ʒ
    ESBT_PetSkill,                  //!< ���＼��

	ESBT_End,
};


//��ݰ�ť����
class BtnData
{
private:
	EShortcutBtnType 	m_eType;		//	��ť����
	DWORD				m_dwParam1;		//	��Ӧ����1(skill:id,	item:��Ŀ+λ��, petskill:petid)
	DWORD				m_dwParam2;		//	��Ӧ����2(skill:NULL,	item:typeid, petskill:skillid(���ȼ�))
	INT64				m_n64Param3;		//	(skill:NULL, item: 64id, petskill:null)

public:

	BtnData() { m_eType = ESBT_Null; m_dwParam1 = m_dwParam2 = GT_INVALID; m_n64Param3 = GT_INVALID;}

	BtnData( EShortcutBtnType eType, DWORD dwParam1, DWORD dwParam2, INT64 n64Param3 ){
		Set( eType, dwParam1, dwParam2, n64Param3 );
	}
	virtual ~BtnData(){} 

	BtnData& operator=(const BtnData& data ){
		if(this==&data)
			return *this;

		m_eType			= data.m_eType;
		m_dwParam1		= data.m_dwParam1;
		m_dwParam2		= data.m_dwParam2;
		m_n64Param3		= data.m_n64Param3;
		return *this;
	}

	void Set( EShortcutBtnType eType, DWORD dwData1, DWORD dwData2, INT64 n64Data3 )
	{
		m_eType			= eType;
		m_dwParam1		= dwData1;
		m_dwParam2		= dwData2;
		m_n64Param3		= n64Data3;
	}

	const EShortcutBtnType	GetType() const		{ return m_eType;		}
	const DWORD	GetData1() const	{ return m_dwParam1;	}
	const DWORD	GetData2() const	{ return m_dwParam2;	}	
	const INT64	GetData3() const	{ return m_n64Param3;	}

};
