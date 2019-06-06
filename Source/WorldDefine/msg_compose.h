//-----------------------------------------------------------------------------
//!\file msg_compose.h
//!
//!\date 2008-09-25
//!
//!\brief �ͻ����������֮����ںϳɣ��ֽ⣬ǿ������Ϣ
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "compose_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum 
{
	//���д�����
	E_Compose_NPC_Not_Exist = 1, 				//NPC������
	E_Compose_Formula_Not_Exist = 2, 			//�䷽������
	E_Compose_Stuff_Not_Enough = 3, 			//������������
	E_Compose_Stuff_Formula_Not_Match = 4, 		//���Ϻ��䷽��ƥ��
	E_Compose_NPC_Distance = 5,					//NPC����̫Զ
	
	//�ϳ�ǿ�����ص����ֽ��,�Ǹ��ݺϳɹ�ʽ���
	E_Compose_Consolidate_Success = 11, 		//�ɹ�
	E_Compose_Consolidate_Lose = 12, 			//ʧ��
	E_Compose_Consolidate_Perfect = 13, 		//����

	
	//�ϳɴ�����
	E_Compose_Stuff_Not_Exist = 21, 			//�ϳɲ��ϲ�����
	E_Compose_Type_Not_Exist = 22,				//�ϳ����Ͳ�����
	E_Compose_Skill_Not_Exist = 23,				//���û�кϳɼ���
	E_Compose_FormulaNotMatch = 24,				//�ϳ��䷽��ƥ��
	E_Compose_NPCCanNotCompose = 25,			//���Ǻϳ�NPC
	E_Compose_Vitality_Inadequate = 26,			//��һ���ֵ����
	E_Compose_NotEnough_Money = 27,				//��ҽ�Ǯ����
	E_Compose_Bag_Full = 28,					//��������
	
	//�ֽ������
	E_Decomposition_Item_Not_Exist = 31, 		//�ֽ���Ʒ������

	//ǿ��������
	E_Consolidate_Equip_Not_Exist = 41, 		//ǿ��װ��������
	E_Consolidate_Type_Not_Exist = 42,			//ǿ�����Ͳ�����
	E_Consolidate_NPCCanNotPosy	= 43,			//��������NPC
	E_Consolidate_NotEquipment = 44,			//ǿ������Ʒ����װ��
	E_Consolidate_BeyondPosyTime,
	E_Consolidate_FormulaNotMatch,				//ǿ���䷽��ƥ��
	E_Consolidate_NotEnough_Money,				//��ҽ�Ǯ����
	E_Consolidate_NotEnough_Stuff,				//��Ҳ��ϲ���
	E_Consolidate_EquipCanNotPosy,				//�����Բ��ܱ�����
	E_Consolidate_ValConsume_Inadequate,		//װ��Ǳ��ֵ����

	E_Compose_NPCCanNotEngrave,					//�����Կ�NPC
	E_Consolidate_EquipCanNotEngrave,			//�����Բ��ܱ��Կ�
	E_Consolidate_BeyondEngraveTime,			//�����Կ̴���

	E_Consolidate_Gem_Not_Exit,					//��ʯ������
	E_Consolidate_Gem_Full,						//װ����������ʯ
	E_Consolidate_EquipCanNotInlay,				//�ñ�ʯ���ܱ���Ƕ
	E_Consolidate_Gem_Not_Hole,					//װ���޿�

	E_Consolidate_Brand_Not_Exit,				//ӡ�ǲ�����
	E_Consolidate_Brand_TopLevel,				//ӡ�ǵȼ��ﵽ����
	E_Consolidate_EquipCanNotBrand,				//��װ�����ܱ�ӡ��

	E_Consolidate_LoongSoul_Not_Exit,			//����ʯ������
	E_Consolidate_LoongSoul_TopLevel,			//����ȼ��ﵽ����
	E_Consolidate_EquipCanNotLoongSoul,			//��װ�����ܱ�����

	E_Consolidate_NPCCanNotQuench,				//���Ǵ��NPC
	E_Consolidate_WXValueInadequate,			//ԭ����ֵ����
	E_Consolidate_False,						//���ʧ��

	E_Compose_Quality_Not_Match,				//�㻯ֻ���ڻ�ɫ������װ��
	E_Compose_Not_Fashion,						//��߼�ʱװ���ɵ㻯��ֽ�
	E_Compose_Equip_Lock,						//��������װ�����ɵ㻯��ֽ�
	E_Compose_Equip_Time_Limit,					//��ʱ�����Ƶ�װ�����ɵ㻯��ֽ�
	E_Compose_Equip_Not_identify,				//δ������װ�����ɵ㻯��ֽ�
	E_Compose_Equip_Level_Inadequate,			//װ���ȼ�����
	E_Compose_Equip_Quality_Inadequate,			//װ��Ʒ�ʲ���
	E_Compose_Equip_Type_Inadequate,			//װ�����Ͳ������䷽Ҫ��

	E_Consolidate_Chisel_Not_Exit,				//��ʯ������
	E_Consolidate_Equip_CanNot_Chisel,			//װ�����ܱ�����
	E_Consolidate_Equip_Hole_Full,				//��Ƕ��������Ϊ5�������ɽ��п���
	E_Consolidate_Chisel_Not_Match,				//��ʯ�ȼ�С���������װ���ȼ������ɽ��п���
	E_Consolidate_NotIdentified,				//δ����װ�����ɿ���

	E_Raise_PotVal_Not_Needing,					// �Ѿ����ˣ�����Ҫ������Ǳ��ֵ
	E_Raise_PotVal_Times_Zero,					// ��������Ϊ��

	// Jason v1.3.1 2009-12-21 ��ʯ���errorid
	E_Gem_NPCCanNotRemoveGem,					//���Ǳ�ʯ���NPC

};

