// This Winsock Wrapper replaces all MFC socket classes (CSocket, CAsyncSocket
// CCeSocket). It offers async notifications, buffered read and easy data access
// functions.
//
// Coded by Marco Zaratti / marco.zaratti@gmail.it / 2006.04.05
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CESOCKET_H__DEAC64AC_C034_4162_9C57_5C75CBEDE2BB__INCLUDED_)
#define AFX_CESOCKET_H__DEAC64AC_C034_4162_9C57_5C75CBEDE2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32_WCE
	#define _WCE_SECTION
#endif

#ifdef _WCE_SECTION
	#pragma comment(lib,"ws2.lib")
#else
	#pragma comment (lib, "ws2_32.lib")
	#include <afxmt.h>
#endif

#include <winsock2.h>
#include "containers.h"

#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

#define TCPBUFFERSIZE 1024
#define UDPBUFFERSIZE 2048

#define THREAD_TERMINATION_MAXWAIT	5	//seconds

//! Winsock wrapper class
/*! @author Marco Zaratti - marco.zaratti@gmail.com - 2006.04.05
 *
 *  This class is designed to replace bugged MFC CCeSocket. It allows to
 *  use both TCP and UDP socket types in both client or server configuration.
 *  Provides async events notification for:
 *   - Data receive
 *   - Closed connections
 *   - Accepted clients (only for TCP)
 *  It implements a dynamic buffer that can be accessed through simple functions:
 *   - Binary read: it's like accessing a file, usefule for TCP streming contents
 *   - Text read: it's possible to read single strings terminated with CR
 *   - Packet read: you can also read single packets, exactly as they were received.
 *
 *  To use the socket first call Create function. If you want to make a client call
 *  Connect, if you want to make a server call Accept. This is all you need to have
 *  a properly working socket. To clse the socket call Disconnect. After a disconnect
 *  you must call Create again. To receive notifications you should subclass CCESocket
 *  and override virtual notification functions that you need. Keep in mind that these
 *  functions are called from another thread, not from the main application thread. If
 *  you need to execute something in the window thread you could send a message to
 *  it with PostMessage (this is required i.e. for MFC objects. They don't work if passed
 *  among threads, you should use these objects in the same thread were they were defined).
 */
class CCESocket  
{
public:
	//! Possible socket states
	enum socketState {NONE=0, DISCONNECTING, CREATED, CONNECTED, ACCEPTING};
	//! End of line formats used to instruct SendLine function on how to terminate a line.
	//! Default is EOL_LFCR ("\r\n").
	enum eolFormat {EOL_NULL=0, EOL_LFCR, EOL_CR};
	//! Asyncronous events sent with the OnClose notification
	enum asyncEvents {EVN_CONNCLOSED=0, EVN_CONNLOST, EVN_SERVERDOWN};

	CCESocket();
	virtual ~CCESocket();

	///////////////////////////////////////////////////////
	// CREATION FUNCTION
	///////////////////////////////////////////////////////

	//! Creates the socket.
	/*! Create a TCP or UDP socket.
	 *  @param[in] socketType Can be SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
	 *  @param[in] bufferSize You can specify the buffer size for incoming
	 *                    packets. Otherwise default values will be used:
	 *                    1024 bytes for TCP and 2048 bytes for UDP.
	 *  @return TRUE if created successfully, FALSE if an error occurs.
	 */
	bool Create(int socketType, int bufferSize = 0);

	///////////////////////////////////////////////////////
	// CONNECTION FUNCTIONS
	///////////////////////////////////////////////////////

	//! Connect to a remote host.
	/*! Connect to a remote host.
	 *  @param[in] addr		  Remote host address. It can be an IP number or the host name.
	 *  @param[in] remotePort Remote host port to connect to.
	 *  @return TRUE if connected successfully, FALSE if an error occurs.
	 *  @remarks This function connects only TCP sockets. UDP sockets will
	 *           save the passed address and use it to send data. Besides
	 *           UDP sockets can connect multiple times to different addresses,
	 *           it is not necessary to disconnect each time.
	 */
	bool Connect(CString &addr, UINT remotePort);

	//! Accepts an incoming connection request.
	/*! Accepts an incoming connection request. To make a server first call
	 *  Create and then Accept. This will bind the socket to a local port
	 *  waiting for connections. TCP will trigger OnAccept giving the
	 *  possibility to accept or refuse the connection. If accepted you must
	 *  create a new (subclassed) CCESocket class and pass it the service socket
	 *  with AcceptServiceSocket. The new class will be already connected and
	 *  can be used to send and receive data. UDP will trigger a normal OnReceive
	 *  notification. After receiving this data you can use the same socket to send
	 *  data to the remote client. UDP acts blindly, you never know if someone
	 *  is really listening, and doesn't require service sockets like TCP does.
	 *  @param[in] localPort Local port to bind for incoming requests.
	 *  @param[in] maxConn   Maximum length of the queue of pending connections.
	 *  @return TRUE if accept was successful, FALSE if an error occurs.
	 */
	bool Accept(UINT localPort, int maxConn = SOMAXCONN);

