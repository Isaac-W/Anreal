#pragma once

#include "ThreadHelper.h"
#include "Tracker.h"

class CMemTDriver :
	public CRunnable
{
public:
	// Default constructor.
	CMemTDriver();

	// Overloaded constructor.
	CMemTDriver(USHORT nPortNum);

	// Overloaded constructor. Initializes portnum and the specified orientation transformations.
	CMemTDriver(USHORT nPortNum, const TTransform &trkTransform);

	// Destructor.
	~CMemTDriver();

	// Gets the transformation values.
	void GetTransformation(TTransform *pTrkTransform);

	// Sets the transformation values.
	void SetTransformation(const TTransform &trkTransform);

	// Starts the tracker process.
	virtual void Run();

private:
	TTransform m_trkTransform;		// Tracker transformation values
};

