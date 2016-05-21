// This Winsock Wrapper replaces all MFC socket classes (CSocket, CAsyncSocket
// CCeSocket). It offers async notifications, buffered read and easy data access
// functions.
//
// Coded by Marco Zaratti / marco.zaratti@gmail.it / 2006.04.05
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CESocket.h"
#include <stdlib.h>
#include <afxmt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CCESocket::m_bWSAStarted = FALSE;
#ifdef _WCE_SECTION
	CRITICAL_SECTION CCESocket::m_readLock;
#else
	CCriticalSection CCESocket::m_readLock;
#endif

CCESocket::CCESocket()
{
	WSADATA wsaData;

	s = INVALID_SOCKET;
	m_socketType = SOCK_STREAM;
	m_socketState = NONE;
	m_readThread = NULL;
	m_acceptThread = NULL;
	m_readThreadState = CLOSED;
	m_acceptThreadState = CLOSED;
	m_udpReadyToSend = FALSE;
	m_receiveAddrSz = sizeof(SOCKADDR_IN);
	m_eolFormat = EOL_LFCR;
	m_errorCode = 0;
	m_recvBufSize = 0;
#ifdef _WCE_SECTION
	InitializeCriticalSection(&CCESocket::m_readLock);
#endif
	m_availableData = 0;

	if(!m_bWSAStarted)
	{
		m_errorCode = WSAStartup(MAKEWORD(2,2), &wsaData);
		if(m_errorCode == 0)
			m_bWSAStarted = TRUE;
	}
}

CCESocket::~CCESocket()
{
	int watchDog;

//	HANDLE readThread, acceptThread;
// 	readThread = m_readThread;
// 	acceptThread = m_acceptThread;

	Disconnect();

	watchDog = 0;
	while (m_readThread)
	{
		SetThreadPriority(m_readThread, THREAD_PRIORITY_HIGHEST);
		if((::WaitForSingleObject(m_readThread, 1000) != WAIT_TIMEOUT) ||
			watchDog >= THREAD_TERMINATION_MAXWAIT)
		{
			CloseHandle(m_readThread);
			m_readThread = NULL;
		}
		watchDog++;
	}
	watchDog = 0;
	while (m_acceptThread)
	{
		SetThreadPriority(m_acceptThread, THREAD_PRIORITY_HIGHEST);
		if((::WaitForSingleObject(m_acceptThread, 1000) != WAIT_TIMEOUT) ||
			watchDog >= THREAD_TERMINATION_MAXWAIT)
		{
			CloseHandle(m_acceptThread);
			m_acceptThread = NULL;
		}
		watchDog++;
	}

	m_ReadBuffer.ClearData();

#ifdef _WCE_SECTION
	DeleteCriticalSection(&CCESocket::m_readLock);
#endif
}

void CCESocket::SetBufferSize(int bufSize)
{
	if(bufSize > 0)
		m_recvBufSize = bufSize;
}

bool CCESocket::Create(int socketType, int bufferSize)
{
	BOOL dontLinger = TRUE;
	LINGER lingerOpt;

	if(!m_bWSAStarted)
		return FALSE;

	//Exit if the socket has been already created
	if(s != INVALID_SOCKET)
	{
		m_errorCode = WSAEACCES;
		return FALSE;
	}

	switch(socketType)
	{
		case SOCK_STREAM:
			s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
		case SOCK_DGRAM:
			s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			break;
		default:
			m_errorCode = WSAEINVAL;
			return FALSE;
	}
	
	if(s == INVALID_SOCKET)
	{
		m_errorCode = WSAGetLastError();
		return FALSE;
	}

	switch(socketType)
	{
		case SOCK_STREAM:
			lingerOpt.l_onoff = 0;
			lingerOpt.l_linger = 0;

			if(setsockopt(s, SOL_SOCKET, SO_DONTLINGER, (char*) &dontLinger, sizeof(BOOL)) != 0)
			{
				m_errorCode = WSAGetLastError();
				closesocket(s);
				return FALSE;
			}

			if(setsockopt(s, SOL_SOCKET, SO_LINGER, (char*) &lingerOpt, sizeof(LINGER)) != 0)
			{
				m_errorCode = WSAGetLastError();
				closesocket(s);
				return FALSE;
			}

			if(bufferSize == 0)
				bufferSize = TCPBUFFERSIZE;
			break;
		case SOCK_DGRAM:
			if(bufferSize == 0)
				bufferSize = UDPBUFFERSIZE;
			m_udpReadyToSend = FALSE;
			break;
	}
	m_recvBufSize = bufferSize;
	m_ReadBuffer.ClearData();
	m_availableData = 0;

	//OK, all done
	m_socketType = socketType;
	m_socketState = CREATED;
	return TRUE;
}

