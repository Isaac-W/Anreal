#include "stdafx.h"
#include "NetTracker.h"

CNetTracker::CNetTracker() :
	m_nPort(DEFAULT_TRACKER_PORT),
	m_hSocket(NULL),
	m_bWinsockLoaded(false)
{
	HRESULT res;
	
	res = InitWinsock();
	ATLASSERT(SUCCEEDED(res));

	res = OpenSocket();
	ATLASSERT(SUCCEEDED(res));
}

CNetTracker::CNetTracker(USHORT nPortNum) :
	m_nPort(nPortNum),
	m_hSocket(NULL),
	m_bWinsockLoaded(false)
{
	HRESULT res;
	
	res = InitWinsock();
	ATLASSERT(SUCCEEDED(res));

	res = OpenSocket();
	ATLASSERT(SUCCEEDED(res));
}
	
CNetTracker::~CNetTracker()
{
	CloseSocket();
	CloseWinsock();
}

void CNetTracker::GetOrientation(TOrientation *pTrkOrientation)
{
	// Sender info
	SOCKADDR_IN addrSender;
	int nAddrSenderSize = sizeof(addrSender);
	int nBytesReceived = 0;

	// Data buffer
	const USHORT BUF_SIZE = 512; // bytes (TODO: Make constant or configurable)
	char pBuf[BUF_SIZE];

	// TODO -- Abort if network timeout
}

HRESULT CNetTracker::InitWinsock()
{
	if (!m_bWinsockLoaded)
	{
		WORD nWinsockVer = MAKEWORD(2, 2); // Request Winsock Version 2.2
		WSADATA wsaData;

		// Load Winsock DLL
		if (WSAStartup(nWinsockVer, &wsaData))
		{
			int nErr = WSAGetLastError();
			ATLASSERT(nErr);

			return E_FAIL;
		}

		m_bWinsockLoaded = true; // Winsock is considered loaded at this point

		// Confirm version
		if (nWinsockVer != wsaData.wVersion)
		{
			CloseWinsock();
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CNetTracker::CloseWinsock()
{
	if (m_bWinsockLoaded)
	{
		if (WSACleanup())
		{
			int nErr = WSAGetLastError();
			ATLASSERT(nErr);
		}

		m_bWinsockLoaded = false;
	}

	return S_OK;
}

HRESULT CNetTracker::OpenSocket()
{
	//
	//	Open socket
	//

	m_hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == m_hSocket)
	{
		m_hSocket = NULL;
		return E_FAIL;
	}

	//
	//	Set up receiving address
	//

	SOCKADDR_IN addrReceive;

	addrReceive.sin_family = AF_INET;
	addrReceive.sin_addr.s_addr = htonl(INADDR_ANY); // Sender address does not matter
	addrReceive.sin_port = htons(m_nPort);

	// Bind address to socket
	if (SOCKET_ERROR == bind(m_hSocket, (sockaddr *)(&addrReceive), sizeof(addrReceive)))
	{
		CloseSocket();
		return E_FAIL;
	}

	ATLASSERT(m_hSocket);
	return S_OK;
}

HRESULT CNetTracker::CloseSocket()
{
	if (m_hSocket)
	{
		if (closesocket(m_hSocket))
		{
			int nErr = WSAGetLastError();
			ATLASSERT(nErr);
		}

		m_hSocket = NULL;
	}

	return S_OK;
}