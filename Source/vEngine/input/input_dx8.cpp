//-----------------------------------------------------------------------------
// File: input
// Desc: interface for directx input
// Auth: Lyp
// Date: 2008-02-21
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "Input_dx8.h"

// ʹ����������
#include "..\variable\var_container.h"
#pragma comment(lib, "dinput8.lib")

namespace vEngine {
//-----------------------------------------------------------------------------
// Construction/Destruction
//-----------------------------------------------------------------------------
InputDX8::InputDX8()
{
	ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));
	ZeroMemory(m_OldKeyBuffer, sizeof(m_OldKeyBuffer));
	m_InputObj = NULL;
	m_lpInputKeyboard = NULL;
	m_lpInputMouse = NULL;
}


InputDX8::~InputDX8()
{
	Destroy();
}


//-----------------------------------------------------------------------------
// init 
//-----------------------------------------------------------------------------
BOOL InputDX8::Init()
{
	//--����DirectInput8 ����
	if( FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&m_InputObj, NULL)) )
		return FALSE;

	//--���������豸����
	if( FAILED(m_InputObj->CreateDevice(GUID_SysKeyboard, &m_lpInputKeyboard, NULL)) )
		return FALSE;

	//--�������ݸ�ʽ
	if( FAILED(m_lpInputKeyboard->SetDataFormat(&c_dfDIKeyboard)) )
		return FALSE;

	//--����Э������
	HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
	if( FAILED(m_lpInputKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND)) )
		return FALSE;

	m_lpInputKeyboard->Acquire();

	// ���
	DIDEVCAPS MouseCapabilities;
	if( FAILED(m_InputObj->CreateDevice(GUID_SysMouse, &m_lpInputMouse, NULL)) )
		return FALSE;

	if(FAILED(m_lpInputMouse->SetDataFormat(&c_dfDIMouse2)))
		return FALSE;
	
	//--Ĭ�ϲ��÷Ƕ�ռ��ǰ��ģʽ
	if( FAILED(m_lpInputMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND|DISCL_NOWINKEY)) )
		return FALSE;

	m_lpInputMouse->Acquire();

	//--�������豸��Caps �Ƿ��л���,����Button ;����Ƿ��Ѿ���������������
	MouseCapabilities.dwSize = sizeof(MouseCapabilities);
	m_lpInputMouse->GetCapabilities(&MouseCapabilities);
	if( !(MouseCapabilities.dwFlags & DIDC_ATTACHED) )
		return FALSE;

	m_dwAxes = MouseCapabilities.dwAxes;
	m_nButtons = (INT)MouseCapabilities.dwButtons;
	//--�������Ƿ�������ʧȥ����
	if( m_lpInputMouse->GetDeviceState(sizeof(m_MouseState),(LPVOID)&m_MouseState) == DIERR_INPUTLOST )
		m_lpInputMouse->Acquire();

	SetAttributes(FALSE, 1.0f);
	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID InputDX8::Destroy()
{
	if(m_InputObj)
	{
		if(m_lpInputKeyboard)
		{
			m_lpInputKeyboard->Unacquire();
			m_lpInputKeyboard->Release();
			m_lpInputKeyboard = NULL;
		}
		if(m_lpInputMouse)
		{
			m_lpInputMouse->Unacquire();
			m_lpInputMouse->Release();
			m_lpInputMouse = NULL;
		}
		m_InputObj->Release();
	}
	m_InputObj = NULL;
}


//-----------------------------------------------------------------------------
// SetAttributes
//-----------------------------------------------------------------------------
VOID InputDX8::SetAttributes(BOOL bInvert, FLOAT fNewSensitivity)
{
	m_bInverted = bInvert;
	m_fSensitivity = fNewSensitivity;
}


//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
VOID InputDX8::Update()
{
	// ����
	memcpy(m_OldKeyBuffer, m_KeyBuffer, sizeof(m_KeyBuffer));
	ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));

	//--Ĭ�ϲ����������ݷ�ʽȡ�ü�������
	if( FAILED(m_lpInputKeyboard->GetDeviceState(sizeof(m_KeyBuffer), (LPVOID)&m_KeyBuffer)) )
		while( DIERR_INPUTLOST == m_lpInputKeyboard->Acquire() );

	// ���
	m_OldMouseState = m_MouseState;
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	if( FAILED(m_lpInputMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState)) )
		while( DIERR_INPUTLOST == m_lpInputMouse->Acquire() );
}






} // namespace vEngine {
