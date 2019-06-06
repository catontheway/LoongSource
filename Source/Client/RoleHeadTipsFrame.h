#pragma once
#include "Render2D.h"
#include "RoleQuestFlag.h"
#include "RoleTitleFlag.h"
#include "RoleTitleProtoData.h"
struct tagPKStateStartCountDownEvent;
struct tagHeadTip
{
	DWORD		roleID;
	tstring		strNameTip;			//��ʾ����ҵ�����
	tstring		strTitleTip;		//��ʾ����ҵĳƺ� added by hyu
	tstring		strGuildTip;		//��ʾ����ҵİ��� added by hqzhang
	tstring		strVipNetBarTip;	//��ʾ����ҵĽ����������� added by yfwang
    tstring     strSpouse;          //���޳�ν added by hyu
    Color4ub    spouseColor;        //���޳�ν����ɫ added by hyu
	Vector3		scrPos;				//��ɫͷ����Ļ����λ��
	Vector3		worldPos;			//��ɫͷ������������
	Color4ub	txtNameColor;		//��ʾ��������ɫ
	Color4ub	txtTitleColor;		//��ʾ�ƺŵ���ɫ added by hyu
	EQuestFlag	qeustFlag;
	tstring		strResTex;			//�ƺ���ͼ
	float		scale;
	RECT		rectName;
	DWORD		dwHostility;		// ����

	tagHeadTip(): roleID(GT_INVALID)
	{
	}

	tagHeadTip(const tagHeadTip& other)
	{
		Copy(other);
	}

	const tagHeadTip& operator = (const tagHeadTip& other)
	{
		if(this!=&other)
			Copy(other);
		return *this;
	}

private:
	void Copy(const tagHeadTip& other)
	{
		roleID		= other.roleID;
	}

};

class Role;
class RoleHeadTipsFrame :
	public GameFrame
{

public:
	RoleHeadTipsFrame(void);
	~RoleHeadTipsFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Render3D();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual VOID Update();

	void Create();

	void SetIsShowHeadTips(BOOL bVal) { m_bShowTips = bVal; }

private:
	void ClearAllTips();
	void DrawAllRoleHeadTips();
	void UpdateRoleHeadTips();
	Color4ub GetRoleTipsColor(Role* pRole);
    const tstring GetRoleTitleTips(Role* pRole);
    Color4ub GetRoleTitleColor(Role* pRole);
	const tstring GetRoleTitleIcon(Role* pRole);
	const tstring GetRoleGuildTips(Role* pRole);
	const tstring GetRoleVipNetBarTips( Role* pRole );
	const DWORD GetRoleHostilityTips( Role* pRole );
    const tstring GetRoleSpouse(Role* pRole);
    Color4ub GetRoleSpouseColor(Role* pRole);

	//����Ѫ����������
	void UpdateLocalHPorMPProgress();
	void UpdateOtherHPorMPProgress();

	//Ѫ����������
	DWORD OnKey(DWORD dwID, BOOL bDown);

private:
	TSFPTrunk<RoleHeadTipsFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pSystem;
	TObjRef<Util>					m_pUtil;
	TObjRef<GUIRender>				m_pRender;
	TObjRef<GameInputMap>			m_pKeyMap;

	IDepthFont*						m_pFont;
	IDepthFont*						m_pFontTitle;		//�ƺ�ʹ��

	vector<tagHeadTip*>				m_vecTipsList;
	RoleQuestFlag					m_questFlag;
	RoleTitleFlag					m_titleFlag;		//�ƺ���ͼ
	ResTexture*						m_pResHP;
	ResTexture*						m_pResMP;

	BOOL							m_bShowTips;
	BOOL							m_bShowHPorMP;

	enum EHostilityValue
	{
		EHostility_Start = -1,
		EHostility_Zero,
		EHostility_One,
		EHostility_Two,
		EHostility_Three,
		EHostility_Four,
		EHostility_Five,
		EHostility_Six,
		EHostility_Seven,
		EHostility_Eight,
		EHostility_Nine,
		EHostility_End,
	};
};