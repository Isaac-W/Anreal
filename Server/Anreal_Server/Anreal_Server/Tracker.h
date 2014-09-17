#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <Windows.h>

#include "ThreadHelper.h"

/////////////////////////////////////////////////////////////////////////////
// Tracker data structs

struct TOrientation
{
	TOrientation() :
		fYaw(0.0),
		fPitch(0.0),
		fRoll(0.0)
	{
	}

	float fYaw;
	float fPitch;
	float fRoll;
};

struct TTransform
{
	TTransform() :
		fYawMult(1.0),
		fPitchMult(1.0),
		fRollMult(1.0),
		fYawOffset(0.0),
		fPitchOffset(0.0),
		fRollOffset(0.0)
	{
	}

	// Scale
	float fYawMult;
	float fPitchMult;
	float fRollMult;

	// Shift
	float fYawOffset;
	float fPitchOffset;
	float fRollOffset;
};

/////////////////////////////////////////////////////////////////////////////
// CTracker : CRunnable

// Class that represents a head tracker object that receives orientation data over UDP.
class CTracker
{
public:
	// Default constructor.
	CTracker();

	// Overloaded constructor. Initializes the UDP socket to the specified portnum.
	CTracker(USHORT nPortNum);

	// Overloaded constructor. Initializes portnum and the specified orientation transformations.
	CTracker(USHORT nPortNum, const TTransform &trkTransform);

	// Destructor.
	~CTracker();

	//
	//	Orientation
	//

	// Gets the tracker orientation.
	void GetOrientation(TOrientation *pTrkOrientation) const;

	// Sets the tracker orientation.
	void SetOrientation(const TOrientation &trkOrientation);

	// Gets the yaw (Z-axis) angle
	float GetYaw() const;

	// Sets the yaw (Z-axis) angle
	void SetYaw(float fYaw);

	// Gets the pitch (Y-axis) angle
	float GetPitch() const;

	// Sets the pitch (Y-axis) angle
	void SetPitch(float fPitch);

	// Gets the roll (X-axis) angle
	float GetRoll() const;

	// Sets the roll (X-axis) angle
	void SetRoll(float fRoll);

	//
	//	Linear orientation transformation
	//

	// Gets the transformation values.
	void GetTransformation(TTransform *pTrkTransform) const;

	// Sets the transformation values.
	void SetTransformation(const TTransform &trkTransform);

	// Gets the yaw (Z-axis) transformation values.
	void GetYawTransform(float *pMult, float *pOffset) const;

	// Sets the yaw (Z-axis) transformation values.
	void SetYawTransform(float fMult, float fOffset);

	// Gets the pitch (Y-axis) transformation values.
	void GetPitchTransform(float *pMult, float *pOffset) const;

	// Sets the pitch (Y-axis) transformation values.
	void SetPitchTransform(float fMult, float fOffset);

	// Gets the roll (X-axis) transformation values.
	void GetRollTransform(float *pMult, float *pOffset) const;

	// Sets the roll (X-axis) transformation values.
	void SetRollTransform(float fMult, float fOffset);

	//
	//	Socket
	//

	HRESULT OpenUDPSocket();
	bool IsSocketOpen();
	
private:
	// Orientation
	TOrientation m_trkOrientation;	// Tracker orientation data
	TTransform m_trkTransform;		// Tracker transformation values

	// Socket
	USHORT m_nPort;					// Port to receive orientation data
	SOCKET m_hSocket;				// UDP socket for tracker connection
};

