//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_skill.h
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: ���＼��
//-------------------------------------------------------------------------------------------------------
#pragma once

struct	tagDBPetSkill;
struct	tagPetSkillProto;
struct	tagPetSkillMsgInfo;
struct	tagPetSkillCmdParam;

class PetSoul;
class Role;

// �����ջ�ʱ��
const INT	COUNT_DOWN_GAIN		= 5 * 60 * TICK_PER_SECOND;

// ����ʹ��ʱ��
const INT	COUNT_DOWN_USING	= 2 * 60 * 60 * TICK_PER_SECOND;



//----------------------------------------------------------------------------------------------------
// ���＼��
//----------------------------------------------------------------------------------------------------
class PetSkill
{
protected:
	PetSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1 = GT_INVALID, INT nPara2 = GT_INVALID);
	virtual	~PetSkill(){}	

public:
	const tagPetSkillProto* GetProto() const { return m_pProto; }

	//----------------------------------------------------------------------------------------------------
	// ������ɾ��
	//----------------------------------------------------------------------------------------------------
	static PetSkill*	CreatePetSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1 = GT_INVALID, INT nPara2 = GT_INVALID);
	static PetSkill*	CreateDBPetSkill(tagDBPetSkill* pDBPetSkill, PetSoul* pSoul);
	static VOID			DeletePetSkill(PetSkill* p2Delete)	{	SAFE_DEL(p2Delete);	}
	BOOL				SaveToDB( tagDBPetSkill* pData);

	//----------------------------------------------------------------------------------------------------
	// ��д������Ϣ
	//----------------------------------------------------------------------------------------------------
	VOID				FillClientInfo(tagPetSkillMsgInfo* pInfo);
	DWORD				GetSkillTypeID() const;
	BYTE				GetCastCondition() const;

	//----------------------------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------------------------
	virtual VOID		Update();

	//----------------------------------------------------------------------------------------------------
	// ��ȴ
	//----------------------------------------------------------------------------------------------------
	BOOL				IsCoolDowning()		{		return GT_VALID(m_nCoolDownTick);				}
	VOID				SetCoolDowning(INT nCoolDownTick = 0);
	VOID				CoolDowning();

	BOOL				IsWorkCounting()	{		return GT_VALID(m_nWorkCountTick);				}
	VOID				SetWorkCounting(INT nTickAdd = 0);
	BOOL				WorkCounting();

	VOID				BreakWork();

	//----------------------------------------------------------------------------------------------------
	// ����
	//----------------------------------------------------------------------------------------------------
	BOOL				CanSetWorking( BOOL bWorking );
	BOOL				SetWorking(BOOL bWorking);
	VOID				SendPetSkillMsg(PVOID pData, DWORD dwSize, DWORD dwErrCode);	


protected:
	PetSoul*			GetSoul()	const			{		return m_pSoul;				}
	Role*				GetMaster() const;

	VOID				SendTimerMsg(DWORD dwPtt, INT nValue);

	INT					m_nCoolDownTick;
	INT					m_nWorkCountTick;

private:
	const tagPetSkillProto* m_pProto;
	PetSoul*			m_pSoul;
};

//----------------------------------------------------------------------------------------------------
// �����ӿ�
//----------------------------------------------------------------------------------------------------
class PassiveSkill : public PetSkill
{
public:
	PassiveSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1 = GT_INVALID, INT nPara2 = GT_INVALID)
		:PetSkill(dwSkillTypeID, pSoul, nPara1, nPara2){}
public:
	virtual BOOL		Active() = 0;
	virtual BOOL		DeActive() = 0;
};

//----------------------------------------------------------------------------------------------------
// �����ӿ�
// �������ܵ�ʹ�÷�Ϊ��������
// PreHandleCmdImpl�Լ��ܽ��������жϣ���ʧ�ܣ��򲻼���ִ�У��������ù���ʱ��
// �ȹ���ʱ�����ִ��PastHandleCmd
//----------------------------------------------------------------------------------------------------
class ActiveSkill : public PetSkill
{
public:
	ActiveSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1 = GT_INVALID, INT nPara2 = GT_INVALID)
		:PetSkill(dwSkillTypeID, pSoul, nPara1, nPara2), m_RecvSkillData(0, 0){}
	~ActiveSkill()	{		SetRecvSkillData(0, NULL);	}

