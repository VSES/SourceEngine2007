// dxtexDoc.cpp : implementation of the CDxtexDoc class
//

#include "stdafx.h"
#include "dxtex.h"
#include "dxtexDoc.h"
#include "dialogs.h"
#include "formats.h"
#include "dds.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Helper function that tells whether the given D3DFMT has a working alpha channel
BOOL FormatContainsAlpha( D3DFORMAT fmt )
{
    BOOL bHasAlpha = FALSE;

    for( int i=0; i < fmtInfoArraySize; i++ )
    {
        if( fmtInfoArray[i].fmt == fmt )
        {
            bHasAlpha = fmtInfoArray[i].bHasAlpha;
            break;
        }
    }

    return bHasAlpha;
}

/////////////////////////////////////////////////////////////////////////////
// CDxtexDoc

IMPLEMENT_DYNCREATE(CDxtexDoc, CDocument)

BEGIN_MESSAGE_MAP(CDxtexDoc, CDocument)
    //{{AFX_MSG_MAP(CDxtexDoc)
    ON_COMMAND(ID_FILE_OPENALPHA, OnFileOpenAlpha)
    ON_COMMAND(ID_FORMAT_GENERATEMIPMAPS, OnGenerateMipMaps)
    ON_COMMAND(ID_FORMAT_CHANGESURFACEFMT, OnFormatChangeSurfaceFmt)
    ON_COMMAND(ID_FORMAT_CHANGECUBEMAPFACES, OnFormatChangeCubeMapFaces)
    ON_COMMAND(ID_FORMAT_MAKEINTOVOLUMEMAP, OnFormatMakeIntoVolumeMap)
    ON_COMMAND(ID_FORMAT_RESIZE, OnFormatResize)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPENALPHA, OnUpdateFileOpenAlpha)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_GENERATEMIPMAPS, OnUpdateFormatGenerateMipmaps)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_CHANGECUBEMAPFACES, OnUpdateFormatChangeCubeMapFaces)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_MAKEINTOVOLUMEMAP, OnUpdateFormatMakeIntoVolumeMap)
    ON_UPDATE_COMMAND_UI(ID_FORMAT_RESIZE, OnUpdateFormatResize)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxtexDoc diagnostics

#ifdef _DEBUG
void CDxtexDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CDxtexDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDxtexDoc construction/destruction

CDxtexDoc::CDxtexDoc()
{
    m_ptexOrig = NULL;
    m_ptexNew = NULL;
    m_dwWidth = 0;
    m_dwHeight = 0;
    m_dwDepth = 0;
    m_numMips = 0;
    m_dwCubeMapFlags = 0;
    m_bTitleModsChanged = FALSE;
}


CDxtexDoc::~CDxtexDoc()
{
    ReleasePpo(&m_ptexOrig);
    ReleasePpo(&m_ptexNew);
}


BOOL CDxtexDoc::OnNewDocument()
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();

    if (!CDocument::OnNewDocument())
        return FALSE;

    CNewTextureDlg dlg;

    // Obtain maximum texture dimension
    D3DCAPS9 d3dcaps;
    if ( SUCCEEDED( pd3ddev->GetDeviceCaps( &d3dcaps ) ) )
    {
        dlg.SetMaxTextureSize( d3dcaps.MaxTextureWidth, d3dcaps.MaxTextureHeight );
    }

    if (IDCANCEL == dlg.DoModal())
        return FALSE;

    m_dwWidth = dlg.m_dwWidth;
    m_dwHeight = dlg.m_dwHeight;
    m_numMips = dlg.m_numMips;

    if (dlg.m_iTexType == 0)
    {
        LPDIRECT3DTEXTURE9 pmiptex;
        hr = pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
            0, dlg.m_fmt, D3DPOOL_MANAGED, &pmiptex, NULL);
        if (FAILED(hr))
        {
            AfxMessageBox(ID_ERROR_CANTCREATETEXTURE);
            return FALSE;
        }
        m_ptexOrig = pmiptex;

    }
    else if (dlg.m_iTexType == 1)
    {
        // Cube Map
        LPDIRECT3DCUBETEXTURE9 pcubetex;
        m_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
        hr = pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
            0, dlg.m_fmt, D3DPOOL_MANAGED, &pcubetex, NULL);
        if (FAILED(hr))
        {
            AfxMessageBox(ID_ERROR_CANTCREATETEXTURE);
            return FALSE;
        }
        m_ptexOrig = pcubetex;
    }
    else
    {
        LPDIRECT3DVOLUMETEXTURE9 pvoltex;
        m_dwDepth = dlg.m_dwDepth;
        hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, m_numMips, 
            0, dlg.m_fmt, D3DPOOL_SYSTEMMEM, &pvoltex, NULL);
        if (FAILED(hr))
        {
            AfxMessageBox(ID_ERROR_CANTCREATETEXTURE);
            return FALSE;
        }
        m_ptexOrig = pvoltex;
    }

    return TRUE;
}


