#pragma once

class Item;
struct tagRoleBuff;
struct tagItemCost;
/** \class ToolTipCreator
	\brief ��ʾ��Ϣ������
*/
class ToolTipCreator
{
public:
	ToolTipCreator(void);
	~ToolTipCreator(void);

	static ToolTipCreator* Inst();

	tstring GetItemTips(Item* pItem);
	tstring GetItemTips(DWORD dwID, BYTE byQuality = 0, DWORD dwNameId = GT_INVALID);
	tstring GetSkillTips(DWORD dwID);
	tstring GetTalentTips(DWORD dwID);
	tstring GetBuffTips(const tagRoleBuff* pBuff);
    tstring GetPriceTips(const tagItemCost* pCost, BOOL bSingle = FALSE);
    tstring GetStallPriceTips(const tagItemCost* pCost, int nCount);
    tstring GetPetSkillTips(DWORD dwID);
	tstring GetCommerceGoodsTips(DWORD dwGoodID);
	tstring GetGuildSkillTips(DWORD dwID);
	tstring GetHotKeyTips( LPCTSTR szBaseTip, DWORD key1, DWORD key2, LPCTSTR szSpace = NULL, bool bParenthesis = true );

	VOID ShowEquipCompare(DWORD dwTypeID);

    // ������̬tips����
    tstring ParseDynamicTips(LPCTSTR szInput);
    tstring ParseKey(tstring& strName, tstring& strValue);
};
