// dxtexDoc.h : interface of the CDxtexDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXTXDOC_H__712C53CF_D63B_11D1_A8B5_00C04FC2DC22__INCLUDED_)
#define AFX_DXtxDOC_H__712C53CF_D63B_11D1_A8B5_00C04FC2DC22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDxtexDoc : public CDocument
{
protected: // create from serialization only
    CDxtexDoc();
    DECLARE_DYNCREATE(CDxtexDoc)

public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDxtexDoc)
    public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
    //}}AFX_VIRTUAL

// Implementation
public:
    HRESULT LoadAlphaBmp(CString& strPath);
    VOID GenerateMipMaps(VOID);
    HRESULT ChangeFormat(LPDIRECT3DBASETEXTURE9 ptexCur, D3DFORMAT fmt, 
                         LPDIRECT3DBASETEXTURE9* pptexNew);
    HRESULT Compress(D3DFORMAT fmt, BOOL bSwitchView);
    HRESULT Resize(DWORD dwWidthNew, DWORD dwHeightNew);
    DWORD NumMips(VOID);
    LPDIRECT3DBASETEXTURE9 PtexOrig(VOID) { return m_ptexOrig; }
    LPDIRECT3DBASETEXTURE9 PtexNew(VOID) { return m_ptexNew; }
    DWORD DwWidth(VOID) { return m_dwWidth; }
    DWORD DwHeight(VOID) { return m_dwHeight; }
    DWORD DwDepth(VOID) { return m_dwDepth; }
    DWORD DwDepthAt(LONG lwMip);
    BOOL TitleModsChanged(VOID) { return m_bTitleModsChanged; }
    VOID ClearTitleModsChanged(VOID) { m_bTitleModsChanged = FALSE; }
    virtual ~CDxtexDoc();
    void OpenSubsurface(D3DCUBEMAP_FACES FaceType, LONG lwMip, LONG lwSlice);
    void OpenAlphaSubsurface(D3DCUBEMAP_FACES FaceType, LONG lwMip, LONG lwSlice);
    void OpenCubeFace(D3DCUBEMAP_FACES FaceType);
    void OpenAlphaCubeFace(D3DCUBEMAP_FACES FaceType);
    BOOL IsCubeMap(VOID) { return (m_dwCubeMapFlags > 0); }
    BOOL IsVolumeMap(VOID) { return (m_dwDepth > 0); }
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
    //{{AFX_MSG(CDxtexDoc)
    afx_msg void OnFileOpenAlpha();
    afx_msg void OnGenerateMipMaps();
    afx_msg void OnFormatDxt1();
    afx_msg void OnFormatDxt2();
    afx_msg void OnFormatDxt3();
    afx_msg void OnFormatDxt4();
    afx_msg void OnFormatDxt5();
    afx_msg void OnFormatChangeCubeMapFaces();
    afx_msg void OnUpdateFileOpenAlpha(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFormatGenerateMipmaps(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFormatChangeCubeMapFaces(CCmdUI* pCmdUI);
    afx_msg void OnFormatMakeIntoVolumeMap();
    afx_msg void OnUpdateFormatMakeIntoVolumeMap(CCmdUI* pCmdUI);
    afx_msg void OnFormatChangeSurfaceFmt();
    afx_msg void OnFormatResize();
    afx_msg void OnUpdateFormatResize(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    LPDIRECT3DBASETEXTURE9 m_ptexOrig;
    LPDIRECT3DBASETEXTURE9 m_ptexNew;
    DWORD m_dwWidth;
    DWORD m_dwHeight;
    DWORD m_dwDepth; // For volume textures
    DWORD m_numMips;
    DWORD m_dwCubeMapFlags;
    BOOL m_bTitleModsChanged;

    CDxtexApp* PDxtexApp(VOID) { return (CDxtexApp*)AfxGetApp(); }
    HRESULT LoadAlphaIntoSurface(CString& strPath, LPDIRECT3DSURFACE9 psurf);
    HRESULT LoadVolumeSliceFromSurface(LPDIRECT3DVOLUME9 pVolume, UINT iSlice, LPDIRECT3DSURFACE9 pSurf);
    HRESULT LoadSurfaceFromVolumeSlice(LPDIRECT3DVOLUME9 pVolume, UINT iSlice, LPDIRECT3DSURFACE9 psurf);
    HRESULT BltAllLevels(D3DCUBEMAP_FACES FaceType, LPDIRECT3DBASETEXTURE9 ptexSrc, 
        LPDIRECT3DBASETEXTURE9 ptexDest);
    BOOL PromptForBmp(CString* pstrPath);
    D3DFORMAT GetFormat(LPDIRECT3DBASETEXTURE9 ptex);
    HRESULT EnsureAlpha(LPDIRECT3DBASETEXTURE9* pptex);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXTXDOC_H__712C53CF_D63B_11D1_A8B5_00C04FC2DC22__INCLUDED_)