	//! Disconnects the socket.
	/*! Disconnect the socket. Both client (connected) or server (accpting)
	 *  sockets will be closed. Disconnect is an irrevocable request for
	 *  complete network activities termination (related to this socket).
	 */
	void Disconnect();

	///////////////////////////////////////////////////////
	// SENDING FUNCTIONS
	///////////////////////////////////////////////////////

	//! Send data to a remote host.
	/*! Send data to a remote host.
	 *  @param[in] buf Contains the data to send.
	 *  @param[in] len Length of the buffer.
	 *  @return Number of bytes sent or SOCKET_ERROR if an error occurs.
	 *  @remarks You cannot send data with a listening TCP socket.
	 */
	int Send(const char* buf, int len);

	//! Send a string to a remote host.
	/*! @param[in] str String to send.
	 *  @return Number of bytes sent or SOCKET_ERROR if an error occurs.
	 *  @remarks You cannot send data with a listening TCP socket.
	 */
	int Send(CString& str);

	//! Send a string to a remote host and adds EOL.
	/*! @param[in] str String to send.
	 *  @return Number of bytes sent or SOCKET_ERROR if an error occurs.
	 *  @remarks You cannot send data with a listening TCP socket.
	 */
	int SendLine(CString &str);

	///////////////////////////////////////////////////////
	// ASYNCHRONOUS NOTIFICATIONS
	///////////////////////////////////////////////////////

	//! Received data notification.
	/*! This notification is called directly from the receiving thread
	 *  to tell that there are new data in the queue. This is the first
	 *  notification of this kind and offers the possibility to
	 *  get the data without buffering them. If you accept the data return
	 *  TRUE. You are responsible of deleting buf after its use. If you return
	 *  FALSE then you'll receive OnReceive() notification.
	 *  @param[out] buf Received data packet.
	 *  @param[out] len Packet length.
	 *  @return Return TRUE if you accept the data, FALSE otherwise.
	 *  @remarks You are on a different thread here so pay attentions. MFC
	 *           objects cannot be passed among threads. Make something
	 *           with the data and then send a message to the main thread or
	 *           make something else that will not hurt MFC.
	 */
	virtual bool OnReceive(char* buf, int len){return FALSE;}

	//! Received data notification.
	/*! If you refused the first OnReceive notification (probably because
	 *  you didn't implemented it) you'll receive this call. Now you can
	 *  access to the data through a data access function.
	 *  @remarks You are on a different thread here so pay attentions. MFC
	 *           objects cannot be passed among threads. Make something
	 *           with the data and then send a message to the main thread or
	 *           make something else that will not hurt MFC.
	 */
	virtual void OnReceive(){}

	//! Accepts an incoming connection request.
	/*! Accepts an incoming connection request. You can refuse or accept it.
	 *  If refused the serviceSocket will be automatically closed. If
	 *  accepted you must create a new (subclassed) CCESocket class and call
	 *  its AcceptServiceSocket. The new class will be already connected and
	 *  can be used to send and receive data.
	 *  @param[in] serviceSocket This is the socket that was connected with
	 *                           the remote client. Pass it to AcceptServiceSocket.
	 *  @return Return FALSE if you don't accept the connection.
	 */
	virtual bool OnAccept(SOCKET serviceSocket){return FALSE;}

	//! Attaches a connected socket.
	/*! Use this function in conjunction with OnAccept to receive remote client
	 *  connections requests.
	 *  @param[in] serviceSocket This is the socket connected with the remote
	 *                           client received from OnAccept notification.
	 */
	void AcceptServiceSocket(SOCKET serviceSocket);

	//! The socket lost connection or failed.
	/*! You receive this notification if something goes wrong:
	 *
	 *   - You are a client and, for some reason, the read thread failed:
	 *      - TCP: closeEvent = EVN_CONNCLOSED. The socket is disconnected. You must call
	 *        create and connect to open another connection.
	 *      - UDP: closeEvent = EVN_CONNLOST. The socket is in created state. You
	 *        must call connect to open another connection.
	 *   - You are a client and the server whom you are sending data disconnected:
	 *      - TCP: closeEvent = EVN_CONNCLOSED. The socket is disconnected. You must call
	 *        create and connect to open another connection.
	 *      - UDP: closeEvent = EVN_CONNLOST. The socket is in created state. You
	 *        must call connect to open another connection. This event happens only if you
	 *        try to send data to a disconnected server.
	 *   - You are a server and, for some reasone, the read (UDP) or accept (TCP) thread failed:
	 *      - TCP: closeEvent = EVN_SERVERDOWN. The socket is disconnected. You must call
	 *        create and accept to start another server.
	 *      - UDP: closeEvent = EVN_SERVERDOWN. The socket is in created state. You
	 *        must call accept to start another server.
	 *   - You are a server and the client disconnected:
	 *      - TCP: closeEvent = EVN_CONNCLOSED. Service socket disconnected.
	 *      - UDP: closeEvent = EVN_CONNLOST. Server is working and waiting for
	 *        another connection. This event happens only if you try to send data
	 *        to a disconnected client.
	 *
	 *  You can call GetLastError to retrive the error code.
	 *  Even if disconnected you have still access to the buffered data.
	 *  @remarks If you set up an UDP server you'll receive OnClose when the remote
	 *           client disconnected. This allows you to stop sending data. TCP server
	 *           never receives OnClose.
	 */
	virtual void OnClose(/*asyncEvents*/ int closeEvent){}

