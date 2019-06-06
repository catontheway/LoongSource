#pragma once

struct tagComboWHB                                              //�ֱ��ʿ��ߣ�λ��
{
	DWORD dwPelsWidth;
	DWORD dwPelsHeight;
	DWORD dwBitsPerPel;
	Cool3D::EPixelFormat eFormat;   
	tagComboWHB( DWORD w, DWORD h, DWORD bpp, Cool3D::EPixelFormat fmt ) : 
	         dwPelsWidth( w ), dwPelsHeight( h ), dwBitsPerPel( bpp ), eFormat( fmt ) { }
};

struct tagMultiSampleCfg
{
	EMultisampleType eLevel;
	tstring strShowText;
	tagMultiSampleCfg(){}
	tagMultiSampleCfg( EMultisampleType level, tstring strText ) : eLevel( level ), strShowText( strText ){}
};

/** \class GameSettingsFrame
    \brief ��Ϸ���ý���
*/
class GameSettingsFrame : public GameFrame
{
public:
	GameSettingsFrame();
	~GameSettingsFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
private:
	enum { SCROBAR_TOTAL = 102 , SCROBAR_VISIBLE = 2 };

	TSFPTrunk<GameSettingsFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*							m_pWnd;						  // ������
	GUIWnd*							m_pPanel;					  // �װ�
	GUIButton*                      m_pBn_Close;                  // �رհ�ť
	GUIPushButton*                  m_pBn_GraphicSet;             // �������ð�ť
	GUIPushButton*                  m_pBn_GameSet;                // ��Ϸ���ð�ť
	GUIWnd*                         m_pPanel_GraphicSet;          // �����������
	GUIWnd*                         m_pPanel_GameSet;			  // ��Ϸ�������
	GUIButton*                      m_pBn_AutoCheck;              // �Զ���ⰴť
	GUIButton*                      m_pBn_OK;                     // ȷ����ť
	GUIButton*                      m_pBn_Cancel;                 // ȡ����ť
	GUIButton*                      m_pBn_Apply;                  // Ӧ�ð�ť

	// ��������
	GUIComboBox*                    m_pComb_WHB;                  // �ֱ��ʿ�
	GUIComboBox*                    m_pComb_Hz;                   // ˢ���ʿ�
	GUIComboBox*                    m_pComb_MultiSample;          // ����ݿ�
	GUIPushButton*                  m_pBn_Windowed_Off;           // ����ģʽ��
	GUIPushButton*                  m_pBn_Windowed_On;            // ����ģʽ��
	GUIPushButton*                  m_pBn_VerSynch_Off;           // ��ֱͬ����
	GUIPushButton*                  m_pBn_VerSynch_On;            // ��ֱͬ����
	GUIPushButton*                  m_pBn_TerrainDetail_Low;      // �ر��ȵ�
	GUIPushButton*                  m_pBn_TerrainDetail_High;     // �ر��ȸ�
	GUIPushButton*                  m_pBn_NormalMap_Off;          // ������ͼ��
	GUIPushButton*                  m_pBn_NormalMap_On;           // ������ͼ��
	GUIPushButton*                  m_pBn_HWSkin_Off;             // GPU������
	GUIPushButton*                  m_pBn_HWSkin_On;              // GPU������
	GUIPushButton*                  m_pBn_TexDetail_Low;          // ��ͼ���ȵ�
	GUIPushButton*                  m_pBn_TexDetail_Mid;          // ��ͼ������
	GUIPushButton*                  m_pBn_TexDetail_High;         // ��ͼ���ȸ�
	GUIPushButton*                  m_pBn_TerrainSpecular_Off;    // ���θ�����
	GUIPushButton*                  m_pBn_TerrainSpecular_On;     // ���θ�����
	GUIScrollBar*                   m_pScro_FarView;              // Զ����Ұ������
	GUIScrollBar*                   m_pScro_RoleView;             // ��ɫ��Ұ������
	GUIScrollBar*                   m_pScro_EffectDetail;         // ��Чϸ�ڹ�����
	GUIScrollBar*                   m_pScro_SceneDetail;          // ����ϸ�ڹ�����
	GUIPushButton*                  m_pBn_WaterSpecular_Off;      // ˮ�������
	GUIPushButton*                  m_pBn_WaterSpecular_On;       // ˮ�������
	GUIPushButton*                  m_pBn_WaterReflect_Off;       // ˮ�淴���
	GUIPushButton*                  m_pBn_WaterReflect_Mid;       // ˮ�淴����
	GUIPushButton*                  m_pBn_WaterReflect_High;      // ˮ�淴���
	GUIPushButton*                  m_pBn_WaterRefract_Off;       // ˮ�������
	GUIPushButton*                  m_pBn_WaterRefract_On;        // ˮ�����俪
	GUIPushButton*                  m_pBn_Lambency_Off;           // ���Ч����
	GUIPushButton*                  m_pBn_Lambency_On;            // ���Ч����
	GUIPushButton*                  m_pBn_Weather_Off;            // ����Ч����
	GUIPushButton*                  m_pBn_Weather_On;             // ����Ч����
	GUIPushButton*                  m_pBn_SunHalo_Off;            // ̫�����ι�
	GUIPushButton*                  m_pBn_SunHalo_On;             // ̫�����ο�
	GUIPushButton*                  m_pBn_DrawGrass_Off;          // ������ʾ��
	GUIPushButton*                  m_pBn_DrawGrass_On;           // ������ʾ��
	GUIPushButton*                  m_pBn_RoleShadow_Off;         // ��ɫ��Ӱ��
	GUIPushButton*                  m_pBn_RoleShadow_On;          // ��ɫ��Ӱ��

