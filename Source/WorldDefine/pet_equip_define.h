#pragma once
#pragma pack(push,1)

// װ���޸��������������Ŀ
const INT MAX_PET_EQUIP_MOD_ATT		= 3;

// װ����λ����
const INT SIZE_PET_EQUIP_BAR		= 4;


struct tagPetEquipProto
{
	DWORD	dwTypeID;		//���ܱ��	װ��ԭ�͵�ΨһID��
	INT		nStep;			//��	���޿ɴ���װ����Ҫ����ͽ�����
	INT		nGrade;			//��	���޿ɴ���װ����Ҫ����͵�����
	INT		nType;			//����	�߻������װ�����ͣ�����ֻ���ȡ��ż��ɣ�
	BOOL	bTypeUnique;	//�Ƿ�ͬ��Ψһ	�˴���1ʱ��������ͬ��װ������ͬʱ������������װ��ʱ��Ŀ�������Ѵ���ͬ��װ�������滻ԭ��װ����
	BOOL	bUnique;		//�Ƿ�Ψһ	�˴���1ʱ����ͬID��װ������ͬʱ������������װ��ʱ��Ŀ�������Ѵ�����ͬװ�������滻ԭ��װ����
	
	INT		nPetAtt[MAX_PET_EQUIP_MOD_ATT];		//��������#	װ���ӳɵ������������ͣ�
	INT		nPetAttMod[MAX_PET_EQUIP_MOD_ATT];	//����ֵ#	װ���ӳɵ���������ֵ��
};

//-----------------------------------------------------------------------------
// ������Ʒ���Ϳͻ��˵���Ϣ�ṹ
//-----------------------------------------------------------------------------
struct tagPetEquipMsgInfo
{
    INT64       n64ItemID;      //!< 64λitemid
    DWORD       dwItemProtoID;  //!< ԭ��id
	INT8		n8Pos;			//!< װ������λ��
};

#pragma pack(pop)