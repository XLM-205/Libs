/*------- Internet Socket -------
*	Internet Socket
*	--Manages connection between computers
*
*
*	Moon Wiz Studios (c) - 03/12/2019
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.5.0
*/

#ifndef _H_MW_SOCKET_
#define _H_MW_SOCKET_

#include "Igniter.h"

#ifndef _H_CSTTYPE_
typedef unsigned char uint8;
typedef unsigned short uint16
typedef unsigned int uint32;
#endif

#define LOCAL_HOST "127.0.0.1"

#ifdef MWI_OS_WINDOWS
	#include <WinSock2.h>				//Requires 'ws2_32.lib'
	#include <WS2tcpip.h>				//For helping converting the IP to 'in_addr'
	#pragma comment(lib,"ws2_32.lib")	//Winsock Library
#else
	#include <sys/socket.h>
#endif

enum SocketErrors
{
	INVALID_TYPE = -1,
};

enum SocketDomain
{
#ifdef MWI_OS_WINDOWS
	//APPLE_TALK = AF_APPLETALK,
	//BLUETOOTH = AF_BTH,
	IPv4 = AF_INET,					//IPv4 (255.255.255.255) Default
	IPv6 = AF_INET6,
	//IPX_SPX = AF_IPX,
	//IRDA = AF_IRDA,
	//NETBIOS = AF_NETBIOS,
	//UNSPECIFIED = AF_UNSPEC
#else
#endif
};

class SocketInfo
{
private:
	union Addr
	{
		uint8  byte[4];
		uint16 partial[2];
		uint32 full;
	};

public:
	uint16 Port;
	Addr Address;

	SocketInfo(uint32 addrs, uint16 port)
	{
		Port = port;
		Address.full = addrs;
	}

#ifdef _INC_STDIO
	void print(void)
	{
		printf("%hhu.%hhu.%hhu.%hhu [%hu]\n", Address.byte[0], Address.byte[1], Address.byte[2], Address.byte[3], Port);
	}
#endif
};

class SocketBase
{
private:
	SocketBase& operator=(const SocketBase&) = delete;	//Disallow cloning sockets!

protected:
	const static uint16 DEFAULT_PORT = 8080;	//Default Port Server (Personal Web Server))

	uint16 m_sockPort;							//Socket port
	uint16 m_sockDmn;							//Socket Domain (IPv4 / IPv6)
	int m_sockAddrSize;							//Size of 'm_sockAddr'
	int m_sockConDscrptr;						//Connection Descriptor
	//int m_sockClientDscrptr;		//Client Connection Descriptor, used only by the server during 'accept' phase. Should pass it to a thread
	int m_sockConType;							//Connection (TCP / UDP)
	//int m_sockPrtl;				//Internet Protocol (Same as the IP header of a packet)
	int m_sockOpt = 1;							//Socket options (?)
	sockaddr_in m_sockAddr;						//Socket Address	
	ErrorDescription m_err;						//Error codes and descriptions
	bool m_isReady = false;						//Becomes true if the socket was initialized and ready
	bool m_isConnected = false;					//Becomes true if a client socket successfully connected to a server

	inline int& getSocketInstances(void)
	{
		static int instances = 0;
		return instances;
	}
	inline void addSocketInstance(void)
	{
		getSocketInstances()++;
	}
	inline void removeSocketInstance(void)
	{
		if (getSocketInstances() > 0)
		{
			getSocketInstances()--;
		}
	}
	
	sockaddr_in makeSockAddr(uint16 port, uint16 domain)
	{
		sockaddr_in out;
		out.sin_family = domain;
		out.sin_port = htons(port);
		out.sin_addr.S_un.S_addr = INADDR_ANY;
		return out;
	}
	sockaddr_in makeSockAddr(const char *IP, uint16 port, uint16 domain)
	{
		sockaddr_in out;
		out.sin_family = domain;
		out.sin_port = htons(port);
		inet_pton(m_sockDmn, IP, &out.sin_addr.S_un.S_addr);
		return out;
	}