bool CCESocket::Connect(CString &addr, UINT remotePort)
{
	char hostStr[257] = {""};
	int wHostLen;
	LPTSTR wHost;
	ulong hostByIP;
	HOSTENT *hostByName;

	if(!m_bWSAStarted)
		return FALSE;

	//Make sure the socket was created
	if(s == INVALID_SOCKET)
	{
		m_errorCode = WSAENOTSOCK;
		return FALSE;
	}

	//We cannot connect if the socket is already connected (TCP only)
	if(m_socketType == SOCK_STREAM && m_socketState > CREATED)
	{
		m_errorCode = WSAEISCONN;
		return FALSE;
	}

	//We cannot connect if the socket is listening (UDP only)
	if(m_socketType == SOCK_DGRAM && m_socketState > CONNECTED)
	{
		m_errorCode = WSAEISCONN;
		return FALSE;
	}

	//Check port values
	if(remotePort > 65535)
	{
		m_errorCode = WSAEINVAL;
		return FALSE;
	}

	//Gets address string and convert it from unicode to multibyte
	wHostLen = addr.GetLength();
	if(wHostLen > 256)
		wHostLen = 256;
	wHost = addr.GetBuffer(wHostLen);
#ifdef _WIN32_WCE
	wcstombs(hostStr, wHost, 256);
#else
	wHost[wHostLen] = '\0';
	WideCharToMultiByte(CP_OEMCP, 0, wHost, -1, hostStr, wHostLen, NULL, NULL); 
	addr.ReleaseBuffer();
#endif
	
	//Builds destination address
	memset(&m_remoteAddress, 0, sizeof(SOCKADDR_IN));
	m_remoteAddress.sin_family = AF_INET;
	m_remoteAddress.sin_port = htons(remotePort);

	hostByIP = inet_addr(hostStr);
	if(hostByIP == INADDR_NONE)
	{
		hostByName = gethostbyname(hostStr);
		if(hostByName == NULL)
		{
			m_errorCode = WSAGetLastError();
			return FALSE;
		}
		m_remoteAddress.sin_addr = *((IN_ADDR*)hostByName->h_addr_list[0]);
	}
	else
		m_remoteAddress.sin_addr.s_addr = hostByIP;
	
	//Connects if TCP.
	//UDP is connectionless. However we'll use m_remoteAddress for sending data.
	if(m_socketType == SOCK_STREAM)
	{
		if(connect(s, (SOCKADDR*) &m_remoteAddress, sizeof(SOCKADDR_IN)) != 0)
		{
			m_errorCode = WSAGetLastError();
			return FALSE;
		}
		m_socketState = CONNECTED;

		//TCP: Start reading thread
		//UDP: Can't start it now, we must first do a sendto to bind to a local port
		m_readThread = CreateThread(NULL, 0, StartThread, this, 0, NULL );
	}
	else
	{
		m_udpReadyToSend  = TRUE;
		m_socketState = CONNECTED;
	}

	return TRUE;
}

