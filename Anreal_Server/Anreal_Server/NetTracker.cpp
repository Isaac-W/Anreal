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

HRESULT CNetTracker::GetOrientation(TrkOrientation *pTrkOrientation)
{
	if (!pTrkOrientation)
	{
		return E_POINTER;
	}

	//
	//	Get data from socket
	//

	// Data buffer
	char pBuf[TRK_BUF_SIZE];

	// Sender info
	SOCKADDR_IN addrSender;
	int nAddrSenderSize = sizeof(addrSender);

	// Blocks if no incoming data (TODO: Set timeout so that we can force stop the thread!)
	int nBytesReceived = recvfrom(	m_hSocket,
									pBuf, TRK_BUF_SIZE, 0,
									(SOCKADDR *)(&addrSender), &nAddrSenderSize);

	if (SOCKET_ERROR == nBytesReceived)
	{
		return E_FAIL;
	}

	//
	//	Convert raw data to orientation struct
	//

	USHORT nCurByte = 0;

	// Yaw
	memcpy(&(pTrkOrientation->fYaw), &(pBuf[nCurByte]), sizeof(pTrkOrientation->fYaw));
	nCurByte += sizeof(pTrkOrientation->fYaw);

	// Pitch
	memcpy(&(pTrkOrientation->fPitch), &(pBuf[nCurByte]), sizeof(pTrkOrientation->fPitch));
	nCurByte += sizeof(pTrkOrientation->fPitch);

	// Roll
	memcpy(&(pTrkOrientation->fRoll), &(pBuf[nCurByte]), sizeof(pTrkOrientation->fRoll));
	nCurByte += sizeof(pTrkOrientation->fRoll);

	ATLASSERT(sizeof(TrkOrientation) >= nCurByte);

	return S_OK;
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
	if (!m_hSocket)
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
	}

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