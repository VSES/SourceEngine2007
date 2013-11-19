#include "camera.h"
#include "stateblock.h"
#include "render.h"

DECLARE_SINGLETON_CPP( Camera );

void Camera::Home()
{
    Place( D3DCUBEMAP_FACE_NEGATIVE_Z );
}

void Camera::Place( D3DXVECTOR3& eye)
{
    __super::Reset();

    D3DXVECTOR3 center = STATE_GET(Render)->GetModelCenter();
    float radius = STATE_GET(Render)->GetModelRadius();

    __super::SetViewParams( &eye, &center );
    __super::SetModelCenter( center );
    __super::SetRadius( radius*2.5f, 0.0f/*radius*/, radius*10.0f );

    D3DXQUATERNION quat;
    D3DXMATRIXA16 mIdentity;
    D3DXMatrixIdentity( &mIdentity );
    D3DXQuaternionRotationMatrix( &quat, &mIdentity );
    __super::SetWorldQuat( quat );
    __super::SetWorldMatrix( mIdentity );

    Resize();
}

void Camera::Place(D3DCUBEMAP_FACES face)
{
    D3DXVECTOR3 center = STATE_GET(Render)->GetModelCenter();
    float radius = 2.5f*STATE_GET(Render)->GetModelRadius();
    D3DXVECTOR3 eye;
    switch( face )
    {
    case D3DCUBEMAP_FACE_NEGATIVE_X:
        {
            eye = D3DXVECTOR3(-radius, 0.0f, 0.0f);
        }
        break;
    case D3DCUBEMAP_FACE_POSITIVE_X:
        {
             eye = D3DXVECTOR3(radius, 0.0f, 0.0f);
        }
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_Y:
        {
            eye = D3DXVECTOR3(0.0f, -radius, 0.0f);
        }
        break;
    case D3DCUBEMAP_FACE_POSITIVE_Y:
        {
            eye = D3DXVECTOR3(0.0f, radius, 0.0f);
        }
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_Z:
        {
            eye = D3DXVECTOR3(0.0f, 0.0f, -radius);
        }
        break;
    case D3DCUBEMAP_FACE_POSITIVE_Z:
    default:
        {
            eye = D3DXVECTOR3(0.0f, 0.0f, radius);
        }
        break;
    };

    eye = center + eye;

    Place(eye);
}

void Camera::Resize()
{
    RECT rect = DXUTGetWindowClientRect();
    
    float fAspectRatio = (FLOAT)rect.right / (FLOAT)rect.bottom;

    D3DXVECTOR4 modelInView;
    D3DXVec3Transform(&modelInView, &m_vModelCenter, &m_mView);

    SetProjParams( D3DX_PI/4, fAspectRatio, max(0.1f, modelInView.z-(m_fMaxRadius*2)) , modelInView.z+(m_fMaxRadius*2) );
    SetWindow( rect.right, rect.bottom );
}


//bool Camera::OnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown )
LRESULT Camera::HandleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_KEYDOWN:
            {
                UINT nChar = (UINT)wParam;
                switch( nChar )
                {
                case VK_HOME:
                    Home();
                    return true;
                case L'X':
                case L'Y':
                case L'Z':
                    bool key_shiftl = (0x8000 & GetKeyState(VK_LSHIFT)) != 0; 
                    bool key_shiftr = (0x8000 & GetKeyState(VK_RSHIFT)) != 0; 
                    bool key_shift = key_shiftl || key_shiftr || ((0x8000 & GetKeyState(VK_SHIFT)) != 0); 

                    D3DCUBEMAP_FACES face = D3DCUBEMAP_FACE_FORCE_DWORD;
                    if(nChar == L'X')
                        face = key_shift ? D3DCUBEMAP_FACE_NEGATIVE_X : D3DCUBEMAP_FACE_POSITIVE_X;
                    else if(nChar == L'Y')
                        face = key_shift ? D3DCUBEMAP_FACE_NEGATIVE_Y : D3DCUBEMAP_FACE_POSITIVE_Y;                         
                    else if(nChar == L'Z')
                        face = key_shift ? D3DCUBEMAP_FACE_NEGATIVE_Z : D3DCUBEMAP_FACE_POSITIVE_Z;

                    Place( face );
                    return true;
                }
            }
            break;
    };

    return __super::HandleMessages( hWnd, uMsg, wParam, lParam );
}