	///////////////////////////////////////////////////////
	// DATA ACCESS FUNCTIONS
	///////////////////////////////////////////////////////

	//! Read binary data.
	/*! Use this function to read buffered data.
	 *  @param[out] buf The buffer that will receive the data. It must
	 *                  be already allocated.
	 *  @param[in]  len Length of the buffer.
	 *  @return Number of bytes actually read.
	 */
	int Read(char* buf, int len);

	//! Read a string from the buffer.
	/*! @param[out] str A reference to a CString that contains the
	 *                  resultant string after it is read from the buffer.
	 *  @return TRUE if the string was read, FALSE if there was no string to read.
	 */
	bool ReadString(CString &str);

	//! Returns the total data size queued in the buffer.
	int GetDataSize() {return m_availableData;}

	//! Gets the number of packet queued in the buffer.
	/*! This function is designed to help in UDP packets retrieval.
	 *  @return Number of packets currently buffered.
	 */ 
	int GetNumPackets();

	//! Gets the size of the head packet in the queue (next packet to to be retrieved).
	int GetPacketSize();

	//! Retrieve the head packet.
	/*! Retrieve the head packet from the FIFO buffer.
	 *  @param[out] buf A pointer to the buffer that will contain packet data.
	 *                  The pointer will be internally allocated to match the
	 *                  packet size. It must be manualy deleted after its use.
	 *  @param[out] len It will contain the buffer length.
	 *  @return TRUE if the packet was successfuly retrieved, FALSE if there was
	 *          no packet to retrieve.
	 */
	bool GetPacket(char*& buf, int* len);

	///////////////////////////////////////////////////////
	// OTHER FUNCTIONS
	///////////////////////////////////////////////////////

	//! Modify buffer size.
	/*! Call this function to modify buffer size. You can call it at any time,
	 *  even if the socket is already connected. This is usefull if you want to
	 *  change buffer size after a AcceptServiceSocket call.
	 *  @param bufSize New buffer size.
	 */
	void SetBufferSize(int bufSize);

	//! Sets the new EOL format for SendLine and ReadString functions
	void SetEolFormat(eolFormat eol) {m_eolFormat = eol;}

	//! Returns the error code of the last error.
	int GetLastError() {return m_errorCode;}

	//! @return SOCK_STREAM for TCP socket, SOCK_DGRAM for UDP socket.
	int GetSocketType() {return m_socketType;}

	//! @return 0=NONE, 1=DISCONNECTED, 2=CREATED, 3=CONNECTED, 4=ACCEPTING.
	int GetSocketState() {return m_socketState;}

protected:

	static DWORD WINAPI StartThread(LPVOID pParam);
	void ReadThread();
	void AcceptThread();

	enum threadState {CLOSED=0, CLOSING, RUNNING};

	static bool m_bWSAStarted;

	SOCKET s;
	int m_socketType;
	socketState m_socketState;
	SOCKADDR_IN m_remoteAddress;
	SOCKADDR_IN m_localAddress;
	int m_receiveAddrSz;
	bool m_udpReadyToSend;

	HANDLE m_readThread;
	HANDLE m_acceptThread;
	threadState m_readThreadState;
	threadState m_acceptThreadState;

	int m_eolFormat;
	int m_errorCode;

	int m_recvBufSize;
	struct DataPacket
	{
		int len;	//buffer length
		char *buf;	//actual buffer
		char *pos;	//position in the buffer (avoids buffer resizing)
	};
	CTList<DataPacket*> m_ReadBuffer;

#ifdef _WCE_SECTION
	static CRITICAL_SECTION m_readLock;	//!< Controls read buffer access.
#else
	static CCriticalSection m_readLock;	//!< Controls read buffer access.
#endif
	ulong m_availableData;

};

#endif // !defined(AFX_CESOCKET_H__DEAC64AC_C034_4162_9C57_5C75CBEDE2BB__INCLUDED_)
