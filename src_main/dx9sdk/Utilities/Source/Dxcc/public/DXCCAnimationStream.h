#ifndef DXCCANIMATIONSTREAM_H
#define DXCCANIMATIONSTREAM_H


typedef interface	IDXCCAnimationStream		IDXCCAnimationStream			,*LPDXCCANIMATIONSTREAM;

interface __declspec( uuid("ae831c8a-7d7d-4959-be8f-0f5b82699367") ) IDXCCAnimationStream;
DEFINE_GUID( IID_IDXCCAnimationStream,
    0xae831c8a,
    0x7d7d,
    0x4959,
    0xbe, 0x8f, 0x0f, 0x5b, 0x82, 0x69, 0x93, 0x67);


//Key an entire matrix rather than SRT
typedef struct DXCCKEY_MATRIX
{
    FLOAT Time;
    D3DXMATRIX Value;
} *LPDXCCKEY_MATRIX;

typedef struct DXCCKEY_TRANSFORM
{
    FLOAT Time;
    D3DXVECTOR3 ValueS;
    D3DXQUATERNION ValueR;
    D3DXVECTOR3 ValueT;
} *LPDXCCKEY_TRANSFORM;



#undef INTERFACE
#define INTERFACE IDXCCAnimationStream
DECLARE_INTERFACE_(IDXCCAnimationStream, IUnknown)
{
    // ID3DXAnimationSet
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // Period
    STDMETHOD_(DOUBLE, GetPeriod)(THIS) PURE;
    STDMETHOD_(DOUBLE, GetPeriodicPosition)(THIS_ DOUBLE Position) PURE;    // Maps position into animation period

     // SRT
    STDMETHOD(GetTransform)(THIS_ 
        DOUBLE PeriodicPosition,            // Position mapped to period (use GetPeriodicPosition)
        D3DXVECTOR3 *pScale,                // Returns the scale
        D3DXQUATERNION *pRotation,          // Returns the rotation as a quaternion
        D3DXVECTOR3 *pTranslation) PURE;    // Returns the translation

     // SRT
    STDMETHOD(GetMatrix)(THIS_ 
        DOUBLE PeriodicPosition,            // Position mapped to period (use GetPeriodicPosition)
        D3DXMATRIX *pMatrix) PURE;			// Returns the matrix

    // Callbacks
    //STDMETHOD(GetCallback)(THIS_ 
    //    DOUBLE Position,                    // Position from which to find callbacks
    //    DWORD Flags,                        // Callback search flags
    //    DOUBLE *pCallbackPosition,          // Returns the position of the callback
    //    LPVOID *ppCallbackData) PURE;       // Returns the callback data pointer

    // Playback
    STDMETHOD_(D3DXPLAYBACK_TYPE, GetPlaybackType)(THIS) PURE;
    STDMETHOD_(DOUBLE, GetSourceTicksPerSecond)(THIS) PURE;

    // Scale keys
    STDMETHOD_(UINT, NumScaleKeys)(THIS) PURE;
	STDMETHOD(GetScaleKey)(THIS_ UINT Key, LPD3DXKEY_VECTOR3 pScaleKey) PURE;
	STDMETHOD(SetScaleKey)(THIS_ UINT Key, LPD3DXKEY_VECTOR3 pScaleKey) PURE;
	STDMETHOD(FindScaleKey)(THIS_ FLOAT fKeyTime, UINT* pKey, LPD3DXKEY_VECTOR3 pValue) PURE;

    // Rotation keys
    STDMETHOD_(UINT, NumRotationKeys)(THIS) PURE;
	STDMETHOD(GetRotationKey)(THIS_ UINT Key, LPD3DXKEY_QUATERNION pRotationKey) PURE;
	STDMETHOD(SetRotationKey)(THIS_ UINT Key, LPD3DXKEY_QUATERNION pRotationKey) PURE;
	STDMETHOD(FindRotationKey)(THIS_ FLOAT fKeyTime, UINT* pKey, LPD3DXKEY_QUATERNION pValue) PURE;

    // Translation keys
    STDMETHOD_(UINT, NumTranslationKeys)(THIS) PURE;
	STDMETHOD(GetTranslationKey)(THIS_ UINT Key, LPD3DXKEY_VECTOR3 pTranslationKey) PURE;
	STDMETHOD(SetTranslationKey)(THIS_ UINT Key, LPD3DXKEY_VECTOR3 pTranslationKey) PURE;
	STDMETHOD(FindTranslationKey)(THIS_ FLOAT fKeyTime, UINT* pKey, LPD3DXKEY_VECTOR3 pValue) PURE;


    STDMETHOD_(UINT, NumTransformKeys)(THIS) PURE;
	STDMETHOD(GetTransformKey)(THIS_ UINT Key, LPDXCCKEY_TRANSFORM pTransformKey) PURE;
	STDMETHOD(SetTransformKey)(THIS_ UINT Key, LPDXCCKEY_TRANSFORM pTransformKey) PURE;
	STDMETHOD(FindTransformKey)(THIS_ FLOAT fKeyTime, UINT* pKey, LPDXCCKEY_TRANSFORM pValue) PURE;

	STDMETHOD(GetTransformKeyAsMatrix)(THIS_ UINT Key, LPDXCCKEY_MATRIX pMatrixKey) PURE;
	STDMETHOD(SetTransformKeyAsMatrix)(THIS_ UINT Key, LPDXCCKEY_MATRIX pMatrixKey) PURE;
	STDMETHOD(FindTransformKeyAsMatrix)(THIS_ FLOAT fKeyTime, UINT* pKey, LPDXCCKEY_MATRIX pValue) PURE;


    // Callback keys
    //STDMETHOD_(UINT, NumCallbackKeys)(THIS) PURE;
	//STDMETHOD(GetCallbackKey)(THIS_ UINT Key, LPD3DXKEY_CALLBACK pCallbackKey) PURE;
	//STDMETHOD(SetCallbackKey)(THIS_ UINT Key, LPD3DXKEY_CALLBACK pCallbackKey) PURE;

	// Key removal methods. These are slow, and should not be used once the animation starts playing
	STDMETHOD(RemoveScaleKey)(THIS_ UINT Key) PURE;
	STDMETHOD(RemoveRotationKey)(THIS_ UINT Key) PURE;
	STDMETHOD(RemoveTranslationKey)(THIS_ UINT Key) PURE;
	STDMETHOD(RemoveTransformKey)(THIS_ UINT Key) PURE;

	// Key removal methods. These are slow, and should not be used once the animation starts playing
	STDMETHOD(InsertScaleKey)(THIS_ UINT iBeforeKey, LPD3DXKEY_VECTOR3 pScaleKey) PURE;
	STDMETHOD(InsertRotationKey)(THIS_ UINT iBeforeKey, LPD3DXKEY_QUATERNION pRotationKey) PURE;
	STDMETHOD(InsertTranslationKey)(THIS_ UINT iBeforeKey, LPD3DXKEY_VECTOR3 pTranslationKey) PURE;
	STDMETHOD(InsertTransformKey)(THIS_ UINT iBeforeKey, LPDXCCKEY_TRANSFORM pTransformKey) PURE;
	STDMETHOD(InsertTransformKeyAsMatrix)(THIS_ UINT iBeforeKey, LPDXCCKEY_MATRIX pMatrixKey) PURE;

    // One-time animaton SRT keyframe registration
    STDMETHOD(CreateSRTKeys)(THIS_ 
		DOUBLE TicksPerSecond,
		D3DXPLAYBACK_TYPE Playback,
        UINT NumScaleKeys,                              // Number of scale keys
        UINT NumRotationKeys,                           // Number of rotation keys
        UINT NumTranslationKeys) PURE;	// Returns the animation index 

   // One-time animaton Matrix keyframe registration
    STDMETHOD(CreateTransformKeys)(THIS_ 
		DOUBLE TicksPerSecond,
		D3DXPLAYBACK_TYPE Playback,
        UINT NumTransforms) PURE;	// Returns the animation index 

    STDMETHOD(DestroyKeys)(THIS) PURE;

    //STDMETHOD(SetName)(THIS_ char* name) PURE;

    STDMETHOD(GetD3DXAnimationSet)(THIS_ IDXCCManager* pManager, LPD3DXKEYFRAMEDANIMATIONSET* ppSet) PURE;


};


#endif