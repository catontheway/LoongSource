#pragma once

#define RECOMMEND_GAMEWORLD 8
#define BOROUGH_MAX			44
#define GAMEWORLD_MAX		16

struct tagGameWorld											//��Ϸ����
{
	DWORD	dwWorldRank;									//��Ϸ����ı��
	TCHAR	szWorldName[X_SHORT_NAME];						//��Ϸ�������
	DWORD	dwWorldID;										//����ID
	INT		nStatus;										//��Ϸ�����״̬				
	DWORD	dwStatus;										//״̬����ɫ
	DWORD	dwWorldIP;										//��Ϸ�����IP
	DWORD	dwWorldPort;									//��Ϸ�����port
	BYTE	byRecommend;									//�Ƿ�Ϊ�Ƽ���Ϸ����
	DWORD	dwRecoRank;										//�Ƽ���Ϸ����ı��
	BYTE	byGreenFlag;									//��ɫ���
};

struct tagBorough											//����
{
	DWORD					dwBoroughRank;					//��Ϸ����ı��
	TCHAR					szBoroughName[X_SHORT_NAME];	//������
	DWORD					dwBoroughID;					//����ID
	INT						dwWorldNum;						//��������Ϸ�������
	vector<tagGameWorld>	vGameWorld;						//��������Ϸ����
};
/** ������������ѡ��
*/

class Client;
class ServerSelectFrame : public GameFrame
{
public:
	ServerSelectFrame();
	~ServerSelectFrame();

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);

	// �õ����������ص���Ϸ����״̬��ɫ
	DWORD GetWorldStatusColor(const INT nWorldStatus);

	// ��������
	void CalDate();

	//ȷ�ϵ�¼
	void ConfirmLogin();

	//�ʺ�ע��
	void AccountRegister();

	//�ʺų�ֵ
	void AccountAbundance();

	//�˳���Ϸ
	void ExitGame();

	//��ȡ������Ϸ����������ļ�
	void LoadConfigFile();

	//��ȡ�ϴε�¼������Ϸ����
	void LoadLastLogin();

	//�����Ƽ���Ϸ����״̬
	void SetRecommedPushButtonState();

	//������Ϸ���簴ť״̬
	void SetWorldPushButtonState();

	//��ʾ��Ϸ���簴ť
	void ShowWorldPushButton();

	//��ʾ����\����Ļ����
	void ShowServerActive();

private:
	TSFPTrunk<ServerSelectFrame>m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<StreamTransport>	m_pStream;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<VarContainer>		m_pVarIPSaver;
	TObjRef<Client>				m_pClient;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIPushButton*				m_pRecommend[RECOMMEND_GAMEWORLD];	//�Ƽ���Ϸ���簴ť
	GUIPushButton*				m_pBorough[BOROUGH_MAX];			//������ť
	GUIPushButton*				m_pGameWorld[GAMEWORLD_MAX];		//��Ϸ���簴ť
	GUIPushButton*				m_pLastLogin;						//�ϴε�¼����Ϸ���簴ť		
	GUIStatic*					m_pLastServer;						//�ϴε�¼�Ĵ���
	GUIButton*					m_pOK;								//ȷ�ϵ�¼
	GUIButton*					m_pRegister;						//ע��
	GUIButton*					m_pAbundance;						//��ֵ
	GUIButton*					m_pExit;							//�뿪
	GUIListBox*					m_pToday;							//���ջ
	GUIListBox*					m_pTomorrow;

	GUIWnd*						m_pWnd_Back;						//�󱳾�
	GUIStatic*					m_pStc_Pic;							//������ͼ

	INT							m_nBorough;							//ѡ��Ĵ�ű��
	INT							m_nGameWorld;						//ѡ�����Ϸ������
	INT							m_nRecoWorld;						//ѡ����Ƽ���Ϸ������
	vector<tagBorough>			m_vBrough;							//����
	vector<tagGameWorld>		m_vRecommend;						//�Ƽ�����Ϸ����

	tstring						m_strRegister;						//ע���ʺŵ���ַ
	tstring						m_strAbundance;						//�ʺų�ֵ����ַ

	tstring						m_strLastIP;						//�ϴε�¼��ip
	DWORD						m_dwLastPort;						//�ϴε�¼��port
	tstring						m_strLastWorldName;					//�ϴε�¼����Ϸ����
	tstring						m_strLastServer;					//�ϴε�¼�Ĵ���

	tstring						m_strGameWorld;						//ѡ�����Ϸ������
	tstring						m_strIP;							//ѡ�����Ϸ����IP
	DWORD						m_dwPort;							//ѡ�����Ϸ����Port
};