//-----------------------------------------------------------------------------
//	�ϳ���Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_Produce)
	DWORD 				dwNPCID; 		//NPCID
	DWORD				dwSkillID;		//����ID
	INT64				n64ItemID;		//��ƷID
	DWORD 				dwFormulaID;  	//�䷽ID
	INT64				n64IMID;		//IMid
	INT64				n64StuffID[1]; 	//���� �����䳤
CMD_END

CMD_START(NS_Produce)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	�ֽ���Ϣ ���ڵ㻯�����е�һ����֧
//-----------------------------------------------------------------------------
CMD_START(NC_Decomposition)
	DWORD				dwNPCID; 		//NPCID	
	DWORD				dwSkillID;		//����ID
	INT64				n64ItemID;		//��ƷID
	DWORD				dwFormulaID;	//�䷽ID
	INT64				n64IMID;		//IMid
	INT64				n64Item; 		//���ֽ���Ʒ64λID
CMD_END

CMD_START(NS_Decomposition)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	ǿ����Ϣ
//-----------------------------------------------------------------------------
CMD_START(NC_ConsolidatePosy)
	DWORD 				dwNPCID; 		//NPCID
	DWORD 				dwFormulaID;  	//�䷽ID
	INT64 				n64ItemID; 		//Ҫ��ǿ����Ʒ��64λID
	INT64				n64IMID;		//IMid
	INT64				n64StuffID[1]; 	//���� �����䳤	
CMD_END

CMD_START(NS_ConsolidatePosy)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_ConsolidateEngrave)
	DWORD				dwNPCID;		//NPCID
	DWORD				dwFormulaID;	//�䷽ID
	INT64				n64ItemID;		//Ҫ��ǿ����Ʒ��64λID
	INT64				n64IMID;		//IMID
	INT64				n64StuffID[1];	//���� 
CMD_END

CMD_START(NS_ConsolidateEngrave)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_ConsolidateQuench)
	DWORD				dwNPCID;		//NPCID
	DWORD				dwFormulaID;	//�䷽ID
	INT64				n64ItemID;		//Ҫ��ǿ����Ʒ��64λID
	INT64				n64IMID;		//IMID
	INT64				n64StuffID[1];	//���� 
CMD_END

CMD_START(NS_ConsolidateQuench)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	��Ƕ����ӡ������
//-----------------------------------------------------------------------------
CMD_START(NC_Inlay)
	INT64				n64SrcItemID;	//��ʯ��64ID
	INT64				n64DstItemID;	//װ����64ID
CMD_END

CMD_START(NS_Inlay)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_Brand)
	INT64				n64SrcItemID;	//ӡ�ǵ�64ID
	INT64				n64DstItemID;	//װ����64ID
	INT64				n64IMID;		//���׷�64ID ��ʹ��:GT_INVALID
CMD_END

CMD_START(NS_Brand)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_LoongSoul)
	INT64				n64SrcItemID;	//����ʯ64ID
	INT64				n64DstItemID;	//װ����64ID
CMD_END

CMD_START(NS_LoongSoul)
	DWORD				dwErrorCode;	//������
CMD_END

CMD_START(NC_Chisel)	
	INT64				n64SrcItemID;	//װ����64ID
	INT64				n64StuffID;		//��ʯ��64ID
CMD_END

CMD_START(NS_Chisel)
	DWORD				dwErrorCode;	//������
CMD_END

//-----------------------------------------------------------------------------
//	����װ��Ǳ��ֵ
//-----------------------------------------------------------------------------

CMD_START(NC_AddPot)
	INT64				n64SrcItemID;	//���ߵ�64ID
	INT64				n64DstItemID;	//װ����64ID
	INT64				n64IMID;		// ���׷�64ID ��ʹ��:GT_INVALID
CMD_END

CMD_START(NS_AddPot)
	DWORD				dwErrorCode;	//������ 0���ɹ��� 1�������� 2��ʧ�� 
	DWORD				dwPotValue;		// ���ӵ�Ǳ��ֵ
CMD_END

// Jason v1.3.1 2009-12-21 ��ʯ������Э��
CMD_START( NC_GetGemRemovalInfo )
	DWORD 				dwNPCID; 		// NPCID
	INT64				n64EquipID;		// װ������id
CMD_END

CMD_START( NS_GetGemRemovalInfo )
	DWORD				dwErrCode;		// 
	INT16				n16GemNum;		// ��ʯ���������5�ű�ʯ
	DWORD				dwGemIDs[MAX_EQUIPHOLE_NUM];	// ��ʯtypeids��0/GT_INVALID��ʾû�б�ʯ
CMD_END

CMD_START( NC_GemRemoval )
	DWORD 				dwNPCID; 		// NPCID
	INT64				n64EquipID;		// װ������id
	INT16				n16GemNum;
	INT64				n64GemRomovalSign[MAX_EQUIPHOLE_NUM];	// ��ʯ�����id
CMD_END

CMD_START( NS_GemRemoval )
	DWORD				dwErrorCode;
CMD_END


#pragma pack(pop)
		




