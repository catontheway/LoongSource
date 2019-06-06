#pragma once

class Player;
/** \class AvatarAniMixer
	\brief ��ҽ�ɫ���������
*/
class AvatarAniMixer
{
public:
	enum EAniPart			//������λ
	{
		EAP_Upper,			//������
		EAP_Lower,			//������
		EAP_All,			//ȫ����
	};

	enum EPriority			//�������ȼ�
	{
		EPri_Lower,			//��
		EPri_Normal,		//��ͨ
		EPri_High,			//��
	};

private:
	enum EPlayState			//��������״̬
	{
		EPS_None,			//δ�����κζ���
		EPS_All,			//ȫ����
		EPS_Upper,			//������
		EPS_Lower,			//������
		EPS_Blend,			//��������
		EPS_UpperAndStand,	//��������վ����������ʱֻ�Ქ��������
	};

	struct tagTrackInfo		//����Ķ�����Ϣ
	{
		EAniPart	aniPart;
		bool		bIsStand;
		tstring		trackName;
		bool		bLoop;
		float		curTime;
		float		fadeInTime;
		EPriority	pri;
	};

public:
	AvatarAniMixer(void);
	~AvatarAniMixer(void);
	void SetPlayer(Player* pPlayer) {m_pPlayer=pPlayer;}
	Player* GetPlayer(void) const	{return m_pPlayer;}

	/** ����һ������
	*/
	void PlayTrack(EAniPart aniPart,bool bIsStand,const TCHAR* szTrackName,bool bLoop,bool bForceReplay=false,float curTime=0.0f,float fadeInTime=0.2f,EPriority pri=EPri_Normal);
	/** ������ͬ����������
	*/
	void Sync2Lower();
	/** �����Ƿ񲥷���ϣ�
		\return �������ڻ���򲥷�ʱ����false����������true
	*/
	bool IsTrackEnd(const TCHAR* szTrackName);
	/** ÿ֡����
	*/
	void Update();

private:
	Player*					m_pPlayer;
	EPlayState				m_state;

	tstring					m_szAllTrack;
	tstring					m_szUpperTrack;
	tstring					m_szLowerTrack;

	EPriority				m_priAll;
	EPriority				m_priUpper;
	EPriority				m_priLower;

	bool					m_bLowerIsStand;

	pair<bool,tagTrackInfo> m_cacheTrack;
};
