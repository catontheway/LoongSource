//-----------------------------------------------------------------------------
//! filename    msg_pet_skill.h
//! author      hyu/xlguo
//! date        2009-3-24
//! last 
//! brief       ���＼����Ϣ
//! sa          pet_define.h
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push,1)

#include "pet_define.h"
#include "pet_skill_define.h"
#include "msg_common_errorcode.h"

//----------------------------------------------------------------------------
// ������ö��
//----------------------------------------------------------------------------
enum EPetSkillMsgErrCode
{
    E_PetSkill_Success              =   E_Success,	// �ɹ�
	E_PetSkill_InUse				,				// ����ʹ��
	E_PetSkill_CoolDown				,				// ��ȴ��
    E_PetSkill_Unknown				,				// δ֪
};

//-----------------------------------------------------------------------------
// ��ӣ����£�ɾ�����＼��(ֻ�Ա�����ң������ڱ���)
//-----------------------------------------------------------------------------
CMD_START(NS_AddPetSkill)
    DWORD               dwPetID;        //!< ����ID
    DWORD				dwSkillID;      //!< ��������
CMD_END

CMD_START(NS_RemovePetSkill)
    DWORD               dwPetID;        //!< ����ID
    DWORD               dwSkillID;      //!< ���＼��ID
CMD_END

//-----------------------------------------------------------------------------
// ʹ�ü���
//-----------------------------------------------------------------------------
CMD_START(NC_PetSkillUse)
	DWORD               dwPetID;        //!< ����ID
	DWORD               dwSkillID;      //!< ���＼��ID
	BYTE				byData[1];		//!< ��������
CMD_END

CMD_START(NS_PetSkillUse)
	DWORD               dwPetID;        //!< ����ID
	DWORD               dwSkillID;      //!< ���＼��ID
	DWORD				dwErrCode;		//!< ����ֵ
	BYTE				byData[1];		//!< ��������
CMD_END

//-----------------------------------------------------------------------------
// ��ֹ�����еļ���
//-----------------------------------------------------------------------------
CMD_START(NC_PetSkillStopWorking)
	DWORD               dwPetID;        //!< ����ID
	DWORD               dwSkillID;      //!< ���＼��ID
CMD_END

CMD_START(NS_PetSkillStopWorking)
	DWORD               dwPetID;        //!< ����ID
	DWORD               dwSkillID;      //!< ���＼��ID
	DWORD				dwErrCode;		//!< ����ֵ
CMD_END

//-----------------------------------------------------------------------------
// ѧϰ����
//-----------------------------------------------------------------------------
CMD_START(NC_PetLearnSkill)
	DWORD               dwPetID;        //!< ����ID
	INT64				n64ItemID;      //!< ������id
CMD_END

CMD_START(NS_PetLearnSkill)
	DWORD               dwPetID;        //!< ����ID
	INT64				n64ItemID;      //!< ������id
	DWORD				dwErrCode;		
CMD_END

CMD_START(NS_PetSkillTimer)
	DWORD				dwPetID;		//!< ����id
	DWORD				dwPetSkillID;	//!< ���＼��id
	BYTE				byType;			//!< ��ʱ����EPetTimerType
	INT					nValue;		//!< ��ʱֵ
CMD_END

#pragma pack(pop)