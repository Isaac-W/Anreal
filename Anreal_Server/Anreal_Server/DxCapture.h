#pragma once

#include "Capture.h"

#include <d3d9.h>

class CDxCapture :
	public CCapture
{
public:
	CDxCapture(HWND hWnd, const CptParam &cptParam);
	virtual ~CDxCapture();

	virtual HRESULT GetCapture(CptImageDesc *pCptImageDesc);

private:
	// Initializes Direct3D.
	HRESULT InitD3D();

	// Closes and frees Direct3D.
	HRESULT CloseD3D();

	// Creates capture surface.
	HRESULT CreateSurface();

	// Releases capture surface resources.
	HRESULT ReleaseSurface();

private:
	// Parameters
	CptParam m_cptParam;

	// hWnd
	HWND m_hWnd;

	// DirectX
	LPDIRECT3D9 m_pDirect3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DSURFACE9 m_pCaptureSurface;

	// Image file buffer
	BYTE *pImageBits;
};
