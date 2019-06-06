#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>	//For DIK define

typedef unsigned short DIK;

/**	\class GInput
	\brief ��Ϸ�����豸
	\remarks 
	\par DInput�豸���� Ĭ�ϲ�����������,ǰ��ģʽ��ֻ�ܲ���һ��ʵ��.
	\par keyֵ��<dinput.h>�е�DIK_XXX����
*/

class GInput
{
public:
	GInput(void);
	virtual ~GInput(void);
	static GInput* Inst(void);

	void UpdateKBInput(void);
	void UpdateMouseInput(void);

	/** ���key���Ƿ񱻰���
	*/
	bool KeyPressed(DIK key);
	bool KeyDown(DIK key);
	bool KeyDownFirst(DIK key);

	/** ����ĸ���������
		\param �����0,�Ҽ���1
	*/
	bool MouseDown(BYTE buttonIndex);
	bool MouseUp(BYTE buttonIndex);
	bool IsMouseMove();
	bool MousePressed(BYTE buttonIndex);
	bool MouseDownFirst(BYTE buttonIndex);

	/** Destroy����DirectInput�豸����
		�����ֶ�����,ȷ������DirectInput���󱻳ɹ�����
	*/
    void Destroy(void);
	
	void SetAttributes(bool bInvert, float fNewSensitivity);

	POINT GetCursorPos();
	
	/** ȡ����ά�ռ�������x����
	*/
	float GetRelativeX(void);
	/** ȡ����ά�ռ�������y����
	*/
	float GetRelativeY(void);
	/** ȡ����ά�ռ�������z����
	*/
	float GetRelativeZ(void);
	/** ȡ�ö�άƽ�����������x����
	*/
	float GetAbsouluteX(void);
	/** ȡ�ö�άƽ�����������y����
	*/
	float GetAbsouluteY(void);
private:
	friend class GWin32App;
	/** ����DirectInput8 ����, ������Ĭ�ϵ� Keyboard , Mouse �豸
	*/
	bool CreateInput(void);
private:
	class Member;
	Member	*m_p;
};