void CCESocket::Disconnect()
{
	if(!m_bWSAStarted || s == INVALID_SOCKET)
		return;

	//After a shutodown reading thread will self terminate
	m_socketState = DISCONNECTING;
	shutdown(s, SD_BOTH);
	closesocket(s);
	s = INVALID_SOCKET;
	m_socketState = NONE;
	m_errorCode = 0;
	m_udpReadyToSend = FALSE;
}

int CCESocket::Send(CString& str)
{
	int sentBytes, len;
	char* buf;
	LPTSTR strBuf;

	len = str.GetLength();
	strBuf = str.GetBuffer(len);
#ifdef _WIN32_WCE
	buf = new char[len];
	wcstombs(buf, strBuf, len);
#else
	//buf = (char*) strBuf;
	WideCharToMultiByte(CP_OEMCP, 0, str, -1, buf, str.GetLength() * 2 , NULL, NULL); 
#endif

	sentBytes = Send(buf, len);

#ifdef _WIN32_WCE
	delete[] buf;
#endif

	return sentBytes;
}

int CCESocket::SendLine(CString& str)
{
	int sentBytes, len;
	char* buf;
	LPTSTR strBuf;

	len = str.GetLength();
	strBuf = str.GetBuffer(len);
	buf = new char[len+2];
#ifdef _WIN32_WCE
	wcstombs(buf, strBuf, len);
#else
	//memcpy(buf, strBuf, len);
	WideCharToMultiByte(CP_OEMCP, 0, str, -1, buf, str.GetLength() * 2 , NULL, NULL); 
#endif

	switch(m_eolFormat)
	{
		case EOL_LFCR:
			buf[len] = '\r';
			buf[len+1] = '\n';
			sentBytes = Send(buf, len+2);
			break;
		case EOL_CR:
			buf[len] = '\n';
			sentBytes = Send(buf, len+1);
			break;
		case EOL_NULL:
			buf[len] = '\0';
			sentBytes = Send(buf, len+1);
			break;
	}

	delete[] buf;
	return sentBytes;
}

int CCESocket::Send(const char* buf, int len)
{
	int dataPtr = 0;
	int sentBytes = 0;

	if(!m_bWSAStarted)
		return SOCKET_ERROR;

	//We cannot send data if the socket's not connected
	if(m_socketState < CONNECTED)
	{
		m_errorCode = WSAENOTCONN;
		return SOCKET_ERROR;
	}

	//TCP listening socket cannot send data
	if(m_socketState == ACCEPTING && m_socketType == SOCK_STREAM)
	{
		m_errorCode = WSAENOTCONN;
		return SOCKET_ERROR;
	}

	//Have we a valid buffer?
	if(!buf || len <= 0)
	{
		m_errorCode = WSAEFAULT;
		return SOCKET_ERROR;
	}

	switch(m_socketType)
	{
	case SOCK_STREAM:
		while(len > 0)
		{
			sentBytes = send(s, &buf[dataPtr], len, 0);
			if(sentBytes == SOCKET_ERROR)
			{
				m_errorCode = WSAGetLastError();
				return SOCKET_ERROR;
			}
			dataPtr += sentBytes;
			len -= sentBytes;
		}
		break;
	case SOCK_DGRAM:
		if(m_udpReadyToSend)
		{
			while(len > 0)
			{
				if(m_socketState == ACCEPTING)
					sentBytes = sendto(s, &buf[dataPtr], len, 0, (SOCKADDR*) &m_localAddress, sizeof(m_localAddress));
				else
					sentBytes = sendto(s, &buf[dataPtr], len, 0, (SOCKADDR*) &m_remoteAddress, sizeof(m_remoteAddress));
				if(sentBytes == SOCKET_ERROR)
				{
					m_errorCode = WSAGetLastError();
					return SOCKET_ERROR;
				}
				dataPtr += sentBytes;
				len -= sentBytes;
			}

			//In client mode: after the first sendto we can start the reading thread
			//because we now have an implicit bind on a local port
			//In server mode: the thread is already running so this if is never true.
			if(m_readThreadState == CLOSED)
			{
				m_readThreadState = RUNNING;
				m_readThread = CreateThread(NULL, 0, StartThread, this, 0, NULL );
			}
		}
	}

	return dataPtr;
}