BOOL CDxtexDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    D3DXIMAGE_INFO imageinfo;
    D3DXIMAGE_INFO imageinfo2;

    if( FAILED( D3DXGetImageInfoFromFile( lpszPathName, &imageinfo ) ) )
    {
        AfxMessageBox(ID_ERROR_COULDNTLOADFILE);
        return FALSE;
    }

    switch( imageinfo.ResourceType )
    {
    case D3DRTYPE_TEXTURE:
        if( FAILED( D3DXCreateTextureFromFileEx( pd3ddev, lpszPathName, 
            imageinfo.Width, imageinfo.Height, imageinfo.MipLevels, 0,
            imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 
            &imageinfo2, NULL, (LPDIRECT3DTEXTURE9*)&m_ptexOrig ) ) )
        {
            AfxMessageBox(ID_ERROR_COULDNTLOADFILE);
            return FALSE;
        }
        m_dwWidth = imageinfo2.Width;
        m_dwHeight = imageinfo2.Height;
        m_dwDepth = 0;
        m_numMips = imageinfo2.MipLevels;

        if( imageinfo.ImageFileFormat == D3DXIFF_BMP )
        {
            // Look for "foo_a.bmp" for alpha channel
            CString strPath = lpszPathName;
            int i = strPath.ReverseFind('.');
            HRESULT hr;
            strPath = strPath.Left(i) + "_a.bmp";
            CFileStatus status;
            if (CFile::GetStatus(strPath, status))
            {
                // Make sure there's an alpha channel to load alpha image into
                if (FAILED(EnsureAlpha(&m_ptexOrig)))
                    return FALSE;

                LPDIRECT3DSURFACE9 psurf;

                hr = ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetSurfaceLevel(0, &psurf);
                if (FAILED(hr))
                    return FALSE;

                hr = LoadAlphaIntoSurface(strPath, psurf);
                ReleasePpo(&psurf);
                if (FAILED(hr))
                    return FALSE;

            }
        }
        break;

    case D3DRTYPE_VOLUMETEXTURE:
        if( FAILED( D3DXCreateVolumeTextureFromFileEx( pd3ddev, lpszPathName, 
            imageinfo.Width, imageinfo.Height, imageinfo.Depth, imageinfo.MipLevels,
            0, imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
            0, &imageinfo2, NULL, (LPDIRECT3DVOLUMETEXTURE9*)&m_ptexOrig ) ) )
        {
            AfxMessageBox(ID_ERROR_COULDNTLOADFILE);
            return FALSE;
        }
        m_dwWidth = imageinfo2.Width;
        m_dwHeight = imageinfo2.Height;
        m_dwDepth = imageinfo2.Depth;
        m_numMips = imageinfo2.MipLevels;
        break;

    case D3DRTYPE_CUBETEXTURE:
        if( FAILED( D3DXCreateCubeTextureFromFileEx( pd3ddev, lpszPathName, 
            imageinfo.Width, imageinfo.MipLevels, 0, imageinfo.Format, 
            D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
            0, &imageinfo2, NULL, (LPDIRECT3DCUBETEXTURE9*)&m_ptexOrig ) ) )
        {
            AfxMessageBox(ID_ERROR_COULDNTLOADFILE);
            return FALSE;
        }
        m_dwWidth = imageinfo2.Width;
        m_dwHeight = imageinfo2.Height;
        m_dwDepth = 0;
        m_numMips = imageinfo2.MipLevels;
        m_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
        break;

    default:
        AfxMessageBox(ID_ERROR_COULDNTLOADFILE);
        return FALSE;
    }

    return TRUE;
}


BOOL CDxtexDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    LPDIRECT3DBASETEXTURE9 ptex;
    ptex = (m_ptexNew == NULL ? m_ptexOrig : m_ptexNew);
    
    if( FAILED( D3DXSaveTextureToFile( lpszPathName, D3DXIFF_DDS, ptex, NULL ) ) )
    {
        AfxMessageBox(ID_ERROR_COULDNTSAVEFILE);
        return FALSE;
    }
    
    SetModifiedFlag(FALSE);

    return TRUE;
}




D3DFORMAT CDxtexDoc::GetFormat(LPDIRECT3DBASETEXTURE9 ptex)
{
    LPDIRECT3DTEXTURE9 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE9 pcubetex = NULL;
    LPDIRECT3DVOLUMETEXTURE9 pvoltex = NULL;
    D3DFORMAT fmt = D3DFMT_UNKNOWN;

    if (IsVolumeMap())
        pvoltex = (LPDIRECT3DVOLUMETEXTURE9)ptex;
    else if (IsCubeMap())
        pcubetex = (LPDIRECT3DCUBETEXTURE9)ptex;
    else
        pmiptex = (LPDIRECT3DTEXTURE9)ptex;

    if (pvoltex != NULL)
    {
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmt = vd.Format;
    }
    else if (pcubetex != NULL)
    {
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    else if( pmiptex != NULL )
    {
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    return fmt;
}



// If *pptex's current format has less than 4 bits of alpha, change
// it to a similar format that has at least 4 bits of alpha.
HRESULT CDxtexDoc::EnsureAlpha(LPDIRECT3DBASETEXTURE9* pptex)
{
    HRESULT hr;
    D3DFORMAT fmtCur = GetFormat(*pptex);
    D3DFORMAT fmtNew = D3DFMT_UNKNOWN;
    LPDIRECT3DBASETEXTURE9 ptex = NULL;

    switch (fmtCur)
    {
    case D3DFMT_X8R8G8B8:
    case D3DFMT_R8G8B8:
        fmtNew = D3DFMT_A8R8G8B8;
        break;

    case D3DFMT_X1R5G5B5:
    case D3DFMT_R5G6B5:
        fmtNew = D3DFMT_A1R5G5B5;
        break;

    case D3DFMT_X8B8G8R8:
        fmtNew = D3DFMT_A8B8G8R8;
        break;

    case D3DFMT_L8:
        fmtNew = D3DFMT_A8L8;
        break;

    default:
        break;
    }

    if( fmtNew != D3DFMT_UNKNOWN )
    {
        if (FAILED(hr = ChangeFormat(m_ptexOrig, fmtNew, &ptex)))
            return hr;
        ReleasePpo(&m_ptexOrig);
        m_ptexOrig = ptex;
    }

    return S_OK;
}




/////////////////////////////////////////////////////////////////////////////
// CDxtexDoc commands

HRESULT CDxtexDoc::LoadAlphaBmp(CString& strPath)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pmiptex;
    LPDIRECT3DSURFACE9 psurf;

    if (IsCubeMap())
        return E_FAIL;

    pmiptex = (LPDIRECT3DTEXTURE9)m_ptexOrig;
    hr = pmiptex->GetSurfaceLevel(0, &psurf);
    if (FAILED(hr))
        return hr;

    hr = LoadAlphaIntoSurface(strPath, psurf);
    ReleasePpo(&psurf);
    if (FAILED(hr))
        return hr;
    
    UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
    return S_OK;
}




HRESULT CDxtexDoc::ChangeFormat(LPDIRECT3DBASETEXTURE9 ptexCur, D3DFORMAT fmtTo, 
                                LPDIRECT3DBASETEXTURE9* pptexNew)
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE9 pmiptex;
    LPDIRECT3DCUBETEXTURE9 pcubetex;
    LPDIRECT3DVOLUMETEXTURE9 pvoltex;
    D3DFORMAT fmtFrom;
    LPDIRECT3DTEXTURE9 pmiptexNew;
    LPDIRECT3DCUBETEXTURE9 pcubetexNew;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexNew;

    if (IsVolumeMap())
    {
        pvoltex = (LPDIRECT3DVOLUMETEXTURE9)ptexCur;
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmtFrom = vd.Format;
    }
    else if (IsCubeMap())
    {
        pcubetex = (LPDIRECT3DCUBETEXTURE9)ptexCur;
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmtFrom = sd.Format;
    }
    else
    {
        pmiptex = (LPDIRECT3DTEXTURE9)ptexCur;
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmtFrom = sd.Format;
    }

    if (fmtFrom == D3DFMT_DXT2 || fmtFrom == D3DFMT_DXT4)
    {
        if (fmtTo == D3DFMT_DXT1)
        {
            AfxMessageBox(ID_ERROR_PREMULTTODXT1);
        }
        else if (fmtTo != D3DFMT_DXT2 && fmtTo != D3DFMT_DXT4)
        {
            AfxMessageBox(ID_ERROR_PREMULTALPHA);
            return S_OK;
        }
    }

    if (IsVolumeMap())
    {
        hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, m_numMips,
            0, fmtTo, D3DPOOL_SYSTEMMEM, &pvoltexNew, NULL);
        if (FAILED(hr))
            return hr;
        *pptexNew = pvoltexNew;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, ptexCur, *pptexNew)))
            return hr;
    }
    else if (IsCubeMap())
    {
        hr = pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
             0, fmtTo, D3DPOOL_MANAGED, &pcubetexNew, NULL);
        if (FAILED(hr))
            return hr;
        *pptexNew = pcubetexNew;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_X, ptexCur, *pptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_X, ptexCur, *pptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Y, ptexCur, *pptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Y, ptexCur, *pptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Z, ptexCur, *pptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Z, ptexCur, *pptexNew)))
            return hr;
    }
    else
    {
        if ((fmtTo == D3DFMT_DXT1 || fmtTo == D3DFMT_DXT2 ||
            fmtTo == D3DFMT_DXT3 || fmtTo == D3DFMT_DXT4 ||
            fmtTo == D3DFMT_DXT5) && (m_dwWidth % 4 != 0 || m_dwHeight % 4 != 0))
        {
            AfxMessageBox(ID_ERROR_NEEDMULTOF4);
            return E_FAIL;
        }

        hr = pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
             0, fmtTo, D3DPOOL_MANAGED, &pmiptexNew, NULL);
        if (FAILED(hr))
            return hr;
        *pptexNew = pmiptexNew;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, ptexCur, *pptexNew)))
            return hr;
    }
    return S_OK;
}