	virtual bool init(void) = 0;

	void build(int port, SocketDomain dmn)
	{
		m_sockPort = (uint16)port;
		m_sockDmn  = (uint16)dmn;
		m_sockAddrSize = sizeof(m_sockAddr);
		if (getSocketInstances() == 0)	//Only initialize system's services once!
		{
#ifdef MWI_OS_WINDOWS
			WSADATA data;	//Starting up WSA Subsystem
			WSAStartup(MAKEWORD(2, 2), &data);
#else

#endif
		}
		m_isReady = this->init();
		if (m_isReady)
		{
			addSocketInstance();
		}
	}

public:
	~SocketBase()
	{
		if (m_isReady)
		{
			removeSocketInstance();
			closesocket(m_sockConDscrptr);
		}
		if (getSocketInstances() == 0)
		{
			WSACleanup();
		}
	}

	SocketInfo connectionInfo(void)
	{
		return SocketInfo(m_sockAddr.sin_addr.S_un.S_addr, m_sockPort);
	}

	bool Connect(const char *IP)
	{
		if (this->isServer())
		{
			m_err.SetError(SocketErrors::INVALID_TYPE, "[CON] Tried to connect with a Server Socket");
			m_isConnected = false;
			return m_isConnected;
		}
#ifdef MWI_OS_WINDOWS
		//Changes m_sockAddr with data from server
		if (inet_pton(m_sockDmn, IP, &m_sockAddr.sin_addr.S_un.S_addr) <= 0)
		{
			m_err.SetError(WSAGetLastError(), "[CON] Failed to convert IP Address");
			m_isConnected = false;
			return m_isConnected;
		}
		m_sockAddrSize = sizeof(m_sockAddr);
		if (connect(m_sockConDscrptr, (sockaddr*)&m_sockAddr, m_sockAddrSize) < 0)
		{
			m_err.SetError(WSAGetLastError(), "[CON] Failed to connect");
			m_isConnected = false;
			return m_isConnected;
		}
#else
#endif
		m_err.ClearError();
		m_isConnected = true;
		return m_isConnected;
	}

	int Read(char *buf, int bSize)
	{
		//Returns the amount of bytes received. If 0, connection closed, if <0, error
		return recv(m_sockConDscrptr, buf, bSize, 0);
	}
	int ReadFrom(char *buf, int bSize, const char *IP)	//TEST
	{
		sockaddr_in sock = makeSockAddr(IP, m_sockPort, m_sockDmn);
		int sockSize = sizeof(sock);
		if (bind(m_sockConDscrptr, (sockaddr*)&sock, sockSize) != 0)
		{
			if (this->isClient())
			{
				m_err.SetError(WSAGetLastError(), "[CLI] Failed to bind ReadFrom Target");
			}
			else
			{
				m_err.SetError(WSAGetLastError(), "[SVR] Failed to bind ReadFrom Target");
			}
			return -1;
		}
		return recvfrom(m_sockConDscrptr, buf, bSize, 0, (sockaddr*)&sock, &sockSize);
	}
	int ReadFrom(char *buf, int bSize, const char *IP, uint16 port, uint16 domain)	//TEST
	{
		sockaddr_in sock = makeSockAddr(IP, port, domain);
		int sockSize = sizeof(sock);
		if (bind(m_sockConDscrptr, (sockaddr*)&sock, sockSize) != 0)
		{
			if (this->isClient())
			{
				m_err.SetError(WSAGetLastError(), "[CLI] Failed to bind ReadFrom Target");
			}
			else
			{
				m_err.SetError(WSAGetLastError(), "[SVR] Failed to bind ReadFrom Target");
			}
			return -1;
		}
		return recvfrom(m_sockConDscrptr, buf, bSize, 0, (sockaddr*)&sock, &sockSize);
	}

