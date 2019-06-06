#pragma once

enum EMiniMapElement
{
	EMME_LocalPlayer = 0,		// �������
	EMME_NatuRes,				// �ɼ���
	EMME_Nest,					// ���ﳲѨ
	EMME_NormalMonster,         // ����
	EMME_Boss,					// BOSS
	EMME_NormalNPC,				// NPC
	EMME_AcceptQuestNPC,         // �ɽ�����NPC
	EMME_CompleteQuestNPC,      // �ɽ�����NPC
	EMME_Teammate,				// ����
	EMME_OtherPlayer,			// �������
	EMME_Num
};

class Role;

/** \class   MiniMapStatic
    \brief   С��ͼ�ؼ�
    \remarks ��ͼ�б���ǰ��ͼ, ColorTexΪ��ͼ��AlphaTexΪAlphaͼ
             ��ʹ��Alphaͼ���ͼ��ϰ�Բ�ε�ͼ��Ⱦ��RenderTarget��
			 Ȼ����ʹ��RenderTarget��ǰ��ͼ��ϵõ�����С��ͼ�ؼ�
			 ͼ����Ϊֱ�Ӱ�ǰ��ͼ��Ⱦ��RenderTarget�ϵĻ������
			 Alphaͨ�����ǵ����󣩡�
*/

class MiniMapStatic : public GUIStatic
{
public:
	struct tagMapData
	{
		Vector2 vWorldPos;
		Vector2 vWorldSize;
		tagMapData() : vWorldPos( 0.0f, 0.0f ), vWorldSize( 0.0f, 0.0f ) {}
	};
	MiniMapStatic();
	virtual ~MiniMapStatic();
	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SetInvisible(BOOL b);

	virtual tagPoint PointToMap( tagPoint ptPointView );
	virtual tagPoint UISizeToMap( tagPoint ptUI );

	BOOL   SetAlphaTex( const tstring strFilename );
	BOOL   SetColorTex( const tstring strFilename );
	VOID   SetMapRect( const tagRect& rc ); 
	BOOL   SetElementImg( const INT nIndex, tstring strFilename );
	VOID   SetElementShow( const INT nIndex, BOOL bShow );
	BOOL   GetElementShow( const INT nIndex ) const;
	VOID   SetZoom( FLOAT fZoom );
	FLOAT  GetZoom() const { return m_fZoom; }

	// ��ͼ����
	VOID   ResetMapData( const tagMapData& data ) { m_MapData = data; }

	// Ѱ·Ŀ�ĵ���
	BOOL	SetDestMarkImg( const tstring& strFilename );
	VOID    ShowDestMark( DWORD dwMapID, const Vector2& vDest );
	VOID    CloseDestMark();

	// ��������Զ��ƶ�����ͼ��ָ����
	VOID            LPAutoMoveTo( const Vector3& vDest, float fValidDist );
	VOID			LPAutoMoveTo( const Vector2& vDest, float fValidDist );
protected:
	tagGUIImage*   m_pColorTex;
	tagGUIImage*   m_pElementImg[EMME_Num];
	BOOL           m_bElementShow[EMME_Num];
private:
	tagPoint       m_ptCenterClient;       // ��ͼ�ؼ�Բ�������Բ��
	INT            m_nRadius;              // ��ͼ�ؼ�Բ������İ뾶
	tagPoint       m_ptMapCenter;          // ��ǰ��ʾ��ͼͼƬ�����ĵ�λ��
	tagGUIImage*   m_pAlphaTex;
	FLOAT          m_fZoom;
protected:
	tagMapData      m_MapData;
	tagGUIImage*	m_pImgDestMark;			// Ѱ·Ŀ�ĵ�ͼ��
	BOOL            m_bShowDestMark;
	DWORD           m_dwDestMarkColor;
	Vector2			m_vDestMark;
	DWORD           m_dwDestMarkMapID;
protected:
	// �����������ͼ�����ת��
	inline tagPoint WorldToMap( const Vector2& vPoint );
	inline Vector2  MapToWorld( const tagPoint& ptPoint );
	EMiniMapElement GetElementType( const Role* pRole );
	// ����Ƿ�ָ��ĳ����ʾ���
	BOOL            IsMousePoint( const INT nIndex, const Vector2& vPosWorld, tagPoint& ptMouse );
private:
	// �������ת��Ϊ��ͼ����
	VOID            DrawLocalPlayer();
	VOID            DrawElement( const INT nIndex, const tagPoint& ptPos, FLOAT fRotate = 0.0f );
	BOOL			PointInMap( tagPoint& ptView );
	VOID			DrawImg( const tagGUIImage* pImg, const tagPoint& ptPos, DWORD dwMulColor = 0xFFFFFFFF, FLOAT fRotate = 0.0f );
};
