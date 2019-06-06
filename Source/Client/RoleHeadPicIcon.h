#pragma once

#include "ActiveStatic.h"

class Role;
class AvatarEquipNode;

/** \class		RoleHeadPicIcon
    \brief		��ʾһ����ɫͷ��
	\remarks	��Ҷ�̬���գ�NPC�Ӵ��̼���(��Cool3DView�ֶ�����)������ɫΪm_dwPicColor
*/
class RoleHeadPicIcon : public ActiveStatic
{
public:
	RoleHeadPicIcon();
	virtual ~RoleHeadPicIcon();

	// ActiveStatic
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Render();
	virtual VOID SetInvisible(BOOL b);

	/** \����ͷ��,ע�⣬�����ڿؼ���Render()�е��ô˺���
	*/
	VOID UpdataHeadPic( Role* pRole );
	VOID UpdataAvatarHeadPic( const tagAvatarAtt& att, const tagAvatarEquip& ep, const tagDisplaySet* const pDisplaySet );
	VOID UpdataAvatarHeadPic( AvatarEquipNode* pAvatarNode );
	VOID UpdataDisplaySet( const tagDisplaySet& ds );
	/** \��Ҫ�ⲿ����
	*/
	VOID Render3D();
	/** \�豸��ʧ����
	*/
	VOID OnDeviceResetEvent(tagGameEvent* );
private:
	BOOL UpdataCamera();
	VOID FreeDynamicData();							// �ͷ����ж�̬��������Դ
	VOID NewPlayerData();							// �������ͷ����Դ
	VOID NewNPCData();								// ����NPCͷ����Դ

	tstring FileNameMdlToHeadPic( tstring strMdlFileName, BOOL bFSKEL = FALSE );
private:
	tagGUIImage*            m_pHeadImg;				 // ͷ��ͼƬ(���:RenderTarget,Npc:Restexture)
	NullSceneGraph*			m_pSG;					 // ���ڱ�������������յ���Ϣ,��������Ⱦ
	CameraBase*				m_pCamera;				 // ���ڸ��������
	AvatarEquipNode*		m_pAvatarNode;			 // ���ڸ��������
	tstring                 m_strNpcHeadPicFileName; // NPCͷ����ļ���
	BOOL                    m_bRefershAvatar;		 // �Ƿ���Ҫˢ�����ͷ��(��ɫ����,������һ�װ)
	BOOL                    m_bRefershNPC;			 // �Ƿ���Ҫˢ��NPCͷ��(��ɫ����)
	Vector3                 m_vCamDir;				 // ��������յ����������
	Vector3                 m_vCamLooatOffset;		 // ��������յ��ӵ�ƫ��ϵ��(���ͷ��Box)
	float                   m_fCamDist;				 // ��������յ��Ӿ�ϵ��(���ͷ��Box�Խǳ���)
	float                   m_fRenderTargetZoom;	 // ���������ʱʹ��RenderTarget��Կؼ���С�ķŴ�ϵ��(���ڼ��پ��)
	DWORD                   m_dwBackColor;           // ������ɫ

	TSFPTrunk<RoleHeadPicIcon> m_Trunk;
};