HRESULT CDxtexDoc::Compress(D3DFORMAT fmtTo, BOOL bSwitchView)
{
    HRESULT hr;
    LPDIRECT3DBASETEXTURE9 ptexNew = NULL;

    if (FAILED(hr = ChangeFormat(m_ptexOrig, fmtTo, &ptexNew)))
        return hr;

    ReleasePpo(&m_ptexNew);
    m_ptexNew = ptexNew;

    SetModifiedFlag();
    m_bTitleModsChanged = TRUE; // force title bar update
    if (bSwitchView)
    {
        if( AfxGetMainWnd() != NULL )
            AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_VIEW_COMPRESSED, 0);
    }

    return S_OK;
}




void CDxtexDoc::OnGenerateMipMaps() 
{
    GenerateMipMaps();
}




void CDxtexDoc::GenerateMipMaps() 
{
    LONG lwTempH;
    LONG lwTempW;
    LONG lwPowsW;
    LONG lwPowsH;
    LPDIRECT3DTEXTURE9 pddsNew = NULL;
    D3DFORMAT fmt;
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE9 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE9 pcubetex = NULL;
    LPDIRECT3DVOLUMETEXTURE9 pvoltex = NULL;
    LPDIRECT3DTEXTURE9 pmiptexNew = NULL;
    LPDIRECT3DCUBETEXTURE9 pcubetexNew = NULL;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexNew = NULL;
    LPDIRECT3DSURFACE9 psurfSrc;
    LPDIRECT3DSURFACE9 psurfDest;
    LPDIRECT3DVOLUME9 pvolSrc;
    LPDIRECT3DVOLUME9 pvolDest;

    if (IsVolumeMap())
        pvoltex = (LPDIRECT3DVOLUMETEXTURE9)m_ptexOrig;
    else if (IsCubeMap())
        pcubetex = (LPDIRECT3DCUBETEXTURE9)m_ptexOrig;
    else
        pmiptex = (LPDIRECT3DTEXTURE9)m_ptexOrig;

    if (pvoltex != NULL)
    {
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmt = vd.Format;
    }
    else if (pcubetex != NULL)
    {
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    else
    {
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }

    lwTempW = m_dwWidth;
    lwTempH = m_dwHeight;
    lwPowsW = 0;
    lwPowsH = 0;
    while (lwTempW > 0)
    {
        lwPowsW++;
        lwTempW = lwTempW / 2;
    }
    while (lwTempH > 0)
    {
        lwPowsH++;
        lwTempH = lwTempH / 2;
    }
    m_numMips = lwPowsW > lwPowsH ? lwPowsW : lwPowsH;

    // Create destination mipmap surface - same format as source
    if (pvoltex != NULL)
    {
        if (FAILED(hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, 
            m_numMips, 0, fmt, D3DPOOL_SYSTEMMEM, &pvoltexNew, NULL)))
        {
            goto LFail;
        }
        hr = pvoltex->GetVolumeLevel(0, &pvolSrc);
        hr = pvoltexNew->GetVolumeLevel(0, &pvolDest);
        hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, pvolSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&pvolSrc);
        ReleasePpo(&pvolDest);
        hr = D3DXFilterVolumeTexture(pvoltexNew, NULL, 0, D3DX_DEFAULT);
    }
    else if (pmiptex != NULL)
    {
        if (FAILED(hr = pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
             0, fmt, D3DPOOL_MANAGED, &pmiptexNew, NULL)))
        {
            goto LFail;
        }
        hr = pmiptex->GetSurfaceLevel(0, &psurfSrc);
        hr = pmiptexNew->GetSurfaceLevel(0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = D3DXFilterTexture(pmiptexNew, NULL, 0, D3DX_DEFAULT);
    }
    else
    {
        if (FAILED(hr = pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
             0, fmt, D3DPOOL_MANAGED, &pcubetexNew, NULL)))
        {
            goto LFail;
        }
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = D3DXFilterCubeTexture(pcubetexNew, NULL, 0, D3DX_DEFAULT);
    }

    ReleasePpo(&m_ptexOrig);
    if (pvoltexNew != NULL)
        m_ptexOrig = pvoltexNew;
    else if (pcubetexNew != NULL)
        m_ptexOrig = pcubetexNew;
    else
        m_ptexOrig = pmiptexNew;

    if (m_ptexNew != NULL)
    {
        // Rather than filtering down the (probably-compressed) m_ptexNew 
        // top level, compress each mip level from the (probably-uncompressed)
        // m_ptexOrig levels.
        if (pvoltexNew != NULL)
        {
            D3DVOLUME_DESC vd;
            ((LPDIRECT3DVOLUMETEXTURE9)m_ptexNew)->GetLevelDesc(0, &vd);
            fmt = vd.Format;
        }
        else if (pcubetexNew != NULL)
        {
            D3DSURFACE_DESC sd;
            ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetLevelDesc(0, &sd);
            fmt = sd.Format;
        }
        else
        {
            D3DSURFACE_DESC sd;
            ((LPDIRECT3DCUBETEXTURE9)m_ptexNew)->GetLevelDesc(0, &sd);
            fmt = sd.Format;
        }
        Compress(fmt, FALSE);
    }

    m_bTitleModsChanged = TRUE; // Generate title bar update
    UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
    SetModifiedFlag();
    return;

