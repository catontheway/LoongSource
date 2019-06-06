#pragma once
#include "AvatarAniMixer.h"

class Player;
/** \class AvatarAniPlayer
	\brief ��ҽ�ɫ����������
	\remarks ���ݶ���״̬���Ŷ����������״̬���ܶ������ǲ����ܣ����ǲ�����˶���
*/
class AvatarAniPlayer
{
public:
	enum EAniType			//��������
	{
		EAT_Stand,			//վ��
		EAT_Move,			//�ƶ�
		EAT_StartJump,		//����
		EAT_Jumpping,		//��Ծ��
		EAT_Land,			//���
		EAT_Drop,			//����
		EAT_VDrop,			//��ֱ����
		EAT_Slide,			//����
		EAT_Ready,			//��ս
		EAT_Ease,			//����
		EAT_Other,			//����
	};

public:
	AvatarAniPlayer(void);
	~AvatarAniPlayer(void);
	void SetPlayer(Player* pPlayer);

	/** ����һ������
	*/
	void PlayTrack(EAniType aniType,AvatarAniMixer::EAniPart aniPart,const TCHAR* szTrackName,bool bLoop,bool bForceReplay=false,float curTime=0.0f,float fadeInTime=0.2f,AvatarAniMixer::EPriority pri=AvatarAniMixer::EPri_Normal);
	/** ������ͬ����������
	*/
	void Sync2Lower();
	/** �����Ƿ񲥷���ϣ�
	*/
	bool IsTrackEnd(EAniType aniType,const TCHAR* szTrackName);
	/** ÿ֡����
	*/
	void Update();

private:
	AvatarAniMixer	m_aniMixer;
};
