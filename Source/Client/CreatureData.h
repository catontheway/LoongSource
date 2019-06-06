#pragma once
#include "..\WorldDefine\creature_define.h"
#include ".\BaseData.h"

/** \����ģ���������
*/
struct tagCreatureMdlData
{
	float   fBeAttackSfxScale;		// ��������Ч����ϵ��
	bool    bCanPitch;				// �Ƿ���
	tstring szDeadSound;			// ������Ч
	tstring szCombatSound;			// ����ս����Ч
	tstring szRoarSound;			// ����������Ч
	tstring szHowlSound;			// ���򺰽���Ч
	tstring szFootSound;			// �Ų�����ǰ׺����׺Ϊ�������ͣ�
	tstring szNormalAttackBeAtkSfx;	// ��ͨ�����Է���������Ч
};

/** class	CreatureData
	brief	���NPC���������ԭʼ���Խṹ�洢
*/
class CreatureData 
	: public BaseData
{
public:
	CreatureData(void);
	~CreatureData(void);
	
	static CreatureData* Inst();
	
	/** ���������ļ�
	*/
	virtual void LoadFromFile();
	/** ��ȡNPCģ���ļ�·��
	*/
	static tstring GetNpcMdlPath(const tagCreatureProto *pNpcAtt);
	/**	��ȡNPC��Ƥ�ļ�·��
	*/
	static tstring GetNpcSkinPath(const tagCreatureProto *pNpcAtt, int index);
	/** ����NPC��ԭʼ����
	*/
	const tagCreatureProto* FindNpcAtt(DWORD attID)
	{
		return FindInMap(m_creatureMap,attID);
	}
	/** ��ȡ���е�NPCԭʼ����
	*/
	void GetNpcMap(map<DWORD,tagCreatureProto>& npcMap)
	{
		npcMap = m_creatureMap;
	}
	/** ��ȡģ���������
	*/
	const tagCreatureMdlData* FindMdlData(const TCHAR* szMdlName);
	/** ��ȡ��������
	*/
	const tstring FindNpcBoneName( const DWORD dwTypeID, const TCHAR* szTagNodeName );
	const tstring FindNpcBoneName( const TCHAR* szMdlName, const TCHAR* szTagNodeName );
	/**��ȡ���ֵ
	*/
	INT FindCreatureInspiration(DWORD dwTypeID);
private:
	DWORD MakeNpcBoneNameKey( const TCHAR* szMdlName, const TCHAR* szTagNodeName );
private:
	map<DWORD,tagCreatureProto>			m_creatureMap;
	map<DWORD,tagCreatureMdlData>		m_mdlData;
	map<DWORD,tstring>					m_npcBoneNameMap;
	map<DWORD,INT>						m_mapInspiration;
};
