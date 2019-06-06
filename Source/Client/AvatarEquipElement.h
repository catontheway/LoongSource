#pragma once

#include "..\WorldDefine\AvatarAtt.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\ItemDefine.h"

class AvatarEquipNode;
struct tagItemDisplayInfo;
struct tagEquipProto; 

/** \class AvatarEquipElement
    \brief ����װ����װ���е�SceneNode������������this����
	\remarks �ⲿ��Ҫ��ɾ��AvatarNode֮ǰɾ����
*/
class AvatarEquipElement
{
public:
	/** \װ����չ��״̬
	*/
	enum EOpenState
	{
		EOS_Closed,
		EOS_Opening,
		EOS_Opened,
		EOS_Closing,
		EOS_Num
	};
	/** \װ��������������
	*/
	enum EPartType
	{
		EPT_Main,					// ��ģ��
		EPT_Pertain,				// ����ģ��
		EPT_Attach,					// ���ر���ģ��
		EPT_BladeFlare,             // ����
		EPT_Glow,					// ����
		EPT_LongHunEffect,			// ������Ч
		EPT_Mend,                   // ���䲿�֣�ʱװģʽװ��£����Ҫ��һ�ڸ첲��
		EPT_Num
	};

	AvatarEquipElement( AvatarEquipNode* pFather, EAvatarElement eElement );
	~AvatarEquipElement();

	/** \����(��Ҫ���ڱ��ζ�������)
	*/
	void Updata();
	/** \����(��Ҫ���ڼ�����Դ)
	*/
	bool ForceUpdate(float deltaTime,SceneGraph *pSG,const tagAvatarAtt& att);
	/** \�ͷ�����ģ�Ͳ����������������
	*/
	void ReloadAtt( const tagAvatarAtt& att, const bool bFashion );
	/** \���¼���װ��
	*/
	void ReloadEquip( const BYTE bySex, const tagEquipDisplay& display, const bool bFashion );
	/** \����������
	*/
	const tagEquipDisplay& GetDisplay() const { return m_equipDisplay; }
	/** \�����ģ��
	*/
	const SceneNode* GetMainMdl() const;
	/** \�ó�/�ջ�(��Ҫ��������)
	*/
	void HandUp( const bool bHandUp );
	/** \չ��/�ջ�(���ű��ζ���)
	*/
	void Open( const bool bOpen );
	/** \���չ��״̬
	*/
	EOpenState GetOpenState() const { return m_eOpenState; }
	/** \����/��ʾ
	    \bRightMdlOnly �Ƿ�ֻ�����ұ�ģ��(�磺���֣����֣�
	    \eType         �������� eType == EPT_Num Ϊȫ��
	*/
	void SetInvisible( const bool bInvisible, const bool bRightMdlOnly = false, const EPartType eType = EPT_Num );
	/** \����/ֹͣ����
	*/
	void PlayBladeFlare( const bool bPlay );
	/** \����������Ч
	*/
	void UpdataLongHunEffect( const BYTE byEquipEffect, const bool bAttachToSG );
	/** \������ͼ
	*/
	void ChangeTexture( SceneNode* pNode, const int nMtlID, const TCHAR* szNewTexPath );
	/** \���÷�ɫ
	*/
	void SetHairColorTex(BYTE bySex, WORD hairID, WORD colorID);
	/** \�����沿ϸ��
	*/
	void SetFaceDetailTex(BYTE bySex, WORD id);
	/** \������Դ�Ƿ񶼼������
	*/
	bool IsResourceReady() const;
	/** \�Ƿ�������
	*/
	bool IsEmpty() const { return m_vecPartData.empty(); }
	/** \���ӵ�����
	*/
	void AttachToSG();
	void AttachPartNode( SceneNode* pMainNode, SceneNode* pPartNode );
	void Destroy();
private:
	/** \װ��������������
	*/
	struct tagPartData
	{
		EPartType  eType;			// ����
		bool       bRight;          // �Ƿ�Ϊ�ұ�ģ��
		SceneNode* pSGNode;         // scene node
		tagPartData( EPartType type, bool bR ) : eType( type ), bRight( bR ), pSGNode( NULL ) {}
	};
private:
	void		  LoadOnePart( EPartType ePartType, const bool bRight, const tstring& strFileName, const EEquipMdlType eMdlType, const tstring& strTagNodeName );
	void		  LoadOneSkin( EPartType ePartType, const bool bRight, const tstring& strFileName );
	void          LoadOneSFX(  EPartType ePartType, const bool bRight, const tstring& strFileName, const tstring& strTagNodeName, const bool bAttchToSG );
	bool          LoadAttachEffect( const EPartType eType, const tstring& strFileName, const bool bAttchToSG );
	bool		  HaveKeyFrame();
	void          PlayKeyFrameTrack( const tstring& strTrackName, bool bLoop );
	bool          IsKeyFrameTrackEnd( const tstring& strTrackName );
	bool		  SetColor( const int nColor );
	const tstring GetMdlExt( const EEquipMdlType eType ) const;
	const tstring GetEquipFileName( const BYTE bySex, const TCHAR* szMdlName, const EEquipMdlType eMdlType ) const;
	const tstring GetAttFileName( const tagAvatarAtt& att, bool bFashion, bool bRight = true ) const;
	const tstring GetGlowName( const TCHAR* szBaseName, const BYTE byVal ) const;
	const tstring GetAttatchEffectBindBoneName( const bool bRight ) const;
private:
	const EAvatarElement   m_eElement;                // AvatarNode�еĲ�λ
	AvatarEquipNode*	   m_pFather;                 // ��AvatarNode
	tagEquipDisplay		   m_equipDisplay;            // ������۽ṹ
	vector<tagPartData>    m_vecPartData;             // �����б�
	EOpenState             m_eOpenState;			  // ��״̬
	tstring				   m_strGlowName;			  // ���ε���Ч���
	tstring				   m_strLongHunEffName;		  // ������Ч����Ч���
	bool                   m_bTexChanged;             // �����޸����
};
