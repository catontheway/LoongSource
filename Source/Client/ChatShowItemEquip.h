#pragma once
#include "GameClientDefine.h"
#include "..\WorldDefine\container_define.h"
struct tagItem;
struct tagEquip;

class ChatShowItemEquip :
	public GameFrame
{
public:
	ChatShowItemEquip(void);
	~ChatShowItemEquip(void);

	static ChatShowItemEquip* Inst();

	//--���
	bool AddItemData(const TCHAR* szKey, DWORD data);
	bool AddEquipData(const TCHAR* szKey, const tagEquip& data);

	//ɾ��
	void DelItemData(const TCHAR* szKey);
	void DelEquipData(const TCHAR* szKey);

	//--����
	DWORD FindItemData(const TCHAR* szKey);
	const tagEquip* FindEquipData(const TCHAR* szKey);

	//�϶�����ת������������
	EItemConType DragType2ContainerType(EDragType eType);

	//����Ʒ�ʻ����ɫ
	tstring GetColorByQuality(BYTE byQuality);

	//--ɾ���������Ʒ��װ����ʼʱ��
	void DelItemTime(const TCHAR* szKey);
	void DelEquipTime(const TCHAR* szKey);

	//--
	DWORD GetCurrentShowItemIndex() { return m_dwCurShowItemIndex++; }
	DWORD GetCurrentShowEquipIndex() { return m_dwCurShowEquipIndex++; }

private:
	map<tstring, DWORD>				m_mapItem;
	map<tstring, tagEquip>			m_mapEquip;
	map<tstring, float>				m_mapItemTime;
	map<tstring, float>				m_mapEquipTime;

	DWORD							m_dwItemIndex;
	DWORD							m_dwEquipIndex;

	DWORD							m_dwCurShowItemIndex;
	DWORD							m_dwCurShowEquipIndex;
};