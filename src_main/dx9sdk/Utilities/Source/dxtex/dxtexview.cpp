// dxtexView.cpp : implementation of the CDxtexView class
//

#include "stdafx.h"
#include "dxtex.h"
#include "formats.h"
#include "dxtexDoc.h"
#include "dxtexView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;
    FLOAT rhw;
    DWORD color;
    FLOAT tu, tv;   // The texture coordinates
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


IMPLEMENT_DYNCREATE(CDxtexView, CScrollView)

BEGIN_MESSAGE_MAP(CDxtexView, CScrollView)
    //{{AFX_MSG_MAP(CDxtexView)
    ON_WM_LBUTTONUP()
    ON_COMMAND(ID_FILE_OPENSUBSURFACE, OnFileOpenSubsurface)
    ON_COMMAND(ID_FILE_OPENALPHASUBSURFACE, OnFileOpenAlphaSubsurface)
    ON_COMMAND(ID_FILE_OPENFACE, OnFileOpenFace)
    ON_COMMAND(ID_FILE_OPENALPHAFACE, OnFileOpenAlphaFace)
    ON_COMMAND(ID_VIEW_ORIGINAL, OnViewOriginal)
    ON_COMMAND(ID_VIEW_COMPRESSED, OnViewCompressed)
    ON_COMMAND(ID_VIEW_SMALLERMIPLEVEL, OnViewSmallerMipLevel)
    ON_COMMAND(ID_VIEW_LARGERMIPLEVEL, OnViewLargerMipLevel)
    ON_COMMAND(ID_VIEW_ALPHACHANNEL, OnViewAlphaChannel)
    ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomIn)
    ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomOut)
    ON_COMMAND(ID_VIEW_CHANGEBACKGROUNDCOLOR, OnViewChangeBackgroundColor)
    ON_COMMAND(ID_VIEW_NEGX, OnViewNegX)
    ON_COMMAND(ID_VIEW_POSX, OnViewPosX)
    ON_COMMAND(ID_VIEW_NEGY, OnViewNegY)
    ON_COMMAND(ID_VIEW_POSY, OnViewPosY)
    ON_COMMAND(ID_VIEW_NEGZ, OnViewNegZ)
    ON_COMMAND(ID_VIEW_POSZ, OnViewPosZ)
    ON_COMMAND(ID_VIEW_HIGHERVOLUMESLICE, OnViewHigherVolumeSlice)
    ON_COMMAND(ID_VIEW_LOWERVOLUMESLICE, OnViewLowerVolumeSlice)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPENSUBSURFACE, OnUpdateFileOpenSubsurface)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPENALPHASUBSURFACE, OnUpdateFileOpenAlphaSubsurface)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPENFACE, OnUpdateFileOpenFace)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPENALPHAFACE, OnUpdateFileOpenAlphaFace)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ORIGINAL, OnUpdateViewOriginal)
    ON_UPDATE_COMMAND_UI(ID_VIEW_COMPRESSED, OnUpdateViewCompressed)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ALPHACHANNEL, OnUpdateViewAlphaChannel)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LARGERMIPLEVEL, OnUpdateViewLargerMipLevel)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SMALLERMIPLEVEL, OnUpdateViewSmallerMipLevel)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomIn)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomOut)
    ON_UPDATE_COMMAND_UI(ID_VIEW_NEGX, OnUpdateViewNegX)
    ON_UPDATE_COMMAND_UI(ID_VIEW_POSX, OnUpdateViewPosX)
    ON_UPDATE_COMMAND_UI(ID_VIEW_NEGY, OnUpdateViewNegY)
    ON_UPDATE_COMMAND_UI(ID_VIEW_POSY, OnUpdateViewPosY)
    ON_UPDATE_COMMAND_UI(ID_VIEW_NEGZ, OnUpdateViewNegZ)
    ON_UPDATE_COMMAND_UI(ID_VIEW_POSZ, OnUpdateViewPosZ)
    ON_UPDATE_COMMAND_UI(ID_VIEW_HIGHERVOLUMESLICE, OnUpdateViewHigherVolumeSlice)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LOWERVOLUMESLICE, OnUpdateViewLowerVolumeSlice)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT GetSurfaceSize(D3DSURFACE_DESC* pDesc)
{
    UINT nSurfaceSize = 0;

    for( int i=0; i < fmtInfoArraySize; i++ )
    {
        if( fmtInfoArray[i].fmt == pDesc->Format )
        {
            nSurfaceSize = pDesc->Width * 
                           pDesc->Height * 
                           fmtInfoArray[i].nBitsPerTexel / sizeof(BYTE);
                                    
            break;
        }
    }

    return nSurfaceSize;
}