LFail:
    ReleasePpo(&pddsNew);
}


void CDxtexDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
    CDocument::SetPathName(lpszPathName, bAddToMRU);

	DWORD lcid = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
    TCHAR* pszLeaf = strrchr(lpszPathName, '\\');
    if( pszLeaf )
    {
        pszLeaf++;
        TCHAR* pszExtension = strrchr(lpszPathName, '.');
        if( pszExtension && 
            CompareString( lcid, NORM_IGNORECASE, pszExtension, -1, TEXT(".dds"), -1 ) != CSTR_EQUAL )
        {
            StringCchCopy(pszExtension, 1, "");
            SetModifiedFlag(TRUE);
            SetTitle(pszLeaf);
            m_strPathName.Empty();
        }
    }
}

DWORD CDxtexDoc::NumMips(VOID)
{
    return m_numMips;
}


void CDxtexDoc::OnFileOpenAlpha() 
{
    HRESULT hr;
    CString fileName;
    LPDIRECT3DTEXTURE9 pmiptex;

    if (IsCubeMap() || IsVolumeMap())
        return;

    // Premultiplied-alpha files don't support this feature:
    D3DSURFACE_DESC sd;
    ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetLevelDesc(0, &sd);
    if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
    {
        AfxMessageBox(ID_ERROR_PREMULTALPHA);
        return;
    }

    // Check if the original has alpha
    if( !FormatContainsAlpha(sd.Format)  )
    {
        // If it doesn't then see if the new does
        if (m_ptexNew != NULL)
        {
            ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetLevelDesc(0, &sd);
            if( !FormatContainsAlpha(sd.Format) )
            {
                AfxMessageBox(ID_ERROR_NEEDALPHA);
                return;
            }
            else
            {
                ReleasePpo(&m_ptexOrig);
                m_ptexOrig = m_ptexNew;
                m_ptexNew  = NULL;
                CWnd* Wnd = AfxGetMainWnd();
                if( Wnd != NULL )
                    Wnd->PostMessage(WM_COMMAND, ID_VIEW_ORIGINAL, 0);
            }
        }
        else
        {
                AfxMessageBox(ID_ERROR_NEEDALPHA);
                return;
        }
    }

    pmiptex = (LPDIRECT3DTEXTURE9)m_ptexOrig;

    if (!PromptForBmp(&fileName))
        return;

    LPDIRECT3DSURFACE9 psurf;
    if (FAILED(hr = pmiptex->GetSurfaceLevel(0, &psurf)))
        return;

    if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurf)))
        return;
    if (m_numMips > 1)
        OnGenerateMipMaps();
    else if (m_ptexNew != NULL)
    {
        ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetLevelDesc(0, &sd);
        Compress(sd.Format, FALSE);
    }
    UpdateAllViews(NULL, 1);
}


HRESULT CDxtexDoc::LoadAlphaIntoSurface(CString& strPath, LPDIRECT3DSURFACE9 psurf)
{
    HRESULT hr;
    D3DSURFACE_DESC sd;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE9 ptexAlpha;
    LPDIRECT3DSURFACE9 psurfAlpha;
    LPDIRECT3DSURFACE9 psurfTarget;

    psurf->GetDesc(&sd);

    // Load the alpha BMP into psurfAlpha, a new A8R8G8B8 surface
    hr = D3DXCreateTextureFromFileEx(pd3ddev, strPath, sd.Width, sd.Height, 1, 0, 
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
        D3DX_DEFAULT, 0, NULL, NULL, &ptexAlpha);
    hr = ptexAlpha->GetSurfaceLevel(0, &psurfAlpha);

    // Copy the target surface into an A8R8G8B8 surface
    hr = pd3ddev->CreateOffscreenPlainSurface(sd.Width, sd.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &psurfTarget, NULL);
    hr = D3DXLoadSurfaceFromSurface(psurfTarget, NULL, NULL, psurf, NULL, NULL, 
        D3DX_DEFAULT, 0);

    // Fill in the alpha channels of psurfTarget based on the blue channel of psurfAlpha
    D3DLOCKED_RECT lrSrc;
    D3DLOCKED_RECT lrDest;

    hr = psurfAlpha->LockRect(&lrSrc, NULL, D3DLOCK_READONLY);
    hr = psurfTarget->LockRect(&lrDest, NULL, 0);

    DWORD xp;
    DWORD yp;
    DWORD* pdwRowSrc = (DWORD*)lrSrc.pBits;
    DWORD* pdwRowDest = (DWORD*)lrDest.pBits;
    DWORD* pdwSrc;
    DWORD* pdwDest;
    DWORD dwAlpha;
    LONG dataBytesPerRow = 4 * sd.Width;

    for (yp = 0; yp < sd.Height; yp++)
    {
        pdwSrc = pdwRowSrc;
        pdwDest = pdwRowDest;
        for (xp = 0; xp < sd.Width; xp++)
        {
            dwAlpha = *pdwSrc << 24;
            *pdwDest &= 0x00ffffff;
            *pdwDest |= dwAlpha;

            pdwSrc++;
            pdwDest++;
        }
        pdwRowSrc += lrSrc.Pitch / 4;
        pdwRowDest += lrDest.Pitch / 4;
    }

    psurfAlpha->UnlockRect();
    psurfTarget->UnlockRect();

    // Copy psurfTarget back into real surface
    hr = D3DXLoadSurfaceFromSurface(psurf, NULL, NULL, psurfTarget, NULL, NULL, 
        D3DX_DEFAULT, 0);
    
    // Release allocated interfaces
    ReleasePpo(&psurfTarget);
    ReleasePpo(&psurfAlpha);
    ReleasePpo(&ptexAlpha);

    return S_OK;
}


