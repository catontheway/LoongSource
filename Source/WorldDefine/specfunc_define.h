#pragma once


#pragma pack(push, 1)

//------------------------------------------------------------------------------
// ��Ʒ���⹦�����Ͷ���
//------------------------------------------------------------------------------

/** \��Ʒ���⹦������(�ô���<val1, val2>��ʾ��2������,<val>��ʾ1��,û��ע����ʾû��)
*/
enum EItemSpecFunc	
{
	EISF_Null										= 0,	// ��

	EISF_AcceptQuest								= 1,	// ��ȡ����
	// nSpecFuncVal1: ������
	// nSpecFuncVal2: ����
	EISF_IdetifyEquip								= 2,	// ����װ��
	// nSpecFuncVal1: ��������
	// nSpecFuncVal2: ����
	EISF_HoleGem									= 3,    // ��Ƕ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_Brand										= 4,	// ��ӡ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_LongHunShi									= 5,    // ����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RemoveAttPt								= 6,	// ����ϴ��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RemoveTalentPt								= 7,	// ����ϴ��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_LearnSkill									= 8,	// ѧϰ����
	// nSpecFuncVal1: ����ID
	// nSpecFuncVal2: ����
	EISF_Grind										= 9,	// ĥʯ
	// nSpecFuncVal1: �ָ����¶�ֵ
	// nSpecFuncVal2: ����
	EISF_ComposeAdvance								= 10,	// �ϳɼ���������
	// nSpecFuncVal1: ����������	
	// nSpecFuncVal2: �����ٷֱ�
	EISF_OpenSafeGuard								= 11,	// ǿ��PK����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RevieveItem								= 12,	// �������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_FriendGift									= 13,	// ��������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_Chisel										= 15,	// ����
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_ProtectSign								= 16,	// ���׷�
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_Chest										= 17,	// ����
	// nSpecFuncVal1: ���俪������Ʒ(�����)
	// nSpecFuncVal2: Կ�׵�TypeID
	EISF_ChestKey									= 18,	// ����Կ��
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����

  //EISF_Chest*                                     = 19,   // ���䱣��
	// nSpecFuncVal1: ����
    // nSpecFuncVal2: ����
	
	EISF_PetEgg										= 20,	// ���ﵰ
	// nSpecFuncVal1: ԭ��ID
	// nSpecFuncVal2: ����
	EISF_PetFood									= 21,	// ����ʳ��
	// nSpecFuncVal1: Я���ȼ�
	// nSpecFuncVal2: ����ֵ
	EISF_PetSkillBook								= 22,	// ���＼����
	// nSpecFuncVal1: ����ԭ��ID
	// nSpecFuncVal2: ����
	EISF_PetLvlupStep								= 23,	// ��������
	// nSpecFuncVal1: ��׺�Ҫ��������ֵ
	// nSpecFuncVal2: ����
    EISF_PetEquipment                               = 24,	// ����װ��
    // nSpecFuncVal1: װ��ԭ��ID
    // nSpecFuncVal2: ����
    EISF_ColorProbability                           = 25,   // ָ����ɫ�׶μ�������
    // nSpecFuncVal1: ����1-4���ɵ͵��߷ֱ��ʾ�ĸ���ɫϡ�г̶Ƚ׶Σ��ͣ��У��ߣ�����
    // nSpecFuncVal2: 1-10000����ʾ��ָ���׶εļ��ʵ������̶ȣ�10000��ʾ����100%��
    EISF_Dye                                        = 26,   // Ⱦ��
    // nSpecFuncVal1: 1-12����Ӧ��ɫ��ͼ���
    // nSpecFuncVal2: ����
	EISF_GuildSkillBook								= 27,	// ���ɼ��ܵ伮
	// nSpecFuncVal1: �Ͻ�ʱ��������������
	// nSpecFuncVal2: ���ɹ���������
	EISF_PetExpPour									= 28,	// ���ﾭ���ע
	// nSpecFuncVal1: ��ע����ֵ
	// nSpecFuncVal2: ����
	EISF_PetEnhance									= 29,	// ������������
	// nSpecFuncVal1: ���޵�Ʒ��
	// nSpecFuncVal2: �������ֵ
	EISF_EquipLock									= 30,	// װ������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_EquipUnLock								= 31,	// װ������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_PetLock									= 32,	// ��������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_PetUnLock									= 33,	// �������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	
	EISF_PortableWare								= 34,   // ����ֿ�

	EISF_TransportTool                              = 35,   // �����ͷ�

	EISF_Fishing									= 36,	// ���㣬�һ�ϵͳ

	EISF_ChangeVocation								= 37,	// תְ

	EISF_EquipAddPotValue							= 38,	// ����װ��Ǳ������
	// nSpecFuncVal1: �ɹ���
	// nSpecFuncVal2: Ǳ��ֵ��������
	EISF_BagExtend									= 39,	// ������չ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_RoleWareExtend								= 40,	// �ֿ���չ
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_BeGoldStone								= 41,	// ���ʯ
	// nSpecFuncVal1: 1�� ���ʯ	 2�� �μ����ʯ
	// nSpecFuncVal2: ����
	EISF_RenameItem									= 42,	// ������
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
	EISF_GemRemovalSign									= 43,	// ��ʯ����� Jason v1.3.1
	// nSpecFuncVal1: ����
	// nSpecFuncVal2: ����
//	EISF_		= ,	// 
};

/** \�ϳɼ��������� EISF_ComposeAdvance �������������
*/
enum ESpecFuncComposeAdvance			
{
	ESFCA_AllProduce		= 0,	// ����������������
	ESFCA_Artisan,					// �ɽ�
	ESFCA_DanTraining,				// ����
	ESFCA_Smith,					// ����
	ESFCA_Casting,					// ����
	ESFCA_Dressmaker,				// �÷�
	ESFCA_Aechnics,					// ����
	ESFCA_Smilt,					// ұ��

	EISFC_AllConsolidate	= 10,	// ����װ��ǿ����������
	EISFC_Posy,						// ����
	EISFC_Engrave,					// �Կ�
	EISFC_Quench,					// ���
	

	EISFC_All				= 20,	// ���кϳɶ������� 
};


#pragma pack(pop)