static UINT GetVolumeSize(D3DVOLUME_DESC* pDesc)
{
    UINT nSurfaceSize = 0;

    for( int i=0; i < fmtInfoArraySize; i++ )
    {
        if( fmtInfoArray[i].fmt == pDesc->Format )
        {
            nSurfaceSize = pDesc->Width * 
                           pDesc->Height *
                           pDesc->Depth *
                           fmtInfoArray[i].nBitsPerTexel / sizeof(BYTE);

            break;
        }
    }

    return nSurfaceSize;
}


CDxtexView::CDxtexView()
{
    m_pVB = NULL;
    m_ptexCur = NULL;
    m_pSwapChain = NULL;
    m_lwMipCur = 0;
    m_CubeFaceCur = D3DCUBEMAP_FACE_FORCE_DWORD;
    m_lwSliceCur = -1;
    m_fZoom = 1.0f;
    m_bViewOrig = TRUE;
    m_bViewAlpha = FALSE;
}


CDxtexView::~CDxtexView()
{
    ReleasePpo(&m_pVB);
    ReleasePpo(&m_ptexCur);
    ReleasePpo(&m_pSwapChain);
}



void CDxtexView::CheckScrollBars(BOOL& bHasHorzBar, BOOL& bHasVertBar) const
{
    DWORD dwStyle = GetStyle();
    CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
    bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
                    (dwStyle & WS_VSCROLL);

    pBar = GetScrollBarCtrl(SB_HORZ);
    bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
                    (dwStyle & WS_HSCROLL);
}

// Note: repaints don't require re-rendering, just recopy from back buffer to view's DC
void CDxtexView::OnDraw(CDC* pDC)
{
    CDxtexDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    HRESULT hr;
    CRect rcSrc;
    CRect rcDest;

    rcSrc = m_rcSrc;
    rcDest = m_rcDest;

    rcDest.OffsetRect(pDC->GetViewportOrg());

    // REVIEW: only update dirty region?
    if (m_pSwapChain != NULL)
    {
        if( PDxtexApp()->HandlePossibleLostDevice() )
        {
            hr = m_pSwapChain->Present(&rcSrc, &rcDest, GetSafeHwnd(), NULL, 0);
            if( hr == D3DERR_DEVICELOST )
            {
                PDxtexApp()->DeviceIsLost();
                Invalidate();
            }
        }
    }
}


#ifdef _DEBUG
void CDxtexView::AssertValid() const
{
    CScrollView::AssertValid();
}


void CDxtexView::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}


CDxtexDoc* CDxtexView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDxtexDoc)));
    return (CDxtexDoc*)m_pDocument;
}
#endif //_DEBUG


void CDxtexView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // Button click means toggle compressed / uncompressed view
    if (m_bViewOrig)
        OnViewCompressed();
    else
        OnViewOriginal();

    CScrollView::OnLButtonUp(nFlags, point);
}


void CDxtexView::OnInitialUpdate()
{
#ifdef _WIN64
    SetClassLongPtr(GetSafeHwnd(), GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(100, 100, 120)));
#else
    SetClassLong(GetSafeHwnd(), GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(100, 100, 120)));
#endif

    if (GetDocument()->IsCubeMap())
        m_CubeFaceCur = D3DCUBEMAP_FACE_POSITIVE_X;
    else
        m_CubeFaceCur = D3DCUBEMAP_FACE_FORCE_DWORD;
    if (GetDocument()->IsVolumeMap())
        m_lwSliceCur = 0;
    BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    UpdateDevice();
    CreateVertexBuffer();
    RenderScene();

    SetScrollSizes(MM_TEXT, CSize(m_rcDest.Width(), m_rcDest.Height()));
    ResizeParentToFit();

    CScrollView::OnInitialUpdate();

    m_bTitleModsChanged = TRUE; // force title bar update
}