BOOL CDxtexDoc::PromptForBmp(CString* pstrPath)
{
    CFileDialog dlgFile(TRUE);

    CString title;
    VERIFY(title.LoadString(AFX_IDS_OPENFILE));

    CString strFilter;
    CString strDefault;

    strFilter += "Image Files (*.dds, *.hdr, *.bmp, *.tga, *.jpg, *.png, *.dib)";
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.dds;*.hdr;*.bmp;*.tga;*.jpg;*.png;*.dib");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    // append the "*.*" all files filter
    CString allFilter;
    VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
    strFilter += allFilter;
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.*");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle = title;
    dlgFile.m_ofn.lpstrFile = pstrPath->GetBuffer(_MAX_PATH);

    INT_PTR nResult = dlgFile.DoModal();
    pstrPath->ReleaseBuffer();
    if (nResult != IDOK)
        return FALSE;
    return TRUE;
}


void CDxtexDoc::OpenSubsurface(D3DCUBEMAP_FACES FaceType, LONG lwMip, LONG lwSlice)
{
    HRESULT hr;
    CString fileName;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE9 ptex = NULL;
    LPDIRECT3DSURFACE9 psurfOrig = NULL;
    LPDIRECT3DSURFACE9 psurfNew = NULL;

    if (!PromptForBmp(&fileName))
        return;

    if (IsVolumeMap())
    {
        hr = D3DXCreateTextureFromFile(pd3ddev, fileName, &ptex);
        hr = ptex->GetSurfaceLevel(0, &psurfOrig);
    }
    else if (IsCubeMap())
    {
        hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexOrig)->GetCubeMapSurface(FaceType, lwMip, &psurfOrig);
        if (m_ptexNew != NULL)
            hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexNew)->GetCubeMapSurface(FaceType, lwMip, &psurfNew);
        hr = D3DXLoadSurfaceFromFile(psurfOrig, NULL, NULL, fileName, NULL, D3DX_DEFAULT, 0, NULL);
    }
    else
    {
        hr = ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetSurfaceLevel(lwMip, &psurfOrig);
        if (m_ptexNew != NULL)
            hr = ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetSurfaceLevel(lwMip, &psurfNew);
        hr = D3DXLoadSurfaceFromFile(psurfOrig, NULL, NULL, fileName, NULL, D3DX_DEFAULT, 0, NULL);
    }

    // Look for "foo_a.bmp" for alpha channel
    int i = fileName.ReverseFind('.');
    fileName = fileName.Left(i) + "_a.bmp";
    CFileStatus status;
    if (CFile::GetStatus(fileName, status))
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfOrig)))
            return;
    }

    if (IsVolumeMap())
    {
        LPDIRECT3DVOLUME9 pvol;
        hr = ((LPDIRECT3DVOLUMETEXTURE9)m_ptexOrig)->GetVolumeLevel(lwMip, &pvol);
        hr = LoadVolumeSliceFromSurface(pvol, lwSlice, psurfOrig);
        ReleasePpo(&pvol);
        if (m_ptexNew)
        {
            hr = ((LPDIRECT3DVOLUMETEXTURE9)m_ptexNew)->GetVolumeLevel(lwMip, &pvol);
            hr = LoadVolumeSliceFromSurface(pvol, lwSlice, psurfOrig);
            ReleasePpo(&pvol);
        }
    }
    else if (psurfNew != NULL)
    {
        hr = D3DXLoadSurfaceFromSurface(psurfNew, NULL, NULL, psurfOrig, NULL, NULL, D3DX_DEFAULT, 0);
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);
    ReleasePpo(&ptex);

    SetModifiedFlag(TRUE);
    UpdateAllViews(NULL, 1);
}