bool CCESocket::Accept(UINT localPort, int maxConn)
{
	if(!m_bWSAStarted)
		return FALSE;

	//Make sure the socket was created
	if(s == INVALID_SOCKET)
	{
		m_errorCode = WSAENOTSOCK;
		return FALSE;
	}

	//We cannot connect if the socket is already connected or accepting
	if(m_socketState > CREATED)
	{
		m_errorCode = WSAEISCONN;
		return FALSE;
	}

	//Check port values
	if(localPort > 65535)
	{
		m_errorCode = WSAEINVAL;
		return FALSE;
	}

	memset(&m_localAddress, 0, sizeof(SOCKADDR_IN));
	m_localAddress.sin_family = AF_INET;
	m_localAddress.sin_port = htons(localPort);
	m_localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(s, (SOCKADDR*) &m_localAddress, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		m_errorCode = WSAGetLastError();
		return FALSE;
	}
	
	//UDP needs only bind
	if(m_socketType == SOCK_DGRAM)
	{
		m_socketState = ACCEPTING;
		m_udpReadyToSend = FALSE;
		//Start reading thread
		m_readThread = CreateThread(NULL, 0, StartThread, this, 0, NULL );
		return TRUE;
	}

	//The following is TCP listen + accept code

	if(listen(s, maxConn) == SOCKET_ERROR)
	{
		m_errorCode = WSAGetLastError();
		return FALSE;
	}

	m_socketState = ACCEPTING;
	m_acceptThread = CreateThread(NULL, 0, StartThread, this, 0, NULL );

	return TRUE;
}

void CCESocket::AcceptServiceSocket(SOCKET serviceSocket)
{
	SOCKADDR_IN peerName;
	int nameSize = sizeof(SOCKADDR_IN);

	//Is it a valid socket?
	if(serviceSocket == INVALID_SOCKET)
		return;

	//Is it connected?
	if(getpeername(serviceSocket, (SOCKADDR*) &peerName, &nameSize) == SOCKET_ERROR)
	{
		shutdown(serviceSocket, SD_BOTH);
		closesocket(serviceSocket);
		return;
	}

	Disconnect();
	s = serviceSocket;

	if(m_recvBufSize == 0)
		m_recvBufSize = TCPBUFFERSIZE;

	m_socketState = CONNECTED;
	m_readThread = CreateThread(NULL, 0, StartThread, this, 0, NULL );
}

DWORD WINAPI CCESocket::StartThread(LPVOID pParam)
{
	CCESocket *parent = (CCESocket*) pParam;

	if(parent->m_socketState == ACCEPTING && parent->m_socketType == SOCK_STREAM)
		parent->AcceptThread();
	else
		parent->ReadThread();
	
	return 0;
}

void CCESocket::AcceptThread()
{
	SOCKET serviceSocket;

	m_acceptThreadState = RUNNING;

	do 
	{
		memset(&m_remoteAddress, 0, sizeof(SOCKADDR_IN));
		serviceSocket = accept(s, (SOCKADDR*) &m_remoteAddress, &m_receiveAddrSz);			
		if(serviceSocket == SOCKET_ERROR)
		{
			m_errorCode = WSAGetLastError();
			break;
		}

		//Accepts a new connection
		if(!OnAccept(serviceSocket))
		{
			shutdown(serviceSocket, SD_BOTH);
			closesocket(serviceSocket);
		}

	} while(m_acceptThreadState == RUNNING);

	m_acceptThreadState = CLOSING;
	if(m_socketState > DISCONNECTING)
	{
		Disconnect();
		OnClose(EVN_SERVERDOWN);
	}	
	// Self terminate the thread
	m_acceptThreadState = CLOSED;
	m_acceptThread = NULL;
	ExitThread(WM_QUIT);
}

