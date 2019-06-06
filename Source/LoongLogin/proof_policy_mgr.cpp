//-----------------------------------------------------------------------------
//!\file proof_policy_mgr.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief ��֤���Թ�����
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "proof_policy.h"
#include "proof_policy_none.h"
#include "proof_policy_own.h"
#include "proof_policy_xunlei.h"
#include "proof_policy_mgr.h"
#include "proof_policy_orange.h"

//-----------------------------------------------------------------------------
// ����
//-----------------------------------------------------------------------------
ProofPolicyMgr::~ProofPolicyMgr()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// ��ʼ��
//-----------------------------------------------------------------------------
BOOL ProofPolicyMgr::Init()
{
	RegisterAFPolicy();

	return TRUE;
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
VOID ProofPolicyMgr::Destroy()
{
	UnregisterAFPolicy();
}

//-------------------------------------------------------------------------------
// ע����֤������
//-------------------------------------------------------------------------------
VOID ProofPolicyMgr::RegisterAFPolicy()
{
	m_AFPolicy.Register(TWrap<ProofPolicyNone>(),	EPP_Test);
	m_AFPolicy.Register(TWrap<ProofPolicyOwn>(),	EPP_Own);
	m_AFPolicy.Register(TWrap<ProofPolicyXunLei>(),	EPP_XunLei);
	m_AFPolicy.Register(TWrap<ProofPolicyOrange>(),	EPP_Orange);

}

//-------------------------------------------------------------------------------
// ��ע����֤������
//-------------------------------------------------------------------------------
VOID ProofPolicyMgr::UnregisterAFPolicy()
{
	m_AFPolicy.Unregister(TWrap<ProofPolicyNone>(),		EPP_Test);
	m_AFPolicy.Unregister(TWrap<ProofPolicyOwn>(),		EPP_Own);
	m_AFPolicy.Unregister(TWrap<ProofPolicyXunLei>(),	EPP_XunLei);
	m_AFPolicy.Unregister(TWrap<ProofPolicyOrange>(),	EPP_Orange);
}

//-------------------------------------------------------------------------------
// ����һ����֤���Զ���
//-------------------------------------------------------------------------------
ProofPolicy* ProofPolicyMgr::CreateProofPolicy(EProofPolicy ePolicy)
{
	return m_AFPolicy.Create(ePolicy);
}

//-------------------------------------------------------------------------------
// ����һ����֤����
//-------------------------------------------------------------------------------
VOID ProofPolicyMgr::DestroyProofPolicy(EProofPolicy ePolicy, ProofPolicy* pPolicy)
{
	if( !P_VALID(pPolicy) ) return;

	m_AFPolicy.Destroy(pPolicy, ePolicy);
}