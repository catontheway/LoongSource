#pragma once

/**
	class: AvatarCfg
	brief: ���õ���ģʽ���ɳ�����ָ��ģ�͡�������ͼ��·��
*/
#define AVATAR_HAIR_TYPE 5
#define AVATAR_HAIR_COLOR 6
#define AVATAR_FACE 5
#define AVATAR_FACE_DETAIL 7
#define AVATAR_DRESS 3           // ��ѡ����ʱװ��ʽ����Ŀ

class AvatarCfg
{
public:
	AvatarCfg(void);
	~AvatarCfg(void);
	static AvatarCfg* Inst();

	int GetNumHairType()const	{ return AVATAR_HAIR_TYPE; }
	int GetNumHairColor()const	{ return AVATAR_HAIR_COLOR; }
	int GetNumFace()const		{ return AVATAR_FACE; }
	int GetNumFaceDetail()const	{ return AVATAR_FACE_DETAIL; }
	int GetNumDress()const		{ return AVATAR_DRESS; }

	const TCHAR* GetHairTypeMdlPath(BYTE bySex, WORD id);						//ͷ��ģ��·��
	const TCHAR* GetHairColorTexPath(BYTE bySex, WORD hairID, WORD colorID);	//ͷ��������ͼ·��
	const TCHAR* GetFaceMdlPath(BYTE bySex, WORD id);							//�沿ģ��·��
	const TCHAR* GetFaceDtlTexPath(BYTE bySex, WORD id);						//�沿������ͼ·��
	/** \��ȡ����ģ�͵�·��
	*/
	const TCHAR* GetUpperUWMdlPath(BYTE bySex,BOOL bFashion);					//����ģ��·��
	const TCHAR* GetLowerUWMdlPath(BYTE bySex,BOOL bFashion);					//����ģ��·��
	const TCHAR* GetBootUWMdlPath(BYTE bySex,BOOL bFashion);					//Ьģ��·��	
	const TCHAR* GetGloveUWMdlPath(BYTE bySex,BOOL bRight,BOOL bFashion);		//������··��(������)

	/** \�����Ա��������һ������ʱװ�ṹ
	*/
	void MakeNewPlayerFahion( const BYTE bySex, const int n, tagAvatarEquip& out ) const;
private:
	tstring							m_strPath;
};