void CDxtexDoc::OpenAlphaSubsurface(D3DCUBEMAP_FACES FaceType, LONG lwMip, LONG lwSlice)
{
    HRESULT hr;
    CString fileName;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE9 ptexOrig = NULL;
    LPDIRECT3DTEXTURE9 ptexNew = NULL;
    LPDIRECT3DSURFACE9 psurfOrig = NULL;
    LPDIRECT3DSURFACE9 psurfNew = NULL;
    LPDIRECT3DVOLUME9 pvolOrig = NULL;
    LPDIRECT3DVOLUME9 pvolNew = NULL;
    D3DSURFACE_DESC sd;
    DWORD dwWidth = m_dwWidth;
    DWORD dwHeight = m_dwHeight;

    if (IsVolumeMap())
    {
        for (int i = 0; i < lwMip; i++)
        {
            dwWidth /= 2;
            dwHeight /= 2;
        }
        hr = pd3ddev->CreateTexture(dwWidth, dwHeight, 1, 
             0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ptexOrig, NULL);
        hr = ptexOrig->GetSurfaceLevel(0, &psurfOrig);
        hr = ((LPDIRECT3DVOLUMETEXTURE9)m_ptexOrig)->GetVolumeLevel(lwMip, &pvolOrig);
        hr = LoadSurfaceFromVolumeSlice(pvolOrig, lwSlice, psurfOrig);
        if (m_ptexNew != NULL)
        {
            hr = pd3ddev->CreateTexture(dwWidth, dwHeight, 1, 
                 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ptexNew, NULL);
            hr = ptexNew->GetSurfaceLevel(0, &psurfOrig);
            hr = ((LPDIRECT3DVOLUMETEXTURE9)m_ptexOrig)->GetVolumeLevel(lwMip, &pvolNew);
            hr = LoadSurfaceFromVolumeSlice(pvolNew, lwSlice, psurfOrig);
        }
    }
    else if (IsCubeMap())
    {
        hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexOrig)->GetCubeMapSurface(FaceType, lwMip, &psurfOrig);
        ((LPDIRECT3DCUBETEXTURE9)m_ptexOrig)->GetLevelDesc(lwMip, &sd);
        if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
        {
            AfxMessageBox(ID_ERROR_PREMULTALPHA);
            goto LCleanup;
        }
        if (m_ptexNew != NULL)
        {
            hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexNew)->GetCubeMapSurface(FaceType, lwMip, &psurfNew);
            ((LPDIRECT3DCUBETEXTURE9)m_ptexNew)->GetLevelDesc(lwMip, &sd);
            if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
            {
                AfxMessageBox(ID_ERROR_PREMULTALPHA);
                goto LCleanup;
            }
        }
    }
    else
    {
        BOOL bAlphaFound = FALSE;
        hr = ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetSurfaceLevel(lwMip, &psurfOrig);
        ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetLevelDesc(lwMip, &sd);
        if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
        {
            AfxMessageBox(ID_ERROR_PREMULTALPHA);
            goto LCleanup;
        }

        // Check if the original has alpha
        if( FormatContainsAlpha(sd.Format) )
        {
            bAlphaFound = TRUE;
        }

        if (m_ptexNew != NULL)
        {
            hr = ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetSurfaceLevel(lwMip, &psurfNew);
            ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetLevelDesc(lwMip, &sd);
            if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
            {
                AfxMessageBox(ID_ERROR_PREMULTALPHA);
                goto LCleanup;
            }
            // Check if the new has alpha
            if( FormatContainsAlpha(sd.Format) )
            {
                bAlphaFound = TRUE;
            }
        }

        if( bAlphaFound == FALSE )
        {
            AfxMessageBox(ID_ERROR_NEEDALPHA);
            goto LCleanup;
        }
    }

    if (!PromptForBmp(&fileName))
        goto LCleanup;

    if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfOrig)))
        goto LCleanup;

    if (psurfNew != NULL)
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfNew)))
            goto LCleanup;
    }

    if (pvolOrig != NULL)
    {
        hr = LoadVolumeSliceFromSurface(pvolOrig, lwSlice, psurfOrig);
    }
    if (pvolNew != NULL)
    {
        hr = LoadVolumeSliceFromSurface(pvolNew, lwSlice, psurfNew);
    }

    SetModifiedFlag(TRUE);
    UpdateAllViews(NULL, 1);

LCleanup:
    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);
    ReleasePpo(&ptexOrig);
    ReleasePpo(&ptexNew);
    ReleasePpo(&pvolOrig);
    ReleasePpo(&pvolNew);
}


void CDxtexDoc::OnFormatChangeCubeMapFaces() 
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    D3DSURFACE_DESC sd;
    LPDIRECT3DCUBETEXTURE9 ptexCube;
    DWORD iLevel;
    LPDIRECT3DSURFACE9 psurfSrc;
    LPDIRECT3DSURFACE9 psurfDest;

    CCubeMapDlg cubeMapDlg;
    if (IDCANCEL == cubeMapDlg.DoModal())
        return;
    
    // Change m_ptexOrig into a cubemap
    ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetLevelDesc(0, &sd);
    hr = D3DXCreateCubeTexture(pd3ddev, m_dwWidth, m_numMips, 0, sd.Format, D3DPOOL_MANAGED, &ptexCube);
    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        hr = ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetSurfaceLevel(iLevel, &psurfSrc);
        hr = ptexCube->GetCubeMapSurface((D3DCUBEMAP_FACES)cubeMapDlg.m_iFace, iLevel, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
            psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
    }
    ReleasePpo(&m_ptexOrig);
    m_ptexOrig = ptexCube;

    // Change m_ptexNew into a cubemap too
    if (m_ptexNew != NULL)
    {
        ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetLevelDesc(0, &sd);
        hr = D3DXCreateCubeTexture(pd3ddev, m_dwWidth, m_numMips, 0, sd.Format, D3DPOOL_MANAGED, &ptexCube);
        for (iLevel = 0; iLevel < m_numMips; iLevel++)
        {
            hr = ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = ptexCube->GetCubeMapSurface((D3DCUBEMAP_FACES)cubeMapDlg.m_iFace, iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&psurfDest);
        }
        ReleasePpo(&m_ptexNew);
        m_ptexNew = ptexCube;
    }
    m_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
    SetModifiedFlag();
    UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
}


void CDxtexDoc::OnFormatMakeIntoVolumeMap() 
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    D3DSURFACE_DESC sd;
    LPDIRECT3DVOLUMETEXTURE9 ptexVolume;
    DWORD iLevel;
    LPDIRECT3DSURFACE9 psurfSrc;
    LPDIRECT3DVOLUME9 pvolumeDest;
    UINT numLayers;

    CVolumeMapDlg volumeMapDlg;
    if (IDCANCEL == volumeMapDlg.DoModal())
        return;

    numLayers = (1 << volumeMapDlg.m_powLayers) * 2;

    // Change m_ptexOrig into a volumemap
    ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetLevelDesc(0, &sd);
    hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, numLayers,
        m_numMips, 0, sd.Format, D3DPOOL_SYSTEMMEM, &ptexVolume, NULL);
    if (FAILED(hr))
    {
        if( E_OUTOFMEMORY == hr )
            AfxMessageBox( ID_ERROR_OUTOFMEMORY );
        else
            AfxMessageBox( ID_ERROR_UNKNOWN );
        return;
    }
    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        hr = ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetSurfaceLevel(iLevel, &psurfSrc);
        hr = ptexVolume->GetVolumeLevel(iLevel, &pvolumeDest);
        hr = LoadVolumeSliceFromSurface(pvolumeDest, 0, psurfSrc);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&pvolumeDest);
    }
    ReleasePpo(&m_ptexOrig);
    m_ptexOrig = ptexVolume;

    // Change m_ptexNew into a volumemap too
    if (m_ptexNew != NULL)
    {
        ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetLevelDesc(0, &sd);
        hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, numLayers,
            m_numMips, 0, sd.Format, D3DPOOL_SYSTEMMEM, &ptexVolume, NULL);
        if (FAILED(hr))
            return;
        for (iLevel = 0; iLevel < m_numMips; iLevel++)
        {
            hr = ((LPDIRECT3DTEXTURE9)m_ptexNew)->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = ptexVolume->GetVolumeLevel(iLevel, &pvolumeDest);
            hr = LoadVolumeSliceFromSurface(pvolumeDest, 0, psurfSrc);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&pvolumeDest);
        }
        ReleasePpo(&m_ptexNew);
        m_ptexNew = ptexVolume;
    }
    m_dwDepth = numLayers;
    SetModifiedFlag();
    UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
}


