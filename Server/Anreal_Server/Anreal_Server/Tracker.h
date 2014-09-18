#pragma once

/////////////////////////////////////////////////////////////////////////////
// Tracker data structs

// Tracker orientation struct.
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

// Tracker linear transformation struct.
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
// CTracker

// Class that represents a head tracker object. Inheriting class must override GetOrientation().
class CTracker
{
public:
	// Default constructor.
	CTracker()
	{
	}

	// Destructor.
	virtual ~CTracker()
	{
	}

	// Gets the tracker orientation.
	virtual void GetOrientation(TOrientation *pTrkOrientation) = 0;
};