VOID CDxtexView::GetImageInfo(CString& strInfo)
{
    LPDIRECT3DBASETEXTURE9 ptex;
    D3DSURFACE_DESC sd;
    D3DVOLUME_DESC vd;
    D3DFORMAT fmt;
    DWORD dwWidth;
    DWORD dwHeight;
    DWORD dwDepth;
    CString strFormat;
    TCHAR sz[100];
    DWORD dwBytes = 0;
    UINT iLevel;

    if (m_bViewOrig)
        ptex = GetDocument()->PtexOrig();
    else
        ptex = GetDocument()->PtexNew();

    if (GetDocument()->IsVolumeMap())
    {
        ((LPDIRECT3DVOLUMETEXTURE9)ptex)->GetLevelDesc(0, &vd);
        fmt = vd.Format;
        dwWidth = vd.Width;
        dwHeight = vd.Height;
        dwDepth = vd.Depth;
    }
    else if (!GetDocument()->IsCubeMap())
    {
        ((LPDIRECT3DTEXTURE9)ptex)->GetLevelDesc(0, &sd);
        fmt = sd.Format;
        dwWidth = sd.Width;
        dwHeight = sd.Height;
        dwDepth = 0;
    }
    else
    {
        ((LPDIRECT3DCUBETEXTURE9)ptex)->GetLevelDesc(0, &sd);
        fmt = sd.Format;
        dwWidth = sd.Width;
        dwHeight = sd.Height;
        dwDepth = 0;
    }

    strFormat = FormatName(fmt);

    // Count bytes in main surface chain
    if (GetDocument()->IsVolumeMap())
    {
        for (iLevel = 0; iLevel < GetDocument()->NumMips(); iLevel++)
        {
            ((LPDIRECT3DVOLUMETEXTURE9)ptex)->GetLevelDesc(iLevel, &vd);
            dwBytes += GetVolumeSize(&vd);
        }
    }
    else if (!GetDocument()->IsCubeMap())
    {
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_FORCE_DWORD, ptex);
    }
    else
    {
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_POSITIVE_X, ptex);
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_NEGATIVE_X, ptex);
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_POSITIVE_Y, ptex);
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_NEGATIVE_Y, ptex);
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_POSITIVE_Z, ptex);
        dwBytes += NumBytesInSurfaces(D3DCUBEMAP_FACE_NEGATIVE_Z, ptex);
    }

    if (dwDepth == 0)
        StringCchPrintf(sz, 100, "%d x %d, %s, %d bytes", dwWidth, dwHeight, (LPCTSTR)strFormat, dwBytes);
    else
        StringCchPrintf(sz, 100, "%d x %d x %d, %s, %d bytes", dwWidth, dwHeight, dwDepth, (LPCTSTR)strFormat, dwBytes);
    strInfo = sz;
}


DWORD CDxtexView::NumBytesInSurfaces(D3DCUBEMAP_FACES FaceType, LPDIRECT3DBASETEXTURE9 ptex)
{
    DWORD dwBytes = 0;
    LPDIRECT3DTEXTURE9 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE9 pcubetex = NULL;
    D3DSURFACE_DESC sd;
    DWORD iLevel;

    if (FaceType == D3DCUBEMAP_FACE_FORCE_DWORD)
        pmiptex = (LPDIRECT3DTEXTURE9)ptex;
    else
        pcubetex = (LPDIRECT3DCUBETEXTURE9)ptex;

    for (iLevel = 0; iLevel < GetDocument()->NumMips(); iLevel++)
    {
        if (pmiptex != NULL)
            pmiptex->GetLevelDesc(iLevel, &sd);
        else
            pcubetex->GetLevelDesc(iLevel, &sd);
        dwBytes += GetSurfaceSize(&sd);
    }

    return dwBytes;
}


HRESULT CDxtexView::UpdateDevice(VOID)
{
    HRESULT hr;
    LPDIRECT3D9 pd3d = PDxtexApp()->Pd3d();
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    D3DDISPLAYMODE dispMode;
    D3DPRESENT_PARAMETERS presentParams;

    pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dispMode);

    ReleasePpo(&m_pSwapChain);
    ZeroMemory(&presentParams, sizeof(presentParams));
    presentParams.Windowed = TRUE;
    presentParams.SwapEffect = D3DSWAPEFFECT_COPY;
    presentParams.BackBufferWidth = m_rcSrc.Width();
    presentParams.BackBufferHeight = m_rcSrc.Height();
    presentParams.BackBufferFormat = dispMode.Format;

    if (FAILED(hr = pd3ddev->CreateAdditionalSwapChain(&presentParams, &m_pSwapChain)))
        return hr;

    COLORREF crBkgd;
    crBkgd = PDxtexApp()->GetProfileInt("Settings", "Background Color", RGB(0, 255, 255));
    m_dwClearColor = D3DCOLOR_RGBA(GetRValue(crBkgd), GetGValue(crBkgd), GetBValue(crBkgd), 255);

    return S_OK;
}

HRESULT CDxtexView::InvalidateDeviceObjects(VOID)
{
    ReleasePpo( &m_pSwapChain );
    return S_OK;
}

HRESULT CDxtexView::RestoreDeviceObjects(VOID)
{
    UpdateDevice();
    RenderScene();
    Invalidate();
    return S_OK;
}