	// ��Ϸ����
	GUIPushButton*                  m_pBn_Music_Off;			  // ���ֿ���
	GUIPushButton*					m_pBn_Music_On;
	GUIScrollBar*					m_pScro_MusicVolume;          // ��������
	GUIPushButton*                  m_pBn_Sound_Off;			  // ��Ч����
	GUIPushButton*					m_pBn_Sound_On;
	GUIScrollBar*					m_pScro_SoundVolume;          // ��Ч����
	GUIPushButton*                  m_pBn_CombatMusic_Off;		  // ս�����ֿ���
	GUIPushButton*                  m_pBn_CombatMusic_On;
	GUIPushButton*                  m_pBn_AcceptTeamInvite_Off;   // �Ƿ�����������
	GUIPushButton*                  m_pBn_AcceptTeamInvite_On;
	GUIPushButton*                  m_pBn_AcceptFactionInvite_Off;// �Ƿ���ܰ�������
	GUIPushButton*                  m_pBn_AcceptFactionInvite_On;
	GUIPushButton*                  m_pBn_AllowFriendAdd_Off;	  // �Ƿ���ܺ�������
	GUIPushButton*                  m_pBn_AllowFriendAdd_On;
	GUIPushButton*                  m_pBn_AcceptTradeRequest_Off; // �Ƿ�ӽ�������
	GUIPushButton*                  m_pBn_AcceptTradeRequest_On;
	GUIPushButton*                  m_pBn_SelfInfoOpenType_Private;// ������Ϣ����
	GUIPushButton*                  m_pBn_SelfInfoOpenType_Friend; // �����ѿɼ�
	GUIPushButton*                  m_pBn_SelfInfoOpenType_Public; // �����˿ɼ�
	GUIPushButton*                  m_pBn_ShowSelfName;           // ��ʾ��������
	GUIPushButton*                  m_pBn_ShowOtherPlayerName;    // ��ʾ�����������
	GUIPushButton*                  m_pBn_ShowFactionName;        // ��ʾ��������
	GUIPushButton*                  m_pBn_ShowChatPop;            // ��ʾ��������
	GUIPushButton*                  m_pBn_ShowHat;                // ��ʾñ��
	GUIPushButton*                  m_pBn_ShowMantle;             // ��ʾ����
	GUIPushButton*                  m_pBn_ShowOtherPalyeTitle;    // ��ʾ������ҳƺ�
	GUIPushButton*                  m_pBn_ShowSkillNameFloatTip;  // ��ʾ��������Ʈ��
	GUIPushButton*                  m_pBn_ShowHPMPText;           // ��ʾHP MP����
	GUIPushButton*                  m_pBn_ShowMonsterName;        // ��ʾNPC ��������

	BOOL GUIEventHandler(tagGUIEvent* pEvent);				      // �¼���Ӧ�ص�����

	std::vector<tagComboWHB>        m_vecComboWHB;                // �ֱ����������б�
	std::vector<DWORD>              m_vecComboHz;                 // ˢ�����������б�
	std::vector<tagMultiSampleCfg>  m_vecComboMultiSample;        // ������������б�
	INT                             m_nSelectWHB;                 // ��ǰѡ��ķֱ�������
	INT                             m_nSelectHz;                  // ��ǰѡ���ˢ��������
	INT                             m_nSelectMultiSample;         // ��ǰѡ��Ŀ��������

	tagDWPlayerPersonalSet          m_curPersonalSet;			  // ��ǰ�ĸ�������
	tagDisplaySet					m_curDisplaySet;              // ��ǰ��װ����ʾ����
	TObjRef<NetSession>				m_pSession;
	DWORD							m_dwLastApplyTime;
private:
	VOID SafeDestroyUI();
	VOID EnableControls();                                        // ���ÿؼ��Ƿ�ɲ���
	BOOL SettingsToUI();                                          // ����������Ϣ����UI״̬
	BOOL UIToSettings();                                          // ����UI״̬�޸�������Ϣ
	VOID BuildDisplayModeList();                                  // ������ʾģʽ�б�
	UINT PixelFormatToBpp( Cool3D::EPixelFormat eFormat );        // �����ظ�ʽ�õ�λ��
	int CalcScrobarStepValue( const int nCurScroValue, const int nMaxStep );
	void ApplySettings();										  // Ӧ������
};

