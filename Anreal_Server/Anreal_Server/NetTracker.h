#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <Windows.h>

#include "Tracker.h"

// Class that implements a tracker that receives orientation data over a network.
// Inherits from CTracker.
class CNetTracker :
	public CTracker
{
public:
	// Default constructor.
	CNetTracker();
	
	// Overloaded constructor. Initializes the UDP socket to the specified portnum.
	CNetTracker(USHORT nPortNum);
	
	// Destructor.
	virtual ~CNetTracker();

	// Gets tracker data from the socket. Blocks if no incoming data.
	virtual HRESULT GetOrientation(TrkOrientation *pTrkOrientation);

private:
	// Initializes the Winsock DLL.
	HRESULT InitWinsock();

	// Cleans up Winsock.
	HRESULT CloseWinsock();

	// Opens the network socket for incoming orientation data.
	HRESULT OpenSocket();

	// Closes the network socket.
	HRESULT CloseSocket();

private:
	// Winsock
	bool m_bWinsockLoaded;			// Flag to set after successful WSAStartup

	// Socket
	USHORT m_nPort;					// Port to receive orientation data
	SOCKET m_hSocket;				// UDP socket for tracker connection
	SOCKADDR_IN m_addrSender;		// UDP packet sender address
};

