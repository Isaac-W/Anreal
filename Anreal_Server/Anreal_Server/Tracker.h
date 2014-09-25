#pragma once

/////////////////////////////////////////////////////////////////////////////
// Tracker data structs

// Tracker orientation struct. Angles are represented in radians.
// Tracker angles sent over the network should follow the same data format.
struct TrkOrientation
{
	TrkOrientation() :
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
// Describes how the individual angles should be scaled and shifted per game.
struct TrkTransform
{
	TrkTransform() :
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

// Tracker parameter struct. Provides a minimum set of parameters a tracker must utilize.
struct TrkParam
{
	TrkParam() :
		bDisableYaw(false),
		bDisablePitch(false),
		bDisableRoll(false)
	{
	}

	bool bDisableYaw;		// Disable yaw tracking		
	bool bDisablePitch;		// Disable pitch tracking
	bool bDisableRoll;		// Disable roll tracking
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
	virtual HRESULT GetOrientation(TrkOrientation *pTrkOrientation) = 0;
};

