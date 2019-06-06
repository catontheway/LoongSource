//-----------------------------------------------------------------------------
//!\file input.h
//!\author Lyp
//!
//!\date 2003-11-13
//! last 2008-02-21
//!
//!\brief interface for keyboard or mouse
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {
CONST INT KEY_BUFFER_SIZE = 256;
//-----------------------------------------------------------------------------
// input interface
//-----------------------------------------------------------------------------
class VENGINE_API InputDX8
{
public:
	// ����DirectInput8 ����, ������Ĭ�ϵ� Keyboard , Mouse �豸
	BOOL Init();
	VOID Destroy();
	VOID Update();
	VOID SetAttributes(BOOL bInvert, FLOAT fNewSensitivity);

	__forceinline BOOL IsKeyDown(DIK key);		// ����ʱ����ΪTRUE
	__forceinline BOOL IsKeyDownOnce(DIK key);	// ����ʱ˲��ΪTRUE
	__forceinline BOOL IsKeyUpOnce(DIK key);		// ����ʱ˲��ΪTRUE
	__forceinline BOOL GetKeyDown(std::vector<DIK>& vecKeys);	// �õ���ǰ�尴�µ�key,Ч�ʽϵ�
	__forceinline BOOL IsMouseDown(INT nIndex);		// ����ʱ����ΪTRUE,�����0,�Ҽ���1
	__forceinline BOOL IsMouseDownOnce(INT nIndex);	// ����ʱ˲��ΪTRUE,�����0,�Ҽ���1
	__forceinline BOOL IsMouseUpOnce(INT nIndex);		// ����ʱ˲��ΪTRUE,�����0,�Ҽ���1
	__forceinline BOOL IsMouseMove();
	

	// ȡ�����ı仯��
	__forceinline FLOAT GetRelativeX();
	__forceinline FLOAT GetRelativeY();
	__forceinline FLOAT GetRelativeZ();
	
	InputDX8();
	~InputDX8();

private:
	TObjRef<Util>			m_pUtil;

	LPDIRECTINPUT8			m_InputObj;
	LPDIRECTINPUTDEVICE8	m_lpInputKeyboard; 
	LPDIRECTINPUTDEVICE8	m_lpInputMouse; 
	DIMOUSESTATE2			m_MouseState;
	DIMOUSESTATE2			m_OldMouseState;
	BOOL					m_bInverted; 
	FLOAT					m_fSensitivity; 

	DWORD					m_dwAxes;
	INT						m_nButtons;
	CHAR					m_KeyBuffer[KEY_BUFFER_SIZE];
	CHAR					m_OldKeyBuffer[KEY_BUFFER_SIZE];
};

//-----------------------------------------------------------------------------
// IsKeyDown
//-----------------------------------------------------------------------------
BOOL InputDX8::IsKeyDown(DIK key)
{
	ASSERT(key>=0 && key<KEY_BUFFER_SIZE);
	return (m_KeyBuffer[key] & 0x80);
}


//-----------------------------------------------------------------------------
// IsKeyDownOnce
//-----------------------------------------------------------------------------
BOOL InputDX8::IsKeyDownOnce(DIK key)
{
	ASSERT(key>=0 && key<KEY_BUFFER_SIZE);
	return !(m_OldKeyBuffer[key] & 0x80) && (m_KeyBuffer[key] & 0x80);
}



//-----------------------------------------------------------------------------
// IsKeyUpOnce
//-----------------------------------------------------------------------------
BOOL InputDX8::IsKeyUpOnce(DIK key)
{
	ASSERT(key>=0 && key<KEY_BUFFER_SIZE);
	return (m_OldKeyBuffer[key] & 0x80) && (!(m_KeyBuffer[key] & 0x80));
}


//-----------------------------------------------------------------------------
// GetCurrentKey
//-----------------------------------------------------------------------------
BOOL InputDX8::GetKeyDown( std::vector<DIK>& vecKeys )
{
	BOOL bResult = FALSE;
	for(INT n=0; n<KEY_BUFFER_SIZE; n++)
	{
		if( m_KeyBuffer[n] & 0x80 )
		{
			vecKeys.push_back((DIK)n);
			bResult = TRUE;
		}
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// IsMouseDown
//-----------------------------------------------------------------------------
BOOL InputDX8::IsMouseDown(INT nIndex)
{
	ASSERT(nIndex<8);
	if( nIndex >= m_nButtons )
		return FALSE;

	return m_MouseState.rgbButtons[nIndex] & 0x80;
}


//-----------------------------------------------------------------------------
// IsMouseDownOnce
//-----------------------------------------------------------------------------
BOOL InputDX8::IsMouseDownOnce(INT nIndex)
{
	ASSERT(nIndex<8);
	if( nIndex >= m_nButtons )
		return FALSE;

	return ( !(m_OldMouseState.rgbButtons[nIndex] & 0x80) 
		&& (m_MouseState.rgbButtons[nIndex] & 0x80) );
}


//-----------------------------------------------------------------------------
// IsMouseUpOnce
//-----------------------------------------------------------------------------
BOOL InputDX8::IsMouseUpOnce(INT nIndex)
{
	ASSERT(nIndex<8);
	if( nIndex >= m_nButtons )
		return FALSE;

	return ( (m_OldMouseState.rgbButtons[nIndex] & 0x80) 
		&& !(m_MouseState.rgbButtons[nIndex] & 0x80) );
}


//-----------------------------------------------------------------------------
// IsMouseMove
//-----------------------------------------------------------------------------
BOOL InputDX8::IsMouseMove()
{
	if(GetRelativeX() != 0.0f || GetRelativeY() != 0.0f || GetRelativeZ() != 0.0f)
		return TRUE;

	return FALSE;
}


//-----------------------------------------------------------------------------
// GetRelativeX
//-----------------------------------------------------------------------------
FLOAT InputDX8::GetRelativeX()
{
	return (FLOAT)m_MouseState.lX * m_fSensitivity;
}


//-----------------------------------------------------------------------------
// GetRelativeY
//-----------------------------------------------------------------------------
FLOAT InputDX8::GetRelativeY()
{
	return (FLOAT)m_MouseState.lY * m_fSensitivity;
}


//-----------------------------------------------------------------------------
// GetRelativeZ
//-----------------------------------------------------------------------------
FLOAT InputDX8::GetRelativeZ()
{
	return (FLOAT)m_MouseState.lZ * m_fSensitivity;
}


} // namespace vEngine {