// Read any incoming data as well as detect the connected lost/reset
void CCESocket::ReadThread()
{
	int bytesRead, bufSize;
	DataPacket *data;
	char * buf;

	m_readThreadState = RUNNING;

	do 
	{
		//m_recvBuffSize can change dynamically so we must track its changes
		bufSize = m_recvBufSize;
		buf = new char[bufSize];
		//Blocking receive
		switch(m_socketType)
		{
			case SOCK_STREAM:
				bytesRead = recv(s, buf, bufSize, 0);
				break;
			case SOCK_DGRAM:
				bytesRead = recvfrom(s, buf, bufSize, 0, (SOCKADDR*) &m_localAddress, &m_receiveAddrSz);
				break;
		}
		//If there was an error exit thread and notify owner with OnClose
		if(bytesRead == SOCKET_ERROR || bytesRead <= 0)
		{
			m_errorCode = WSAGetLastError();
			m_udpReadyToSend = FALSE;
			//This happens when we try to sendto data to a client host that closed its receiving
			//port. In this case we restart revfrom preventing further sendto until a new
			//request from a client host.
			if(m_socketType == SOCK_DGRAM && m_errorCode == WSAECONNRESET && m_socketState == ACCEPTING)
			{
				delete[] buf;
				//Notify that a remote connection closed
				OnClose(EVN_CONNLOST);
				continue;
			}
			break;
		}
		
		m_udpReadyToSend = TRUE;

		//If the client doesn't accepts the raw packet store it in the buffer
		if(!OnReceive(buf, bytesRead))
		{
			data = new DataPacket;
			data->len = bytesRead;
			data->buf = buf;
			data->pos = buf;
			
			//AddTail modifies internal list pointers. That's why we
			//need to lock while updating
#ifdef _WCE_SECTION
			EnterCriticalSection(&CCESocket::m_readLock);
#else
			CSingleLock csl(&CCESocket::m_readLock);
			csl.Lock();
#endif
			
			m_ReadBuffer.AddTail(data);
			
#ifdef _WCE_SECTION
			LeaveCriticalSection(&CCESocket::m_readLock);
#else
			csl.Unlock();
#endif
			
			m_availableData += bytesRead;
			
			//Notify that there are available data
			OnReceive();
		}

	} while(m_readThreadState == RUNNING);

	delete[] buf;
	m_readThreadState = CLOSING;
	if(m_socketState > DISCONNECTING)
	{
		if(m_socketType == SOCK_STREAM)
		{
			Disconnect();
			OnClose(EVN_CONNCLOSED);
		}
		else
		{
			m_socketState = CREATED;
			if(m_socketState == ACCEPTING)
				OnClose(EVN_SERVERDOWN);
			else
				OnClose(EVN_CONNLOST);
		}
	}	
	// Self terminate the thread
	m_readThreadState = CLOSED;
	m_readThread = NULL;
	// ExitThread(WM_QUIT);
}

int CCESocket::Read(char* buf, int len)
{
	int readBytes;
	DataPacket *data;

	if(!buf || len <= 0)
		return 0;
	
	readBytes = 0;

	while (m_ReadBuffer.GetNumEntries() > 0 && len > 0)
	{
		data = m_ReadBuffer.GetHead();

		//copies head packet and removes it from the buffer
		if(len >= data->len)
		{
			memcpy(&buf[readBytes], data->pos, data->len);
			len -= data->len;
			readBytes += data->len;
			delete[] data->buf;

			//RemoveHead modifies internal list pointers. That's why we
			//need to lock while updating
#ifdef _WCE_SECTION
			EnterCriticalSection(&CCESocket::m_readLock);
#else
			CSingleLock csl(&CCESocket::m_readLock);
			csl.Lock();
#endif
			
			delete m_ReadBuffer.RemoveHead();
			
#ifdef _WCE_SECTION
			LeaveCriticalSection(&CCESocket::m_readLock);
#else
			csl.Unlock();
#endif
		}
		//copies len data from the head packet and (fast) resizes it
		else
		{
			memcpy(&buf[readBytes], data->pos, len);
			data->len -= len;
			data->pos += len;
			readBytes += len;
			break;
		}
	}
	m_availableData -= readBytes;
	
	return readBytes;
}

