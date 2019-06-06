#pragma once


struct tagSetting
{
	tstring strName;
	DWORD   dwValue;
};

enum EGameSettings
{
	// ��������
	EGS_Gra_Start,
	EGS_PelsWidth = EGS_Gra_Start,			// �ֱ��ʿ�
	EGS_PelsHeight,                         // �ֱ��ʸ�
	EGS_BitsPerPel,                         // λ��
	EGS_DisplayFrequency,                   // ˢ����
	EGS_MultiSampleType,                    // �����
	EGS_Windowed,                           // ����ģʽ
	EGS_VerSynch,                           // ��ֱͬ��
	EGS_TerrainDetail,                      // �ر���
	EGS_NormalMap,                          // ������ͼ
	EGS_HWSkin,                             // GPU����
	EGS_TexDetail,                          // ��ͼ����
	EGS_TerrainSpecular,                    // ���θ���
	EGS_FarView,                            // Զ����Ұ
	EGS_RoleView,                           // ��ɫ��Ұ
	EGS_EffectDetail,                       // ��Чϸ��
	EGS_SceneDetail,                        // ����ϸ��
	EGS_WaterSpecular,                      // ˮ�����
	EGS_WaterReflect,                       // ˮ�淴��
	EGS_WaterRefract,                       // ˮ������
	EGS_Lambency,                           // ���Ч��
	EGS_Weather,                            // ����Ч��
	EGS_SunHalo,                            // ̫������
	EGS_RoleShadow,                         // ��ɫ��Ӱ
	EGS_DrawGrass,                          // ������ʾ
	EGS_Gra_End = EGS_DrawGrass,

	// ��Ϸ����
	EGS_Game_Start,
	EGS_Music = EGS_Game_Start,				// ���ֿ���
	EGS_Sound,								// ��Ч����
	EGS_MusicVol,							// ��������
	EGS_SoundVol,							// ��Ч����
	EGS_CombatMusic,						// ս������
	EGS_ShowSelfName,						// ��ʾ��������
	EGS_ShowOtherPlayerName,				// ��ʾ�����������
	EGS_ShowFactionName,					// ��ʾ��������
	EGS_ShowChatPop,						// ��ʾ��������
	EGS_ShowOtherPalyerTitle,				// ��ʾ������ҳƺ�
	EGS_ShowSkillNameFloatTip,				// ��ʾ��������Ʈ��
	EGS_ShowHPMPText,						// ��ʾHP MP����
	EGS_ShowMonsterName,					// ��ʾ��������
	EGS_Game_End = EGS_ShowMonsterName,

	// С��ͼ����
	EGS_MM_Start,
	EGS_MM_ShowNatuRes = EGS_MM_Start,  	// �ɼ���
	EGS_MM_ShowNest,						// ���ﳲѨ
	EGS_MM_ShowNormalMonster,				// ����
	EGS_MM_ShowBoss,						// BOSS
	EGS_MM_ShowNormalNPC,					// NPC
	EGS_MM_ShowAcceptQuestNPC,				// �ɽ�����NPC
	EGS_MM_ShowCompleteQuestNPC,			// �ɽ�����NPC
	EGS_MM_ShowTeammate,					// ����
	EGS_MM_ShowOtherPlayer,					// �������
	EGS_MM_End = EGS_MM_ShowOtherPlayer,

	// ����
	EGS_Console,							// ����̨
	EGS_GmConsle,							// GM����̨
	EGS_QuestHelp,							// �������
	EGS_MoralityWarning,					// ���¾���
	EGS_Num
};


/** \class GameSet
    \brief ��Ϸ���ã����ڹ�����Ϸ��������
*/
class GameSet
{
public:
	enum { MAX_MUSIC_VOLUME = 100, MAX_SOUND_VOLUME = 100 };
public:
	/** \��õ���ʵ��ָ��
	*/
	static GameSet* Inst() { static GameSet gs; return &gs; }
	/** \��ʼ������������
	*/
	BOOL	Init();
	BOOL	Destroy();
	/** \��ȡ����
	*/
	DWORD	Get( const INT nIndex ) const;
	/** \�޸�����
	*/
	BOOL	Set( const INT nIndex, const DWORD val );
	/** \Ӧ����������
	*/
	VOID	ApplyAll();
	/** \������������
	*/
	BOOL	SaveAll();
	/** \����Ĭ�ϻ�������
	*/
	VOID	BuildDefaultGraphicSettings();
	/** \����С��ͼ����
	*/
	BOOL    SaveMiniMapSettings();
	VOID	SaveQuestHelpSet();
	VOID	SaveMorality();
private:
	BOOL	Load();
	BOOL	ApplySet( const INT nIndex ) const;
	DWORD	GetDefaultValue( const INT nIndex ) const;
private:
	TObjRef<VarContainer>           m_pCfgVars;                   //GameConfig��������
	std::vector<tagSetting>         m_vecSettings;                //��Ϸ��������
	TCHAR                           m_szFilename[MAX_PATH];       //�����ļ���
	DWORD                           m_dwPixelShaderVersion;
	tstring                         m_strPixelShaderVersion;
private:
	GameSet();
	~GameSet();
};