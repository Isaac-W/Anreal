#include "stdafx.h"

#include "DxCapture.h"

CDxCapture::CDxCapture(HWND hWnd, const CptParam &cptParam) :
	m_hWnd(hWnd),
	m_cptParam(cptParam)
{
	HRESULT res;

	res = InitD3D();
	ATLASSERT(SUCCEEDED(res));

	res = CreateSurface();
	ATLASSERT(SUCCEEDED(res));
}

CDxCapture::~CDxCapture()
{
	HRESULT res;

	res = ReleaseSurface();
	ATLASSERT(SUCCEEDED(res));

	res = CloseD3D();
	ATLASSERT(SUCCEEDED(res));
}

HRESULT CDxCapture::GetCapture(CptImageDesc *pCptImageDesc)
{
	if (!pCptImageDesc)
	{
		ATLASSERT(pCptImageDesc);
		return E_POINTER;
	}

	HRESULT res;

	//
	//	Unlock rect
	//

	res = m_pCaptureSurface->UnlockRect();
	ATLASSERT(SUCCEEDED(res));

	//
	//	Get front buffer
	//

	res = m_pD3DDevice->GetFrontBufferData(0, m_pCaptureSurface);
	ATLASSERT(SUCCEEDED(res));

	//
	//	Lock rect
	//

	D3DLOCKED_RECT rectSurface;
	res = m_pCaptureSurface->LockRect(&rectSurface, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	ATLASSERT(SUCCEEDED(res));

	//
	//	Get surface descriptor
	//

	D3DSURFACE_DESC desc;
	res = m_pCaptureSurface->GetDesc(&desc);
	ATLASSERT(SUCCEEDED(res));

	//
	//	Set return vars
	//

	pCptImageDesc->pBits = (BYTE *)(rectSurface.pBits);
	pCptImageDesc->nPitch = rectSurface.Pitch;
	pCptImageDesc->nWidth = desc.Width;
	pCptImageDesc->nHeight = desc.Height;
	pCptImageDesc->nBPP = 4; // A8R8G8B8 -- TODO: Determine this programatically

	return S_OK;
}

HRESULT CDxCapture::InitD3D()
{
	// Create D3D object
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	ATLASSERT(m_pDirect3D);

	// Init parameters
	D3DPRESENT_PARAMETERS d3DPParam;
	ZeroMemory(&d3DPParam, sizeof(d3DPParam));

	d3DPParam.Windowed = TRUE;
	d3DPParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// Create window if necessary
	if (!m_hWnd)
	{
		// Register window class
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = _T("AnrWindowClass");

		RegisterClassEx(&wc);

		// Create dummy window
		m_hWnd = CreateWindow(_T("AnrWindowClass"),
							  _T("Anreal Dummy Window"),
							  WS_OVERLAPPEDWINDOW,
							  0,
							  0,
							  320,
							  240,
							  NULL,
							  NULL,
							  NULL,
							  NULL);
	}

	// Create device
	HRESULT res = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											m_hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3DPParam,
											&m_pD3DDevice);

	ATLASSERT(SUCCEEDED(res));
	return res;
}

HRESULT CDxCapture::CloseD3D()
{
	m_pD3DDevice->Release();
	m_pDirect3D->Release();

	return S_OK;
}

HRESULT CDxCapture::CreateSurface()
{
	ATLASSERT(m_pD3DDevice);

	D3DDISPLAYMODE ddm;
	HRESULT res = m_pD3DDevice->GetDisplayMode(0, &ddm);
	ATLASSERT(SUCCEEDED(res));

	// TODO: Use CptParam.rcCapture!
	res = m_pD3DDevice->CreateOffscreenPlainSurface(ddm.Width,
													ddm.Height,
													D3DFMT_A8R8G8B8,
													D3DPOOL_SYSTEMMEM,
													&m_pCaptureSurface,
													NULL);

	ATLASSERT(SUCCEEDED(res));
	return res;
}

HRESULT CDxCapture::ReleaseSurface()
{
	m_pCaptureSurface->UnlockRect();
	m_pCaptureSurface->Release();

	return S_OK;
}