HRESULT CDxtexView::CreateVertexBuffer(VOID)
{
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();

    // Create the the vertex buffer
    if( FAILED( pd3ddev->CreateVertexBuffer( 6 * sizeof(CUSTOMVERTEX),
        0 /* Usage */, D3DFVF_CUSTOMVERTEX, 
        D3DPOOL_MANAGED, &m_pVB, NULL ) ) )
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT CDxtexView::RenderScene(VOID)
{
    CWaitCursor waitCursor;
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();

    LPDIRECT3DSURFACE9 psurf;

    if (m_pSwapChain == NULL)
        return E_FAIL;

    // Vertices for our quad
    CUSTOMVERTEX vertexArray[] =
    {
        // x, y, z, rhw, color
        { -0.5f,                     -0.5f,                      0.5f, 1.0f, 0xffffffff, 0.0, 0.0, },
        { (FLOAT)m_rcSrc.right-0.5f, -0.5f,                      0.5f, 1.0f, 0xffffffff, 1.0, 0.0, },
        { (FLOAT)m_rcSrc.right-0.5f, (FLOAT)m_rcSrc.bottom-0.5f, 0.5f, 1.0f, 0xffffffff, 1.0, 1.0, },

        { (FLOAT)m_rcSrc.right-0.5f, (FLOAT)m_rcSrc.bottom-0.5f, 0.5f, 1.0f, 0xffffffff, 1.0, 1.0, },
        { -0.5f,                     (FLOAT)m_rcSrc.bottom-0.5f, 0.5f, 1.0f, 0xffffffff, 0.0, 1.0, },
        { -0.5f,                     -0.5f,                      0.5f, 1.0f, 0xffffffff, 0.0, 0.0, },
    };

    // Copy the global vertex data into the vertex buffer
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, sizeof(vertexArray), (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, vertexArray, sizeof(vertexArray) );
    m_pVB->Unlock();

    hr = m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &psurf);

    hr = pd3ddev->SetRenderTarget(0, psurf);

    ReleasePpo(&psurf);

    hr = pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET, m_dwClearColor, 0.0f, 0);

    hr = pd3ddev->BeginScene();

    // If the texture uses premultiplied alpha, the source blend should be D3DBLEND_ONE
    // since RGB is already at the level we want.  With nonpremultiplied alpha, the
    // source blend should be D3DBLEND_SRCALPHA.
    D3DSURFACE_DESC sd;
    m_ptexCur->GetLevelDesc(0, &sd);
    if (!m_bViewAlpha && (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4))
    {
        if (FAILED(hr = pd3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE)))
            return hr;
    }
    else
    {
        if (FAILED(hr = pd3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
            return hr;
    }
    hr = pd3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    hr = pd3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    pd3ddev->SetTexture(0, m_ptexCur);
    pd3ddev->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
    pd3ddev->SetFVF( D3DFVF_CUSTOMVERTEX );
    pd3ddev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
    pd3ddev->SetTexture(0, NULL);

    hr = pd3ddev->EndScene();

    return S_OK;
}


void CDxtexView::OnViewOriginal()
{
    if (GetDocument()->PtexOrig() == NULL)
        return;
    BuildViewSurface(TRUE, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    RenderScene();
    m_bTitleModsChanged = TRUE; // force title bar update
    InvalidateRect(&m_rcDest, FALSE); // force redraw of this view
}


void CDxtexView::OnViewCompressed()
{
    if (GetDocument()->PtexNew() == NULL)
        return;
    BuildViewSurface(FALSE, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    RenderScene();
    m_bTitleModsChanged = TRUE; // force title bar update
    InvalidateRect(&m_rcDest, FALSE); // force redraw of this view
}


void CDxtexView::OnUpdateViewOriginal(CCmdUI* pCmdUI)
{
    if (GetDocument()->PtexOrig() == NULL)
    {
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(0);
    }
    else
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(m_bViewOrig);
    }
}


void CDxtexView::OnUpdateViewCompressed(CCmdUI* pCmdUI)
{
    if (GetDocument()->PtexNew() == NULL)
    {
        pCmdUI->Enable(FALSE);
        pCmdUI->SetCheck(0);
    }
    else
    {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(!m_bViewOrig);
    }
}


void CDxtexView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    m_bTitleModsChanged = TRUE; // force title bar update
    if (lHint == 1)
    {
        BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
        RenderScene();
    }
    else if (lHint == 2)
    {
        UpdateDevice();
        RenderScene();
    }
    else if (lHint == 3)
    {
        RenderScene();
    }
    else if( lHint == 4)
    {
        BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
        UpdateDevice();
        RenderScene();
        SetScrollSizes(MM_TEXT, CSize(m_rcDest.Width(), m_rcDest.Height()));
        ResizeParentToFit();
    }

    CScrollView::OnUpdate(pSender, lHint, pHint);
}


void CDxtexView::OnViewSmallerMipLevel()
{
    m_lwMipCur++;
    if (m_lwSliceCur > 0)
        m_lwSliceCur /= 2;
    BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    UpdateDevice();
    RenderScene();
    m_bTitleModsChanged = TRUE; // force title bar update
    SetScrollSizes(MM_TEXT, CSize(m_rcDest.Width(), m_rcDest.Height()));
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnViewLargerMipLevel()
{
    m_lwMipCur--;
    if (m_lwSliceCur > 0)
        m_lwSliceCur *= 2;
    BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    UpdateDevice();
    RenderScene();
    m_bTitleModsChanged = TRUE; // force title bar update
    SetScrollSizes(MM_TEXT, CSize(m_rcDest.Width(), m_rcDest.Height()));
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnViewAlphaChannel(VOID)
{
    BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, !m_bViewAlpha);
    RenderScene();
    Invalidate(); // force redraw of this view
    m_bTitleModsChanged = TRUE; // force title bar update
}


void CDxtexView::OnUpdateViewAlphaChannel(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_bViewAlpha);
}


void CDxtexView::OnUpdateViewLargerMipLevel(CCmdUI* pCmdUI)
{
    if (m_lwMipCur > 0)
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CDxtexView::OnUpdateViewSmallerMipLevel(CCmdUI* pCmdUI)
{
    if (m_lwMipCur < (LONG)GetDocument()->NumMips() - 1)
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CDxtexView::OnViewZoomIn()
{

#define ZOOMIN 2.0f

    if (m_fZoom < 8.0f)
        m_fZoom *= ZOOMIN;

    Zoom(ZOOMIN);
}


void CDxtexView::OnViewZoomOut()
{

#define ZOOMOUT 0.5f

    if (m_fZoom > 0.125f)
        m_fZoom *= ZOOMOUT;

    Zoom(ZOOMOUT);
}

void CDxtexView::Zoom(float zoomfactor)
{
    // get the current center position
    CPoint ptUpperLeft = GetScrollPosition();
    RECT windowRect;
    GetClientRect(&windowRect);
    CPoint ptCenter;
    ptCenter.x = ptUpperLeft.x + windowRect.right/2;
    ptCenter.y = ptUpperLeft.y + windowRect.bottom/2;

    m_rcDest.right = (LONG)(m_rcSrc.right * m_fZoom);
    m_rcDest.bottom = (LONG)(m_rcSrc.bottom * m_fZoom);
    SetScrollSizes(MM_TEXT, CSize(m_rcDest.Width(), m_rcDest.Height()));

	BOOL h, v;

	CheckScrollBars(h, v);

	if (h)
	{
	    ptCenter.x = (LONG)(ptCenter.x * zoomfactor);
		ptUpperLeft.x = __max(ptCenter.x - windowRect.right/2, 0);
	}

	if (v)
	{
		ptCenter.y = (LONG)(ptCenter.y * zoomfactor);
		ptUpperLeft.y = __max(ptCenter.y - windowRect.bottom/2, 0);
	}

	if (h || v)
	{
	    ScrollToPosition(ptUpperLeft);
	}

    m_bTitleModsChanged = TRUE; // force title bar update
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewZoomIn(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_fZoom < 8.0f);
}


void CDxtexView::OnUpdateViewZoomOut(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_fZoom > 0.125f);
}


CString CDxtexView::GetStrTitleMods(VOID)
{
    CString strTitleMods;
    strTitleMods = "(";

    // Append alpha, if in alpha mode
    if (m_bViewAlpha)
            strTitleMods += "Alpha, ";

    // Show format
    LPDIRECT3DBASETEXTURE9 ptex;
    CString strFormat;
    D3DFORMAT fmt;

    if (m_bViewOrig)
        ptex = GetDocument()->PtexOrig();
    else
        ptex = GetDocument()->PtexNew();

    if (GetDocument()->IsVolumeMap())
    {
        D3DVOLUME_DESC vd;
        ((LPDIRECT3DVOLUMETEXTURE9)ptex)->GetLevelDesc(0, &vd);
        fmt = vd.Format;
    }
    else if (!GetDocument()->IsCubeMap())
    {
        D3DSURFACE_DESC sd;
        ((LPDIRECT3DTEXTURE9)ptex)->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    else
    {
        D3DSURFACE_DESC sd;
        ((LPDIRECT3DCUBETEXTURE9)ptex)->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }

    strTitleMods += FormatName(fmt);
    strTitleMods += TEXT(", ");

    // Append cube map info, if a cube map
    switch (m_CubeFaceCur)
    {
    case D3DCUBEMAP_FACE_NEGATIVE_X:
        strTitleMods += "Negative X, ";
        break;
    case D3DCUBEMAP_FACE_POSITIVE_X:
        strTitleMods += "Positive X, ";
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_Y:
        strTitleMods += "Negative Y, ";
        break;
    case D3DCUBEMAP_FACE_POSITIVE_Y:
        strTitleMods += "Positive Y, ";
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_Z:
        strTitleMods += "Negative Z, ";
        break;
    case D3DCUBEMAP_FACE_POSITIVE_Z:
        strTitleMods += "Positive Z, ";
        break;
    }

    if (m_lwSliceCur >= 0)
    {
        CString strSlice;
        strSlice.Format("Slice %d of %d, ", m_lwSliceCur + 1, GetDocument()->DwDepthAt(m_lwMipCur));
        strTitleMods += strSlice;
    }

    // Append mip info, if multiple mip levels
    DWORD dwNumMips = GetDocument()->NumMips();
    if (dwNumMips > 1)
    {
        CString strMipInfo;
        strMipInfo.Format("Mip %d of %d, ", m_lwMipCur + 1, dwNumMips);
        strTitleMods += strMipInfo;
    }

    // Append view magnification
    CString strView;
    strView.Format("%d", (LONG)(100 * m_fZoom));
    strTitleMods += strView + "%";

    strTitleMods += ")";
    return strTitleMods;
}


CString CDxtexView::FormatName(D3DFORMAT fmt)
{
    CString str = TEXT("Unknown fmt");

    for( int i=0; i < fmtInfoArraySize; i++ )
    {
        if( fmtInfoArray[i].fmt == fmt )
        {
            str = fmtInfoArray[i].strName;
            break;
        }
    }
    
    return str;
}

void CDxtexView::OnViewChangeBackgroundColor()
{
    CHOOSECOLOR cc;
    COLORREF crArray[16];

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = GetSafeHwnd();
    cc.rgbResult = PDxtexApp()->GetProfileInt("Settings", "Background Color", RGB(0, 255, 255));
    cc.lpCustColors = crArray;
    cc.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;

    if (ChooseColor(&cc))
    {
        PDxtexApp()->WriteProfileInt("Settings", "Background Color", cc.rgbResult);

        // Update all views of all documents of our one doc template
        POSITION posTemp = PDxtexApp()->GetFirstDocTemplatePosition();
        CDocTemplate* pDocTemplate = PDxtexApp()->GetNextDocTemplate(posTemp);
        CDocument* pdoc;
        POSITION pos = pDocTemplate->GetFirstDocPosition();
        while (pos != NULL)
        {
            pdoc = pDocTemplate->GetNextDoc(pos);
            pdoc->UpdateAllViews(NULL, 2);
        }
    }
}


void CDxtexView::OnFileOpenSubsurface()
{
    GetDocument()->OpenSubsurface(m_CubeFaceCur, m_lwMipCur, m_lwSliceCur);
}


void CDxtexView::OnUpdateFileOpenSubsurface(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}


void CDxtexView::OnFileOpenAlphaSubsurface()
{
    GetDocument()->OpenAlphaSubsurface(m_CubeFaceCur, m_lwMipCur, m_lwSliceCur);
}


void CDxtexView::OnUpdateFileOpenAlphaSubsurface(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}


HRESULT CDxtexView::BuildViewSurface(BOOL bOrig, D3DCUBEMAP_FACES FaceType, LONG lwSlice, LONG lwMip, BOOL bViewAlpha)
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DBASETEXTURE9 ptex;
    BOOL bIsCubeMap = GetDocument()->IsCubeMap();
    BOOL bIsVolumeMap = GetDocument()->IsVolumeMap();
    D3DSURFACE_DESC sd;
    D3DVOLUME_DESC vd;

    ReleasePpo(&m_ptexCur);

    if (bIsVolumeMap && lwSlice == -1)
        lwSlice = 0;

    if (bIsCubeMap && FaceType == D3DCUBEMAP_FACE_FORCE_DWORD)
        FaceType = D3DCUBEMAP_FACE_POSITIVE_X;

    m_bViewOrig = bOrig;
    m_bViewAlpha = bViewAlpha;
    m_lwSliceCur = lwSlice;
    m_lwMipCur = lwMip;
    m_CubeFaceCur = FaceType;

    if (bOrig)
        ptex = GetDocument()->PtexOrig();
    else
        ptex = GetDocument()->PtexNew();

    if (bIsVolumeMap)
    {
        ((LPDIRECT3DVOLUMETEXTURE9)ptex)->GetLevelDesc(m_lwMipCur, &vd);
        sd.Width = vd.Width;
        sd.Height = vd.Height;
    }
    else if (!bIsCubeMap)
    {
        ((LPDIRECT3DTEXTURE9)ptex)->GetLevelDesc(m_lwMipCur, &sd);
    }
    else
    {
        ((LPDIRECT3DCUBETEXTURE9)ptex)->GetLevelDesc(m_lwMipCur, &sd);
    }

    hr = pd3ddev->CreateTexture(sd.Width, sd.Height, 1,
         0 /* Usage */, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_ptexCur, NULL);
    if (FAILED(hr))
        return hr;

    m_rcSrc.SetRect(0, 0, sd.Width, sd.Height);
    m_rcDest.SetRect(0, 0, (INT)(sd.Width * m_fZoom), (INT)(sd.Height * m_fZoom));

    LPDIRECT3DSURFACE9 psurfSrc = NULL;
    LPDIRECT3DSURFACE9 psurfDest = NULL;

    hr = m_ptexCur->GetSurfaceLevel(0, &psurfDest);

    if (bIsVolumeMap)
    {
        LPDIRECT3DVOLUME9 pvolSrc;
        hr = ((LPDIRECT3DVOLUMETEXTURE9)ptex)->GetVolumeLevel(m_lwMipCur, &pvolSrc);
        hr = LoadSurfaceFromVolumeSlice(pvolSrc, m_lwSliceCur, psurfDest);
        ReleasePpo(&pvolSrc);
    }
    else if (!bIsCubeMap)
    {
        hr = ((LPDIRECT3DTEXTURE9)ptex)->GetSurfaceLevel(m_lwMipCur, &psurfSrc);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL,
            D3DX_FILTER_TRIANGLE, 0);
    }
    else
    {
        hr = ((LPDIRECT3DCUBETEXTURE9)ptex)->GetCubeMapSurface(FaceType, m_lwMipCur, &psurfSrc);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL,
            D3DX_FILTER_TRIANGLE, 0);
    }


    if (bViewAlpha)
    {
        // Move alpha channels into RGB (and set alpha to 0xff)
        D3DLOCKED_RECT lr;

        hr = psurfDest->LockRect(&lr, NULL, 0);

        DWORD xp;
        DWORD yp;
        DWORD* pdwRow = (DWORD*)lr.pBits;
        DWORD* pdw;
        DWORD dwAlpha;
        LONG dataBytesPerRow = 4 * sd.Width;

        for (yp = 0; yp < sd.Height; yp++)
        {
            pdw = pdwRow;
            for (xp = 0; xp < sd.Width; xp++)
            {
                dwAlpha = *pdw >> 24;
                *pdw = 0xff000000 | (dwAlpha << 16) | (dwAlpha << 8) | (dwAlpha);
                pdw++;
            }
            pdwRow += lr.Pitch / 4;
        }
        psurfDest->UnlockRect();
    }

    ReleasePpo(&psurfSrc);
    ReleasePpo(&psurfDest);

    return S_OK;
}


