// ClientSocket.h: interface for the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKET_H__CF90F241_4C08_4220_A5B5_FC085F552C13__INCLUDED_)
#define AFX_CLIENTSOCKET_H__CF90F241_4C08_4220_A5B5_FC085F552C13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CESocket.h"

class CClientSocket : public CCESocket
{
public:
	CClientSocket(CWnd *parent);
	virtual ~CClientSocket();

	virtual void OnReceive();
	virtual void OnClose(int closeEvent);

protected:

	CWnd *m_Parent;
};

#endif // !defined(AFX_CLIENTSOCKET_H__CF90F241_4C08_4220_A5B5_FC085F552C13__INCLUDED_)
