//-----------------------------------------------------------------------------
//!\file	main.cpp
//!\brief	��Ϸ��ѭ��
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "server.h"

//----------------------------------------------------------------------------------
// ��ѭ��
//----------------------------------------------------------------------------------
INT main(VOID)
{	
	vEngine::InitNetwork();
	vEngine::InitEngine();

	if( g_Server.Init() )
	{
		g_Server.MainLoop();
	}
	g_Server.Destroy();

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();

	return 0;
}

