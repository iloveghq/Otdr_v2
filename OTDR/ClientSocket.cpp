// ClientSocket.cpp: implementation of the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "GenClient.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientSocket::CClientSocket(CWnd * parent) : CCESocket()
{
	m_Parent = parent;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnReceive()
{
	if(m_Parent)
		::PostMessage(m_Parent->m_hWnd, SOCK_ONRECEIVE, NULL, NULL);
}


void CClientSocket::OnClose(int closeEvent)
{
	if(m_Parent)
		::PostMessage(m_Parent->m_hWnd, SOCK_ONCLOSE, NULL, NULL);
}