HRESULT CDxtexView::LoadSurfaceFromVolumeSlice(LPDIRECT3DVOLUME9 pVolume, UINT iSlice, LPDIRECT3DSURFACE9 psurf)
{
    HRESULT hr;
    D3DVOLUME_DESC vd;
    D3DLOCKED_BOX lb;
    D3DBOX box;
    RECT rc;

    pVolume->GetDesc(&vd);

    box.Left = 0;
    box.Right = vd.Width;
    box.Top = 0;
    box.Bottom = vd.Height;
    box.Front = iSlice;
    box.Back = iSlice + 1;

    rc.left = 0;
    rc.right = vd.Width;
    rc.top = 0;
    rc.bottom = vd.Height;

    hr = pVolume->LockBox(&lb, &box, 0);
    if (FAILED(hr))
        return hr;

    hr = D3DXLoadSurfaceFromMemory(psurf, NULL, NULL, lb.pBits, vd.Format, lb.RowPitch,
        NULL, &rc, D3DX_FILTER_TRIANGLE, 0);

    pVolume->UnlockBox();

    return hr;
}


void CDxtexView::OnViewNegX()
{
    BuildViewSurface(m_bViewOrig, D3DCUBEMAP_FACE_NEGATIVE_X, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    m_bTitleModsChanged = TRUE; // force title bar update
    RenderScene();
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewNegX(CCmdUI* pCmdUI)
{
    BOOL bEnable = GetDocument()->IsCubeMap();
    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_CubeFaceCur == D3DCUBEMAP_FACE_NEGATIVE_X);
}


void CDxtexView::OnViewPosX()
{
    BuildViewSurface(m_bViewOrig, D3DCUBEMAP_FACE_POSITIVE_X, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    m_bTitleModsChanged = TRUE; // force title bar update
    RenderScene();
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewPosX(CCmdUI* pCmdUI)
{
    BOOL bEnable = GetDocument()->IsCubeMap();
    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_CubeFaceCur == D3DCUBEMAP_FACE_POSITIVE_X);
}


void CDxtexView::OnViewNegY()
{
    BuildViewSurface(m_bViewOrig, D3DCUBEMAP_FACE_NEGATIVE_Y, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    m_bTitleModsChanged = TRUE; // force title bar update
    RenderScene();
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewNegY(CCmdUI* pCmdUI)
{
    BOOL bEnable = GetDocument()->IsCubeMap();
    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_CubeFaceCur == D3DCUBEMAP_FACE_NEGATIVE_Y);
}


void CDxtexView::OnViewPosY()
{
    BuildViewSurface(m_bViewOrig, D3DCUBEMAP_FACE_POSITIVE_Y, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    m_bTitleModsChanged = TRUE; // force title bar update
    RenderScene();
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewPosY(CCmdUI* pCmdUI)
{
    BOOL bEnable = GetDocument()->IsCubeMap();
    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_CubeFaceCur == D3DCUBEMAP_FACE_POSITIVE_Y);
}


void CDxtexView::OnViewNegZ()
{
    BuildViewSurface(m_bViewOrig, D3DCUBEMAP_FACE_NEGATIVE_Z, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    m_bTitleModsChanged = TRUE; // force title bar update
    RenderScene();
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewNegZ(CCmdUI* pCmdUI)
{
    BOOL bEnable = GetDocument()->IsCubeMap();
    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_CubeFaceCur == D3DCUBEMAP_FACE_NEGATIVE_Z);
}


void CDxtexView::OnViewPosZ()
{
    BuildViewSurface(m_bViewOrig, D3DCUBEMAP_FACE_POSITIVE_Z, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    m_bTitleModsChanged = TRUE; // force title bar update
    RenderScene();
    Invalidate(); // force redraw of this view
}


void CDxtexView::OnUpdateViewPosZ(CCmdUI* pCmdUI)
{
    BOOL bEnable = GetDocument()->IsCubeMap();
    pCmdUI->Enable(bEnable);
    pCmdUI->SetCheck(m_CubeFaceCur == D3DCUBEMAP_FACE_POSITIVE_Z);
}

void CDxtexView::OnFileOpenFace()
{
    GetDocument()->OpenCubeFace(m_CubeFaceCur);
}

void CDxtexView::OnUpdateFileOpenFace(CCmdUI* pCmdUI)
{
    BOOL bEnable = (m_CubeFaceCur != D3DCUBEMAP_FACE_FORCE_DWORD);
    pCmdUI->Enable(bEnable);
}

void CDxtexView::OnFileOpenAlphaFace()
{
    GetDocument()->OpenAlphaCubeFace(m_CubeFaceCur);
}

void CDxtexView::OnUpdateFileOpenAlphaFace(CCmdUI* pCmdUI)
{
    BOOL bEnable = (m_CubeFaceCur != D3DCUBEMAP_FACE_FORCE_DWORD);
    pCmdUI->Enable(bEnable);
}

void CDxtexView::OnViewHigherVolumeSlice()
{
    m_lwSliceCur++;
    BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    UpdateDevice();
    RenderScene();
    m_bTitleModsChanged = TRUE; // force title bar update
    Invalidate(); // force redraw of this view
}

void CDxtexView::OnUpdateViewHigherVolumeSlice(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetDocument()->IsVolumeMap() && m_lwSliceCur < (LONG)GetDocument()->DwDepthAt(m_lwMipCur) - 1);
}

void CDxtexView::OnViewLowerVolumeSlice()
{
    m_lwSliceCur--;
    BuildViewSurface(m_bViewOrig, m_CubeFaceCur, m_lwSliceCur, m_lwMipCur, m_bViewAlpha);
    UpdateDevice();
    RenderScene();
    m_bTitleModsChanged = TRUE; // force title bar update
    Invalidate(); // force redraw of this view
}

void CDxtexView::OnUpdateViewLowerVolumeSlice(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetDocument()->IsVolumeMap() && m_lwSliceCur > 0);
}