	int Send(const char *data, int bSize)
	{
		return send(m_sockConDscrptr, data, bSize, 0);
	}
	int SendTo(const char *data, int bSize, const char *IP)	//TEST
	{
		sockaddr_in sock = makeSockAddr(IP, m_sockPort, m_sockDmn);
		return sendto(m_sockConDscrptr, data, bSize, 0, (sockaddr*)&sock, sizeof(sock));
	}
	int SendTo(const char *data, int bSize, const char *IP, uint16 port, uint16 domain)	//TEST
	{
		sockaddr_in sock = makeSockAddr(IP, port, domain);
		return sendto(m_sockConDscrptr, data, bSize, 0, (sockaddr*)&sock, sizeof(sock));
	}

	int Shutdown(void)
	{
		shutdown(m_sockConDscrptr, SD_SEND);
	}

	virtual bool isClient(void) = 0;
	virtual bool isServer(void) = 0;

	bool isConnected(void)
	{
		return m_isConnected;
	}
	bool isReady(void)
	{
		return m_isReady;
	}

	ErrorDescription* getError(void)
	{
		return &m_err;
	}
};

class SocketServer : public SocketBase
{
private:
	bool init(void)
	{
	#ifdef MWI_OS_WINDOWS
		SOCKET temp = socket(m_sockDmn, SOCK_STREAM, 0);
		if (temp == INVALID_SOCKET)
		{
			m_err.SetError(WSAGetLastError(), "[SVR] Failure Creating Socket Descriptor");
			return false;
		}
		if (setsockopt(temp, SOL_SOCKET, SO_KEEPALIVE, (char*)&m_sockOpt, sizeof(m_sockOpt)))
		{
			m_err.SetError(WSAGetLastError(), "[SVR] Failure Attaching Socket");
			return false;
		}
		m_sockAddr = makeSockAddr(m_sockPort, m_sockDmn);
		if (bind(temp, (sockaddr*)&m_sockAddr, m_sockAddrSize) == SOCKET_ERROR)
		{
			m_err.SetError(WSAGetLastError(), "[SVR] Failure Binding Socket");
			return false;
		}
		if (listen(temp, SOMAXCONN) < 0)	//Where 'SOMAXCONN' defines the length of the queue of pending connections
		{
			m_err.SetError(WSAGetLastError(), "[SVR] Failure Set Listening State");
			return false;
		}
		m_sockConDscrptr = accept(temp, nullptr, nullptr);
		if (m_sockConDscrptr < 0)	//Accept changes m_sockAddr with the incoming data
		{
			m_err.SetError(WSAGetLastError(), "[SVR] Failure Accepting Socket");
			return false;
		}
		closesocket(temp);
	#else
	#endif
		//Everything went perfectly well!
		m_err.ClearError();
		return true;
	}

public:
	SocketServer(int port, SocketDomain domain)
	{
		build(port, domain);
	}
	SocketServer(int port)
	{
		build(port, SocketDomain::IPv4);
	}
	SocketServer()
	{
		build(DEFAULT_PORT, SocketDomain::IPv4);
	}

	//Set server to listening state for new connections
	bool Listen(void)
	{
		return init();
	}

	bool isClient(void)
	{
		return false;
	}
	bool isServer(void)
	{
		return true;
	}

};

class SocketClient : public SocketBase
{
protected:
	bool init(void)
	{
#ifdef MWI_OS_WINDOWS
		if ((m_sockConDscrptr = socket(m_sockDmn, SOCK_STREAM, 0)) < 0)
		{
			m_err.SetError(m_sockConDscrptr, "[CLI] Failure Creating Socket Descriptor");
			return false;
		}
		m_sockAddr.sin_family = m_sockDmn;
		m_sockAddr.sin_port = htons(m_sockPort);
#else
#endif
		m_err.ClearError();
		return true;
	}

public:
	SocketClient(int port, SocketDomain domain)
	{
		build(port, domain);
	}
	SocketClient(int port)
	{
		build(port, SocketDomain::IPv4);
	}
	SocketClient()
	{
		build(DEFAULT_PORT, SocketDomain::IPv4);
	}

	bool isClient(void)
	{
		return true;
	}
	bool isServer(void)
	{
		return false;
	}

};

#endif