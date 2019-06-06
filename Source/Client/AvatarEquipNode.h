#pragma once

#include "..\Cool3D\SceneGraph\AvatarNode.h"
#include "..\WorldDefine\AvatarAtt.h"
#include "..\WorldDefine\ItemDefine.h"

class AvatarEquipElement;

/** \class AvatarEquipNode���̳�AvatarNode
    \brief �ṩ�˽�ɫ�������Լ�װ���Ĺ�����
*/
class AvatarEquipNode : public AvatarNode
{
public:
	AvatarEquipNode(void);
	virtual ~AvatarEquipNode(void);

	/** \AvatarNode
	*/
	virtual bool IsInFrustum(const Frustum *pFrumstum) const;
	virtual void Update(float deltaTime,SceneGraph *pSG);
	virtual void ForceUpdate(float deltaTime,SceneGraph *pSG);

	/** \����Ĭ��Avatar
	*/
	void LoadDefault();
	/** \����ָ����Avatar
	*/
	void LoadAvatar( const tagAvatarAtt& att, const tagAvatarEquip& ep );
	/**	\����ȫ��Att��û���޸ĵĲ����򲻸���
	*/
	void UpdateAllAtt( const tagAvatarAtt& att );
	/**	\����ȫ��Equip��û���޸ĵĲ����򲻸���
	*/
	void UpdateAllEquip( const tagAvatarEquip& ep );
	/** \�趨����װ��
	*/
	void SetEquip( const EAvatarElement eAvaEnt, const tagEquipDisplay& ed );
	/** \��õ���װ������
	*/
	const tagEquipDisplay& GetEquip( const EAvatarElement eAvaEnt ) const;
	/** \��õ���װ������ģ��ָ��
	*/
	const SceneNode* GetElementMainMdl( const EAvatarElement eAvaEnt ) const;
	/** \�ó�/�ջ�����
	*/
	void HandUpWeapon( const bool bHandUp );
	/** \����/��ʾ����
	*/
	void HideWeapon( const bool bHide );

	/** \����/��ʾ��������ģ��
	*/
	void HideWeaponPertain(bool bHide);


	/** \��ȡ������������
	*/
	EItemTypeEx GetRWeaponType() const;
	/** \��ȡ������������
	*/
	EItemTypeEx GetLWeaponType() const;
	/** \չ��/�ر�����װ��(���ű��ζ���)
	*/
	void OpenAllEquip( const bool bOpen );
	/** \����װ���Ƿ��Ѵ�
	*/
	bool IsAllEquipOpened();
	/** \����װ���Ƿ��ѹر�
	*/
	bool IsAllEquipClosed();
	/** \����/ֹͣ���⣨����������
	*/
	void PlayBladeFlare( const bool bPlay );
	
	/** \����/ֹͣ��װ��Ч
	*/
	void PlaySuitEffect(bool bPlay);


	/** \����������Ч
	*/
	void UpdataLongHunEffect( const EAvatarElement eAvaEnt, const BYTE byEquipEffect );
	/** \������װ��Ч
	*/
	void UpdataSuitEffect( const DWORD dwSuitID );
	/** \��ý�ɫ�������
	*/
	const tagAvatarAtt& GetAtt() const { return m_Att; }
	/** \�����װID
	*/
	const DWORD GetSuitID() const { return m_dwSuitID; }
	/** \������Դ�Ƿ񶼼������
	*/
	bool IsAllResourceLoaded() const;
	/** \��ʾ����
	*/
	const tagDisplaySet& GetDisplaySet() const { return m_displaySet; }
	void SetDisplaySet( const tagDisplaySet& ds, const bool bHead = true );
private:
	/** \����ص�
	*/
	void CheckOverlap();
	/** \����ͷ�Ƿ�����
	*/
	void ReloadHead( const bool bHide, const bool bFashion );
private:
	tagAvatarAtt			m_Att;										// ��ɫ������ʱ��ָ���������������
	AvatarEquipElement*		m_pEquipElement[X_AVATAR_ELEMENT_NUM];		// ��ɫװ��
	AvatarEquipElement*		m_pLoadingElement[X_AVATAR_ELEMENT_NUM];	// ���ڼ��ص�װ��
	DWORD					m_dwSuitID;									// ��װID
	bool					m_bWeaponHandUp;							// �����Ƿ����ó�
	bool                    m_bWeaponHide;								// �����Ƿ�����
	bool					m_bWeaPonPertainHide;						// ��������ģ���Ƿ�����
	bool					m_bPlaySuitEffect;							// �Ƿ񲥷���װ��Ч
	tagDisplaySet			m_displaySet;								// �����ʾ����
	tagEquipDisplay			m_edHead;									// �����أ����Ա���һ��
	SGAttachableEffectNode* m_pSuitEffect;                              // ��װ��Ч
	bool                    m_bNeedCull;
	DECL_RTTI(AvatarEquipNode);
};
