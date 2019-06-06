#pragma once
#include "RoleInfoProtoData.h"
#include "..\WorldDefine\msg_role_vcard.h"
#include "..\WorldDefine\role_att.h"

const tstring MALE_PIC_PATH     = _T("data\\ui\\state\\L_status-image-male.bmp");
const tstring FEMALE_PIC_PATH   = _T("data\\ui\\state\\L_status-image-female.bmp");

class HttpPicEx;

const DWORD REFRESH_DELAY = 3000;   //!< ��λ������

/**
	*class InformationFrame
	*brief ��ɫ��������
	*author hyu
	*base GameFrame
	*remark
*/
class InformationFrame : public GameFrame
{
public:
	InformationFrame(void);
	~InformationFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	//ˢ�·������ϵĽ�ɫ����������
	VOID UpdateInformation();

private:

	//��ʼ���ؼ�����
	VOID InitInfoData();

	//������Ϸframe�¼�
	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	//����ui�¼�
	DWORD EventHandler(tagGUIEvent* pEvent);

	//���������ʱ����ʡ�б�
	VOID ChangeProvince(INT areaRow);

	//�����ȡ��ɫ��������������Ϣ
	DWORD OnNS_GetRoleVCard(tagNS_GetRoleVCard* pMsg, DWORD dwParam);

	//�����ɫ��������
	VOID SaveAll();

	//�������ý�ɫ��������������Ϣ
	DWORD OnNS_SetRoleVCard(tagNS_SetRoleVCard* pMsg, DWORD dwParam);

	// ��Ҹ���
	DWORD OnNS_LPRename( tagNS_LPRename *pNetCmd, DWORD );

	// ֻ����10��������¼
	tstring GetRenameHis();

private:
	template<class TKey, class TValue>
	TValue FindValuebyKey(map<TKey, TValue>* pMap, TKey tKey);

    bool                            m_bisRefreshing;        // �Ƿ�����ˢ��

	TSFPTrunk<InformationFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;
	GUIWnd*							m_pWndFather;

	GUIStatic*						m_pStcName;					//����
	GUIStatic*						m_pStcParty;				//����
	GUIStatic*						m_pStcLevel;				//�ȼ�
	GUIStatic*						m_pStcCareer;				//ְҵ
	GUIStatic*						m_pStcPosition1;			//����ְλ
	GUIStatic*						m_pStcPosition2;

	GUIComboBox*					m_pComboBoxGender;			//�Ա�
	GUIComboBox*					m_pComboBoxConstellation;	//����
	GUIComboBox*					m_pComboBoxChZodiac;		//��Ф
	GUIComboBox*					m_pComboBoxArea;			//����
	GUIComboBox*					m_pComboBoxProvince;		//ʡ

	GUIEditBox*						m_pEditCityName;			//����

	GUIEditBoxEx*					m_pEditBoxExPresentation;	//����ǩ��
	GUIEditBoxEx*					m_pEditRenameHistory;		// ������ʷ

	HttpPicEx*						m_pStcHeadPic;				//ͷ��

	GUIButton*						m_pBtRefresh;				//ˢ��

	GUIPushButton*					m_pPbtVisible;				//������ҿɼ�

	GUIButton*						m_pBtSave;					//����

	map<INT, DWORD>					m_mapConstellationRow;		//�����ؼ����к�id��Ӧ��
	map<DWORD, INT>					m_mapConstellationID;

	map<INT, DWORD>					m_mapChineseZodiacRow;		//��Ф�ؼ����к�id��Ӧ��
	map<DWORD, INT>					m_mapChineseZodiacID;

	map<INT, DWORD>					m_mapAreaRow;				//����ؼ����к�id��Ӧ��
	map<DWORD, INT>					m_mapAreaID;

	map<INT, DWORD>					m_mapProvinceRow;			//ʡ�пؼ����к�id��Ӧ��
	map<DWORD, INT>					m_mapProvinceID;

	map<INT, DWORD>					m_mapGenderRow;				//�Ա�ؼ����к�id��Ӧ��
	map<DWORD, INT>					m_mapGenderID;


};
