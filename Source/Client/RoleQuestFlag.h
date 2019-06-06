#pragma once

enum EQuestFlag
{
	EQF_Null,
	EQF_Accept,					//�ɽ���
	EQF_Finish,					//���
	EQF_InAcceptability,		//���ɽ���
	EQF_NotFinish,				//û���
};


class RoleQuestFlag
{
public:
	RoleQuestFlag(void);
	~RoleQuestFlag(void);

	void Create();
	void Destroy();

	void Update();
	void Draw(POINT pt,float z,float scale,EQuestFlag qf);

private:
	int						m_curFrame;
	float					m_frameTime; 

	vector<ResTexture*>		m_picArray0;
	vector<ResTexture*>		m_picArray1;
};
