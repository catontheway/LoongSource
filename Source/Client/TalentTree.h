#pragma once

#include "..\WorldDefine\skill_define.h"

class TalentButton;
struct tagSkillData;
/** \struct	tagTalentLeaf
	\brief	��¼������Ҷ�ڵ��������
*/
struct tagTalentLeaf
{
	DWORD				dwTalentLeaf;				// ����ID
	ESkillUseType		eUseType;					// ����ʹ�÷�ʽ
	ETalentType			eTalentType;				// ������������
	ESkillLevelUpType	eLevelUpType;				// ������������
	BOOL				bActive;					// �Ƿ��Ͷ��
	INT					nCurLevel;					// ��ǰ�ȼ�
	INT					nLearnLevel;				// ϰ�õĵȼ�
	INT					nMaxLevel;					// ����Ͷ�ȼ�
	tagPoint			Pos;						// ����λ��
	DWORD				dwChildLeaf;				// ��������ID
	TalentButton*		pBtnTalentLeaf;				// ���ܰ�ť
	GUIStatic*			pStcMaxPoint;				// ����Ͷ������ʾ
	GUIStatic*			pStcBidden;					// ��Ͷ������ʾ
	GUIStatic*			pStcTalentBranch;			// ������֦
	TCHAR				szIconPath[X_SHORT_NAME];	// ����ͼ��

	tagTalentLeaf(){ZeroMemory(this, sizeof(tagTalentLeaf)); dwTalentLeaf = dwChildLeaf = NULL;}
};

/** \class	 TalentTree
	\brief	 ������
	\remarks �洢�����������ڵ㼰��Ӧ��ϵ
*/
class TalentTree
{
public:
	TalentTree(void);
	~TalentTree(void);
	
	/** \����������
	*/
	VOID Create(GUIWnd* pWnd, GUISystem* pGUI);
	/** \����������
	*/
	VOID Destroy();
	
	/** \��ȡҶ�ڵ㼼��ID
	*/
	DWORD GetCurLeafID(DWORD dwWndID){return m_mapIndex.Peek(dwWndID);}
	/** \��ȡҶ�ڵ����ݽṹ
	*/
	tagTalentLeaf* GetCurLeaf(DWORD dwWndID);

	/** \���õ�ǰҳ
	*/
	VOID SetCurPage(INT nPage);

	/** \����������
	*/
	VOID UpdateTalentLeaf(DWORD dwID, BOOL bAdd = FALSE);
	/** \�����������������Ӱ��
	*/
	VOID OnRoleLevelUp(INT nLevel, INT nInc);
	/** \ְҵ�仯����������Ӱ��
	*/
	VOID OnRoleClassChange(EClassType eCur, EClassType ePast);
	/** \���ʵ�仯����������Ӱ��
	*/
	VOID OnTalentPointChange(ETalentType eTalent, INT nCurPoint);
	
	/** \������������������Ӱ��
	*/
	VOID OnTalentLevelUp(DWORD dwPreTalentID);
	/** \û�����ʵ�
	*/
	VOID OnNoTalentPoint();
	/** \�����ʵ��Ͷ
	*/
	VOID OnHaveTalentPoint();
	/** \��Ͷ�������ʹﵽ����
	*/
	VOID OnNoTalentTypeCanChoose();
	/** \�������
	*/
	VOID OnClearAllTalent();
	
private:
	/** \��ȡ�����������ļ�
	*/
	VOID LoadTalentConfig(LPCTSTR szFile);
	/** \��ȡ������Ҷ�ڵ�
	*/
	VOID LoadTalentLeaf(XmlElement* pElement);
	
	/** \����һ������������
	*/
	VOID CreateWnd(ETalentType eType, GUIWnd* pFather, GUISystem* pGUI);
	/** \����һ��Ҷ�ڵ�
	*/
	VOID CreateLeaf(tagTalentLeaf* pLeaf, GUIWnd* pFather, GUISystem* pGUI);
	/** \����һ��Ҷ�ڵ����֦
	*/
	VOID CreateBranch(tagTalentLeaf* pLeaf, tagPoint ChildPos, GUIWnd* pFather, GUISystem* pGUI);
	
	/** \������Ͷ����
	*/
	VOID SetBidden(tagTalentLeaf* pLeaf);
	

	/** \��ȡ��ǰҶ�ڵ������
	*/
	VOID GetCurLeafPos(SIZE& size, tagPoint pos);
	/** \��ȡ��ǰҶ�ڵ���֦�Ĵ�С
	*/
	VOID GetCurBranchSize(SIZE& size, tagPoint pos, tagPoint child);

	/** \�ͷ�TMap������
	*/
	template<typename K, typename V>
	VOID FreeMap(TMap<K, V*> &mapList);

	/** \���ӿ���Ӱ���
	*/
	VOID AddTalentActiveEffect(DWORD dwTypeID);
	/** \����һ����Ԫ��
	*/
	template<typename A, typename B>
	VOID AddNode(TMap<A, TList<B>*> &mapList, A key, B value);

	/** \ˢ�¼��ܿ���״̬
	*/
	BOOL IsTalentLeafActive(tagTalentLeaf* pLeaf);
	/** \ˢ�¿�Ͷ�����ʼ����б�
	*/
	VOID ReflashActiveTalentList(tagTalentLeaf* pLeaf);

	/** \��ȡ��һ����TypeID
	*/
	DWORD GetNextLevelTalentTypeID(tagTalentLeaf* pLeaf);
	
	/** \��������Tips
	*/
	VOID ReflashTalentTips(tagTalentLeaf* pLeaf);

private:
	TObjRef<Util>	m_pUtil;
	
	//--���������ݴ洢�������飬���м�ֵΪ������Ҷ�ڵ�����ID
	TMap<DWORD, tagTalentLeaf*>		m_mapTalent;	
	//--������������ͨ����Ӧ�ؼ�ID�ҵ���Ӧ�ļ���ID
	TMap<DWORD, DWORD>				m_mapIndex;
	//--�����������ı����ؼ�
	GUIWnd*							m_pWndTalent[ETT_End];
	
	//--��Ͷ�����ʼ����б�
	TList<DWORD>					m_listActiveTalent[ETT_End];
	//--���ʵ����Լ��ܵĿ���Ӱ��
	TMap<INT, TList<DWORD>*>		m_mapTalentPointEffect[ETT_End];
	//--���ܵȼ��Ժ������ܵĿ���Ӱ��
	TMap<DWORD, TList<DWORD>*>		m_mapTalentSkillEffect;
	//--��ҵȼ��Լ��ܵĿ���Ӱ��
	TMap<INT, TList<DWORD>*>		m_mapRoleLevelEffect;
	//--���ְҵ�Լ��ܵĿ���Ӱ��
	TMap<EClassType, TList<DWORD>*>	m_mapRoleClassEffect;


	INT								m_nCurPage;	// ��ǰҳ
};