HRESULT CDxtexDoc::LoadVolumeSliceFromSurface(LPDIRECT3DVOLUME9 pVolume, UINT iSlice, LPDIRECT3DSURFACE9 psurf)
{
    HRESULT hr;
    D3DSURFACE_DESC sd;
    D3DVOLUME_DESC vd;
    D3DLOCKED_RECT lr;
    D3DBOX boxSrc;
    D3DBOX boxDest;

    psurf->GetDesc(&sd);
    pVolume->GetDesc(&vd);

    boxSrc.Left = 0;
    boxSrc.Right = sd.Width;
    boxSrc.Top = 0;
    boxSrc.Bottom = sd.Height;
    boxSrc.Front = 0;
    boxSrc.Back = 1;

    boxDest.Left = 0;
    boxDest.Right = vd.Width;
    boxDest.Top = 0;
    boxDest.Bottom = vd.Height;
    boxDest.Front = iSlice;
    boxDest.Back = iSlice + 1;

    hr = psurf->LockRect(&lr, NULL, 0);
    if (FAILED(hr))
        return hr;

    hr = D3DXLoadVolumeFromMemory(pVolume, NULL, &boxDest, lr.pBits, sd.Format, lr.Pitch, 
        0, NULL, &boxSrc, D3DX_DEFAULT, 0);

    psurf->UnlockRect();

    return hr;
}


HRESULT CDxtexDoc::LoadSurfaceFromVolumeSlice(LPDIRECT3DVOLUME9 pVolume, UINT iSlice, LPDIRECT3DSURFACE9 psurf)
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
        NULL, &rc, D3DX_DEFAULT, 0);

    pVolume->UnlockBox();

    return hr;
}


HRESULT CDxtexDoc::BltAllLevels(D3DCUBEMAP_FACES FaceType, 
    LPDIRECT3DBASETEXTURE9 ptexSrc, LPDIRECT3DBASETEXTURE9 ptexDest)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pmiptexSrc;
    LPDIRECT3DTEXTURE9 pmiptexDest;
    LPDIRECT3DCUBETEXTURE9 pcubetexSrc;
    LPDIRECT3DCUBETEXTURE9 pcubetexDest;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexSrc;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexDest;
    DWORD iLevel;

    if (IsVolumeMap())
    {
        pvoltexSrc = (LPDIRECT3DVOLUMETEXTURE9)ptexSrc;
        pvoltexDest = (LPDIRECT3DVOLUMETEXTURE9)ptexDest;
    }
    else if (IsCubeMap())
    {
        pcubetexSrc = (LPDIRECT3DCUBETEXTURE9)ptexSrc;
        pcubetexDest = (LPDIRECT3DCUBETEXTURE9)ptexDest;
    }
    else
    {
        pmiptexSrc = (LPDIRECT3DTEXTURE9)ptexSrc;
        pmiptexDest = (LPDIRECT3DTEXTURE9)ptexDest;
    }

    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        if (IsVolumeMap())
        {
            LPDIRECT3DVOLUME9 pvolSrc = NULL;
            LPDIRECT3DVOLUME9 pvolDest = NULL;
            hr = pvoltexSrc->GetVolumeLevel(iLevel, &pvolSrc);
            hr = pvoltexDest->GetVolumeLevel(iLevel, &pvolDest);
            hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, 
                pvolSrc, NULL, NULL, D3DX_DEFAULT, 0);
            ReleasePpo(&pvolSrc);
            ReleasePpo(&pvolDest);
        }
        else if (IsCubeMap())
        {
            LPDIRECT3DSURFACE9 psurfSrc = NULL;
            LPDIRECT3DSURFACE9 psurfDest = NULL;
            hr = pcubetexSrc->GetCubeMapSurface(FaceType, iLevel, &psurfSrc);
            hr = pcubetexDest->GetCubeMapSurface(FaceType, iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&psurfDest);
        }
        else
        {
            LPDIRECT3DSURFACE9 psurfSrc = NULL;
            LPDIRECT3DSURFACE9 psurfDest = NULL;
            hr = pmiptexSrc->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = pmiptexDest->GetSurfaceLevel(iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&psurfDest);
        }
    }

    return S_OK;
}


HRESULT CDxtexDoc::Resize(DWORD dwWidthNew, DWORD dwHeightNew)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pmiptexNew;
    LPDIRECT3DDEVICE9 pd3ddev = PDxtexApp()->Pd3ddev();
    
    hr = pd3ddev->CreateTexture(dwWidthNew, dwHeightNew, m_numMips, 
         0, GetFormat(m_ptexOrig), D3DPOOL_MANAGED, &pmiptexNew, NULL);
    if (FAILED(hr))
        return hr;
    if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, m_ptexOrig, pmiptexNew)))
        return hr;
    ReleasePpo(&m_ptexOrig);
    m_ptexOrig = pmiptexNew;

    if( m_ptexNew != NULL )
    {
        hr = pd3ddev->CreateTexture(dwWidthNew, dwHeightNew, m_numMips, 
             0, GetFormat(m_ptexOrig), D3DPOOL_MANAGED, &pmiptexNew, NULL);
        if (FAILED(hr))
            return hr;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, m_ptexNew, pmiptexNew)))
            return hr;
        ReleasePpo(&m_ptexNew);
        m_ptexNew = pmiptexNew;
    }

    m_dwWidth = dwWidthNew;
    m_dwHeight = dwHeightNew;
    SetModifiedFlag(TRUE);
    UpdateAllViews(NULL, 4);

    return S_OK;
}


