/********************************************************************
	created:	2008/12/03
	created:	3:12:2008   11:09
	filename: 	d:\loong_client\Client\AudioSys.h
	file path:	d:\loong_client\Client
	file base:	AudioSys
	file ext:	h
	author:		leili
	
	purpose:	����ϵͳ��������������Ϸ�в��ű������ּ�����������Ч
*********************************************************************/
#pragma once

#include "MapEvent.h"
#include "CombatEvent.h"

class LocalPlayer;
class ClientCamera;


#define SOUND_LOUD		(1.0f)	// ������
#define SOUND_NORMAL	(0.7f)	// ������
#define SOUND_WISPER	(0.4f)	// ����С

class AudioSys
{
public:
	AudioSys(void);
	~AudioSys(void);

	BOOL				Init(void);
	VOID				Destroy(void);

	VOID				Update(DWORD delta);
	VOID				SetCamera(ClientCamera& camera);

	/**	����2D��Ч
	*/
	VOID				Play2DSound(LPCTSTR szSound, float volume=1.0f);

	/**	ֹͣ��������2D��Ч
	*/
	VOID				StopAll2DSound(void);

	// ��������
	VOID				PlayMusic(LPCTSTR szMusic, BOOL bLoop, FLOAT fVolume);
	// ֹͣ����
	VOID				StopMusic();
	BOOL				MusicIsPlaying(void);

	/**	����3D��Ч
	@remarks
		��Ч�������������û�����
		��Ч��minDist��maxDist֮��������Ա仯
		С��minDist��һֱ���������
		����maxDist��һֱ����С����
	*/
	INT					Create3DSound(LPCTSTR szSound,bool bLoop,float minDist,float maxDist,float volume=1.0f);

	/**	����3D��Ч
	*/
	VOID				Destroy3DSound(INT id);

	/**	����3D��Ч
	*/
	VOID				Play3DSound(INT id);

	/**	ֹͣ����3D��Ч
	*/
	VOID				Stop3DSound(INT id);

	/**	����3D��Ч����
	@param pos ��Ч��λ��
	@param vel ��Ч�ƶ����ٶ�
	*/
	VOID				Set3DSoundAtt(INT id,const Vector3& pos,const Vector3& vel);

	/**	ֹͣ��������3D��Ч
	*/
	VOID				StopAll3DSound(void);

	/** ����3D��Ч������
	@remarks
		��Ч������������AudioSys�Լ�����
	*/
	VOID				Play3DSound(LPCTSTR szSound,float minDist,float maxDist,const Vector3& pos,float volume=1.0f);

	VOID				EnableCombatMusic(BOOL bEnable) { m_enableCombatMusic = bEnable; }

private:
	DWORD				EnterMapArea(tagEnterMapAreaEvent* pEvent);	// ����ĳ����
	DWORD				LeaveMapArea(tagLeaveMapAreaEvent* pEvent);	// �뿪ĳ����
	DWORD				EnterCombat(tagEnterCombatEvent* pEvent);	// ����ս��
	DWORD				LeaveCombat(tagLeaveCombatEvent* pEvent);	// �뿪ս��

	DWORD				OnOpenMap(tagGameEvent* pEvent);	// ����3D������Ч
	DWORD				OnCloseMap(tagGameEvent* pEvent);	// �������ű�����Ч
	DWORD				OnCloseMapMusic(tagGameEvent* pEvent);	// �������ű�������

	VOID				GarbageCollection(void);
	VOID				Release3DSound(void);

private:
	TSFPTrunk<AudioSys>		m_Trunk;
	TObjRef<VarContainer>	m_pVarContainer;
	Util					*m_pUtil;

	static const int		s_fadeinTime	= 0;	// ����ʱ��
	static const int		s_fadeoutTime	= 2000;	// ����ʱ��
	static const int		s_musicCount	= 3;

	struct tagSound 
	{
		DWORD	name;	// ��ЧCRC
		INT		id;		// ��ЧID
	};
	typedef std::list<tagSound> SoundList;
	SoundList			m_playSounds;		// ��¼��ǰ���ڲ��ŵ���Ч
	SoundList			m_deadSounds;		// ��¼�Ѿ�������ϵ���Ч

	const tagMapAreaEx*	m_playArea;
	int					m_playMusicIndex;
	DWORD				m_musicIntervalEscaped;

	enum EEnvType
	{
		ENV_Normal	= 0,
		ENV_Combat
	};
	EEnvType			m_envType;			//��ǰ��������

	typedef std::vector<tstring> CombatEnvMusics;
	CombatEnvMusics		m_combatEnvMusic;

	bool				m_bFirstCombat;
	BOOL				m_enableCombatMusic;
};