bool CCESocket::ReadString(CString &str)
{
	bool strFound;
	int strSize, strPos;
	DataPacket *data;
	char *buf;
	char eol;

	strSize = 0;
	strFound = FALSE;

	if(m_eolFormat == EOL_NULL)
		eol = '\0';
	else
		eol = '\n';

	//We must do a double pass:
	// 1) Search for the string termination '\n'
	// 2) Copy string data
	if(!m_ReadBuffer.IsEmpty())
	{
		//First pass
		data = m_ReadBuffer.GetHead();
		for(strPos=0; strPos < data->len; strPos++)
			if(data->pos[strPos] == eol)
			{
				strSize = strPos +1;
				strFound = TRUE;
				break;
			}
		while(!strFound && !m_ReadBuffer.IsLast())
		{
			strSize += data->len;
			data = m_ReadBuffer.GetNext();
			for(strPos=0; strPos < data->len; strPos++)
				if(data->pos[strPos] == eol)
				{
					strSize += strPos +1;
					strFound = TRUE;
					break;
				}
		}
		//Second pass
		if(strFound)
		{
			strPos = 0;
			m_availableData -= strSize;
			buf = new char[strSize +1];
			buf[strSize] = '\0';
			while (strSize > 0)
			{
				data = m_ReadBuffer.GetHead();

				//copies head packet and removes it from the buffer
				if(strSize >= data->len)
				{
					memcpy(&buf[strPos], data->pos, data->len);
					strSize -= data->len;
					strPos += data->len;
					delete[] data->buf;

					//RemoveHead modifies internal list pointers. That's why we
					//need to lock while updating
					#ifdef _WCE_SECTION
						EnterCriticalSection(&CCESocket::m_readLock);
					#else
						CSingleLock csl(&CCESocket::m_readLock);
						csl.Lock();
					#endif

					delete m_ReadBuffer.RemoveHead();

					#ifdef _WCE_SECTION
						LeaveCriticalSection(&CCESocket::m_readLock);
					#else
						csl.Unlock();
					#endif
				}
				//copies len data from the head packet and (fast) resizes it
				else
				{
					memcpy(&buf[strPos], data->pos, strSize);
					data->len -= strSize;
					data->pos += strSize;
					break;
				}
			}
			str = buf; //Assign and convert to unicode
			delete[] buf;
			return TRUE;
		}		
	}
	str = _T("");
	return FALSE;
}

int CCESocket::GetNumPackets()
{
	return m_ReadBuffer.GetNumEntries();
}

int CCESocket::GetPacketSize()
{
	if(m_availableData > 0)
		return m_ReadBuffer.GetHead()->len;

	return 0;
}

bool CCESocket::GetPacket(char*& buf, int* len)
{
	DataPacket *data = NULL;

	//There's no need to lock while accessing data
	if(m_availableData > 0)
	{
		data = m_ReadBuffer.GetHead();
		*len = data->len;
		buf = data->buf;
		m_availableData -= *len;

		//RemoveHead modifies internal list pointers. That's why we
		//need to lock while updating
		#ifdef _WCE_SECTION
			EnterCriticalSection(&CCESocket::m_readLock);
		#else
			CSingleLock csl(&CCESocket::m_readLock);
			csl.Lock();
		#endif
		
			delete m_ReadBuffer.RemoveHead();

		#ifdef _WCE_SECTION
			LeaveCriticalSection(&CCESocket::m_readLock);
		#else
			csl.Unlock();
		#endif
	}
	
	return (data != NULL);
}
