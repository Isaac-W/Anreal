#pragma once

#include "ThreadHelper.h"
#include "NetTracker.h"

class CMemTDriver :
	public CRunnable
{
public:
	// Constructor. Initializes portnum and the pointer offset list.
	// TODO... pass a new struct?

	// Constructor. Initializes portnum and the specified orientation transformations.
	CMemTDriver(USHORT nPortNum, const TTransform &trkTransform);

	// Destructor.
	virtual ~CMemTDriver();

	// Gets the transformation values.
	void GetTransformation(TTransform *pTrkTransform);

	// Sets the transformation values.
	void SetTransformation(const TTransform &trkTransform);

	// Starts the tracker process.
	virtual void Run();

private:
	CTracker *m_pTracker;			// Pointer to member tracker
	TTransform m_trkTransform;		// Tracker transformation values
};