public:
	DWORD			PreHandleCmd(PVOID pSkillData, DWORD dwSkillDataSize, PVOID pSendData, DWORD &dwSendSize);
	virtual DWORD	PreHandleCmdImpl(INT &nWorkingAdd) = 0;
	DWORD			PastHandleCmd();
	virtual DWORD	PastHandleCmdImpl(INT &nCoolDownAdd) = 0;

protected:
	VOID			SetSendSkillData(DWORD& dwSize, PVOID pData);
	PVOID			GetSendSkillData(DWORD* &dwSize);
	VOID			SetRecvSkillData(DWORD dwSize, PVOID pData);
	PVOID			GetRecvSkillData(DWORD &dwSize);

private:
	std::pair<DWORD, PVOID>		m_RecvSkillData;
	std::pair<DWORD*, PVOID>	m_SendSkillData;
};


//----------------------------------------------------------------------------------------------------
// ���������ӿ�
//----------------------------------------------------------------------------------------------------
class EnhanceSkill : public PetSkill
{
public:
	EnhanceSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1 = GT_INVALID, INT nPara2 = GT_INVALID)
		:PetSkill(dwSkillTypeID, pSoul, nPara1, nPara2){}
public:
	virtual BOOL	Open() = 0;
	virtual BOOL	Close() = 0;
};

//----------------------------------------------------------------------------------------------------
// ����ǿ������
//----------------------------------------------------------------------------------------------------
class PetMountAddSkill:public EnhanceSkill
{
	friend class PetSkill;

	PetMountAddSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2) 
		:EnhanceSkill(dwSkillTypeID, pSoul, nPara1, nPara2),m_bInUsing(FALSE){}

private:
	BOOL		Open();
	BOOL		Close();

private:
	BOOL		m_bInUsing;
};

//----------------------------------------------------------------------------------------------------
// ����ǿ����
//----------------------------------------------------------------------------------------------------
class PetStrengthSkill:public PassiveSkill
{
	friend class PetSkill;
	
	PetStrengthSkill(DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2) 
		:PassiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2),m_bInUsing(FALSE){}

private:
	BOOL		Active();
	BOOL		DeActive();

private:
	BOOL		m_bInUsing;
};

//----------------------------------------------------------------------------------------------------
// ����ιҩ����
//----------------------------------------------------------------------------------------------------
class PetMedicineFeedSkill : public ActiveSkill
{
	friend class PetSkill;
	PetMedicineFeedSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2){}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);
};

//----------------------------------------------------------------------------------------------------
// ����ʰȡ����
//----------------------------------------------------------------------------------------------------
class PetPickUpSkill : public ActiveSkill
{
	friend class PetSkill;
	PetPickUpSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);
};

//----------------------------------------------------------------------------------------------------
// �����츳����
//----------------------------------------------------------------------------------------------------
class PetWuXingSkill : public ActiveSkill
{
	friend class PetSkill;
	PetWuXingSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);
};

//----------------------------------------------------------------------------------------------------
// ����״̬����
//----------------------------------------------------------------------------------------------------
class PetBuffSkill : public ActiveSkill
{
	friend class PetSkill;
	PetBuffSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);

	BOOL		bInUse;
};

//----------------------------------------------------------------------------------------------------
// ����ɼ�����
//----------------------------------------------------------------------------------------------------
class PetGatherSkill : public ActiveSkill
{
	friend class PetSkill;
	PetGatherSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);

};

//----------------------------------------------------------------------------------------------------
// ����С������
//----------------------------------------------------------------------------------------------------
class PetSaleSkill : public ActiveSkill
{
	friend class PetSkill;
	PetSaleSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);

};

//----------------------------------------------------------------------------------------------------
// ����ѵ������
//----------------------------------------------------------------------------------------------------
class PetExperanceSkill : public ActiveSkill
{
	friend class PetSkill;
	PetExperanceSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);

};

//----------------------------------------------------------------------------------------------------
// ����ֹܼ���
//----------------------------------------------------------------------------------------------------
class PetStoreHouseSkill : public ActiveSkill
{
	friend class PetSkill;
	PetStoreHouseSkill( DWORD dwSkillTypeID, PetSoul* pSoul, INT nPara1, INT nPara2 )
		:ActiveSkill(dwSkillTypeID, pSoul, nPara1, nPara2)	{}

private:
	DWORD PreHandleCmdImpl(INT &nWorkingAdd);
	DWORD PastHandleCmdImpl(INT &nCoolDownAdd);

};