void CDxtexDoc::OpenCubeFace(D3DCUBEMAP_FACES FaceType)
{
    HRESULT hr;
    CString fileName;
    LPDIRECT3DSURFACE9 psurfOrig = NULL;
    LPDIRECT3DSURFACE9 psurfNew = NULL;

    if (!IsCubeMap())
        return;

    hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexOrig)->GetCubeMapSurface(FaceType, 0, &psurfOrig);
    if (m_ptexNew != NULL)
        hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexNew)->GetCubeMapSurface(FaceType, 0, &psurfNew);

    if (!PromptForBmp(&fileName))
        return;

    hr = D3DXLoadSurfaceFromFile(psurfOrig, NULL, NULL, fileName, NULL, D3DX_DEFAULT, 0, NULL);

    // Look for "foo_a.bmp" for alpha channel
    int i = fileName.ReverseFind('.');
    fileName = fileName.Left(i) + "_a.bmp";
    CFileStatus status;
    if (CFile::GetStatus(fileName, status))
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfOrig)))
            return;
    }

    if (m_numMips > 1)
    {
        hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE9)m_ptexOrig, NULL, 0, D3DX_DEFAULT);
    }


    if (psurfNew != NULL)
    {
        hr = D3DXLoadSurfaceFromSurface(psurfNew, NULL, NULL, psurfOrig, NULL, NULL, D3DX_DEFAULT, 0);

        if (m_numMips > 1)
        {
            hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE9)m_ptexNew, NULL, 0, D3DX_DEFAULT);
        }
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);

    SetModifiedFlag(TRUE);
    UpdateAllViews(NULL, 1);
}


void CDxtexDoc::OpenAlphaCubeFace(D3DCUBEMAP_FACES FaceType)
{
    HRESULT hr;
    CString fileName;
    LPDIRECT3DSURFACE9 psurfOrig = NULL;
    LPDIRECT3DSURFACE9 psurfNew = NULL;
    D3DSURFACE_DESC sd;

    if (!IsCubeMap())
        return;

    hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexOrig)->GetCubeMapSurface(FaceType, 0, &psurfOrig);
    ((LPDIRECT3DCUBETEXTURE9)m_ptexOrig)->GetLevelDesc(0, &sd);
    if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
    {
        AfxMessageBox(ID_ERROR_PREMULTALPHA);
        return;
    }
    if (m_ptexNew != NULL)
    {
        hr = ((LPDIRECT3DCUBETEXTURE9)m_ptexNew)->GetCubeMapSurface(FaceType, 0, &psurfNew);
    }

    if (!PromptForBmp(&fileName))
        return;

    if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfOrig)))
        return;

    if (psurfNew != NULL)
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfNew)))
            return;
    }

    if (m_numMips > 1)
    {
        hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE9)m_ptexOrig, NULL, 0, D3DX_DEFAULT);
    }


    if (psurfNew != NULL)
    {
        hr = D3DXLoadSurfaceFromSurface(psurfNew, NULL, NULL, psurfOrig, NULL, NULL, D3DX_DEFAULT, 0);

        if (m_numMips > 1)
        {
            hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE9)m_ptexNew, NULL, 0, D3DX_DEFAULT);
        }
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);

    SetModifiedFlag(TRUE);
    UpdateAllViews(NULL, 1);
}


DWORD CDxtexDoc::DwDepthAt(LONG lwMip)
{
    DWORD dwDepth = m_dwDepth;
    while (lwMip > 0 && dwDepth > 1)
    {
        dwDepth /= 2;
        lwMip--;
    }
    return dwDepth;
}


void CDxtexDoc::OnFormatChangeSurfaceFmt() 
{
    CChangeFmtDlg changeFmtDlg;
    LPDIRECT3DBASETEXTURE9 ptex;

    ptex = (m_ptexNew == NULL ? m_ptexOrig : m_ptexNew);

    if (IsVolumeMap())
    {
        D3DVOLUME_DESC vd;
        ((LPDIRECT3DVOLUMETEXTURE9)ptex)->GetLevelDesc(0, &vd);
        changeFmtDlg.m_fmt = vd.Format;
    }
    else if (IsCubeMap())
    {
        D3DSURFACE_DESC sd;
        ((LPDIRECT3DCUBETEXTURE9)ptex)->GetLevelDesc(0, &sd);
        changeFmtDlg.m_fmt = sd.Format;
    }
    else
    {
        D3DSURFACE_DESC sd;
        ((LPDIRECT3DTEXTURE9)ptex)->GetLevelDesc(0, &sd);
        changeFmtDlg.m_fmt = sd.Format;
    }

    changeFmtDlg.m_bVolume = IsVolumeMap();

    if (IDCANCEL == changeFmtDlg.DoModal())
        return;

    Compress(changeFmtDlg.m_fmt, TRUE);
}


void CDxtexDoc::OnUpdateFileOpenAlpha(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!IsCubeMap() && !IsVolumeMap()); 
}


void CDxtexDoc::OnUpdateFormatGenerateMipmaps(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_numMips <= 1); 
}


void CDxtexDoc::OnUpdateFormatChangeCubeMapFaces(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!IsCubeMap() && !IsVolumeMap()); 
}


void CDxtexDoc::OnUpdateFormatMakeIntoVolumeMap(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!IsCubeMap() && !IsVolumeMap());
}


void CDxtexDoc::OnFormatResize() 
{
    CResizeDialog resizeDialog;

    resizeDialog.m_oldWidth.Format("%d", m_dwWidth);
    resizeDialog.m_oldHeight.Format("%d", m_dwHeight);
    resizeDialog.m_newWidth = m_dwWidth;
    resizeDialog.m_newHeight = m_dwHeight;
    resizeDialog.m_oldWidth.Format("%d", m_dwWidth);
    if( IDOK == resizeDialog.DoModal() )
        Resize(resizeDialog.m_newWidth, resizeDialog.m_newHeight);
}


void CDxtexDoc::OnUpdateFormatResize(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(!IsCubeMap() && !IsVolumeMap());
}
