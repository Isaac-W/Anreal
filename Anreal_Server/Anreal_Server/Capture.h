#pragma once

/////////////////////////////////////////////////////////////////////////////
// Capture data structs

// Capture parameter struct. Provides a minimum set of parameters a capture class must utilize.
struct CptParam
{
	CptParam() :
		bCaptureRegion(false),
		rcCapture(0, 0, 0, 0)
	{
	}

	bool bCaptureRegion;
	CRect rcCapture;
};

// Capture image description struct.
// Provides info about the format of the captured image.
struct CptImageDesc
{
	CptImageDesc() :
		pBits(NULL),
		nPitch(0),
		nWidth(0),
		nHeight(0),
		nBPP(4)
	{
	}

	BYTE *pBits;
	int nPitch;
	
	int nWidth;
	int nHeight;
	int nBPP;
};

/////////////////////////////////////////////////////////////////////////////
// CCapture

// Class that represents a generic screen capture object. Inheriting class must override GetCapture().
class CCapture
{
public:
	// Default constructor.
	CCapture()
	{
	}

	// Destructor.
	virtual ~CCapture()
	{
	}

	// Gets a capture of the screen. Returns a buffer containing the image file.
	virtual HRESULT GetCapture(CptImageDesc *pCptImageDesc) = 0;
};
