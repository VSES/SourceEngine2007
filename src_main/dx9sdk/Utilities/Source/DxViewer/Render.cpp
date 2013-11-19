

#include "d3d9.h"
#include "d3dx9.h"

#include "App.h"
#include "camera.h"
#include "render.h"
#include "resource.h"

#include "windows.h"
#pragma warning(disable: 4995)
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#pragma warning(default: 4995)

const SHORT IbFSQ[] = {0, 1, 2, 2, 1, 3};
const FSQ_VERTEX VbFSQ[] = 
{
    {-1.0f, -1.0f, 0.5}, 
    {1.0f, -1.0f, 0.5}, 
    {-1.0f, 1.0f, 0.5}, 
    { 1.0f, 1.0f, 0.5} };

void Render::PromoteSharedValues( Sas::BoundValueTable* table  )
{
    if(table->GetParent() == NULL)
        return;

    table->PromoteToParent( L"Sas.Skeleton.NumJoints" );

    table->PromoteToParent( L"Sas.EnvironmentMap" );

    //table->PromoteToParent( L"Sas.Camera" );
    table->PromoteToParent( L"Sas.Camera.WorldToView" );
    table->PromoteToParent( L"Sas.Camera.Projection" );
    table->PromoteToParent( L"Sas.Camera.NearFarClipping" );
    table->PromoteToParent( L"Sas.Camera.Position" );

    table->PromoteToParent( L"Sas.Time.Last");
    table->PromoteToParent( L"Sas.Time.Now");
    table->PromoteToParent( L"Sas.Time.FrameNumer" );

    table->PromoteToParent( L"Sas.NumAmbientLights" );
    table->PromoteToParent( L"Sas.AmbientLight[0]" );
    //table->PromoteToParent( L"Sas.AmbientLight[0].Color" );

    table->PromoteToParent( L"Sas.NumDirectionalLights" );
    table->PromoteToParent( L"Sas.DirectionalLight[0]" );
    //table->PromoteToParent( L"Sas.DirectionalLight[0].Color" );
    //table->PromoteToParent( L"Sas.DirectionalLight[0].Direction" );

    table->PromoteToParent( L"Sas.NumPointLights");
    table->PromoteToParent( L"Sas.PointLight[0]" );
    //table->PromoteToParent( L"Sas.PointLight[0].Color" );
    //table->PromoteToParent( L"Sas.PointLight[0].Position" );
    //table->PromoteToParent( L"Sas.PointLight[0].Range" )
    ;
    table->PromoteToParent( L"Sas.NumSpotLights" );
    table->PromoteToParent( L"Sas.SpotLight[0]" );
    //table->PromoteToParent( L"Sas.SpotLight[0].Color" );
    //table->PromoteToParent( L"Sas.SpotLight[0].Position" );
    //table->PromoteToParent( L"Sas.SpotLight[0].Direction" );
    //table->PromoteToParent( L"Sas.SpotLight[0].Range" );
    //table->PromoteToParent( L"Sas.SpotLight[0].Theta" );
    //table->PromoteToParent( L"Sas.SpotLight[0].Phi" );
}
void Render::UpdateRootBoundValueTable()
{
    Sas::BoundValueTable* table = Sas::Host::GetRootTable();

    static Sas::BindAddressHandle hSasSkeletonNumJoints = WCharToBindAddressHandle( L"Sas.Skeleton.NumJoints" );

    static Sas::BindAddressHandle hSasEnvironmentMap = WCharToBindAddressHandle( L"Sas.EnvironmentMap" );

    static Sas::BindAddressHandle hSasCameraWorldToView = WCharToBindAddressHandle( L"Sas.Camera.WorldToView" );
    static Sas::BindAddressHandle hSasCameraProjection = WCharToBindAddressHandle( L"Sas.Camera.Projection" );
    static Sas::BindAddressHandle hSasCameraNearFarClipping = WCharToBindAddressHandle( L"Sas.Camera.NearFarClipping" );
    static Sas::BindAddressHandle hSasCameraPosition = WCharToBindAddressHandle( L"Sas.Camera.Position" );
     static Sas::BindAddressHandle hSasTimeLast = WCharToBindAddressHandle( L"Sas.Time.Last" );
     static Sas::BindAddressHandle hSasTimeNow = WCharToBindAddressHandle( L"Sas.Time.Now" );
     static Sas::BindAddressHandle hSasTimeFrameNumer = WCharToBindAddressHandle( L"Sas.Time.FrameNumer" );

    static Sas::BindAddressHandle hSasAmbientLight0 = WCharToBindAddressHandle( L"Sas.AmbientLight[0]" );
     static Sas::BindAddressHandle hSasNumAmbientLights = WCharToBindAddressHandle( L"Sas.NumAmbientLights" );
     
    static Sas::BindAddressHandle hSasDirectionalLight0 = WCharToBindAddressHandle( L"Sas.DirectionalLight[0]" );
     static Sas::BindAddressHandle hSasNumDirectionalLights = WCharToBindAddressHandle( L"Sas.NumDirectionalLights" );

    static Sas::BindAddressHandle hSasPointLight0 = WCharToBindAddressHandle( L"Sas.PointLight[0]" );
     static Sas::BindAddressHandle hSasNumPointLights = WCharToBindAddressHandle( L"Sas.NumPointLights" );

    static Sas::BindAddressHandle hSasSpotLight0 = WCharToBindAddressHandle( L"Sas.SpotLight[0]" );
     static Sas::BindAddressHandle hSasNumSpotLights = WCharToBindAddressHandle( L"Sas.NumSpotLights" );

    table->SetInt( hSasSkeletonNumJoints, 1 );
    
    table->SetValue( hSasEnvironmentMap, Sas::Object(Sas::String( STATE_GET(Environment).c_str() )) );

    table->SetMatrix( hSasCameraWorldToView, DXVGetCamera().GetViewMatrix() );
    table->SetMatrix( hSasCameraProjection, DXVGetCamera().GetProjMatrix() );

    Sas::Vector nearFarClipping(Sas::SASVT_FLOAT, 2);
    nearFarClipping.Set( 0, DXVGetCamera().GetNearClip() );
    nearFarClipping.Set( 1, DXVGetCamera().GetFarClip() );

    table->SetValue( hSasCameraNearFarClipping, nearFarClipping );
    table->SetValue( hSasCameraPosition, Sas::Vector( DXVGetCamera().GetEyePt() ) );

    table->SetFloat( hSasTimeLast, DXVGetApp().LastRenderTime );
    table->SetFloat( hSasTimeNow, (float)DXUTGetTime() );
    table->SetInt( hSasTimeFrameNumer, DXVGetApp().RenderCount );


    // Lights
    Sas::Struct ambientLight(1);
    ambientLight.Set(0, Sas::Vector( &D3DXVECTOR3(0.1f, 0.1f, 0.1f) ));
    table->SetValue( hSasAmbientLight0, ambientLight );
    table->SetInt( hSasNumAmbientLights, 1 );

    Sas::Struct directionalLight(2);
    D3DXVECTOR3 direction;
    D3DXVec3Normalize( &direction, &D3DXVECTOR3(0.0f, 0.0f, 1.0f) );
    directionalLight.Set(0, Sas::Vector( &D3DXVECTOR3(0.9f, 0.9f, 0.9f) ));
    directionalLight.Set(1, Sas::Vector( &direction ));
    table->SetValue( hSasDirectionalLight0, directionalLight );
    table->SetInt( hSasNumDirectionalLights, 1 );

    Sas::Struct pointLight(3);
    pointLight.Set(0, Sas::Vector( &D3DXVECTOR3(0.9f, 0.9f, 0.9f) ));
    pointLight.Set(1, Sas::Vector( DXVGetCamera().GetEyePt() ));
    pointLight.Set(2, Sas::Float(1000.0f) );
    table->SetValue( hSasPointLight0, pointLight );
    table->SetInt( hSasNumPointLights, 1 );

    Sas::Struct spotLight(6);
    D3DXVECTOR3 vSpotDirection;
    D3DXVec3Normalize( &vSpotDirection, &D3DXVECTOR3(*DXVGetCamera().GetLookAtPt() - *DXVGetCamera().GetEyePt()) );
    spotLight.Set(0, Sas::Vector( &D3DXVECTOR3(0.9f, 0.9f, 0.9f) ));
    spotLight.Set(1, Sas::Vector( DXVGetCamera().GetEyePt() ));
    spotLight.Set(2, Sas::Vector( &vSpotDirection ) );
    spotLight.Set(3, Sas::Float(1000.0f) );
    spotLight.Set(4, Sas::Float(D3DX_PI/3.0f) );
    spotLight.Set(5, Sas::Float(D3DX_PI/2.0f) );
    table->SetValue( hSasSpotLight0, pointLight );
    table->SetInt( hSasNumSpotLights, 1 );
}

void Render::UpdateBoundValueTable( Sas::BoundValueTable* table, D3DXMATRIX& WorldMatrix  )
{
    static Sas::BindAddressHandle hSasSkeletonMeshToJointToWorld0 = WCharToBindAddressHandle( L"Sas.Skeleton.MeshToJointToWorld[0]" );
    table->SetMatrix( hSasSkeletonMeshToJointToWorld0, &WorldMatrix );
}

D3DXEFFECTINSTANCE* Render::GetDefaultEffectInstance()
{
    static bool ranOnce = false;
    static D3DXEFFECTINSTANCE effectInstance;

    if(ranOnce == false)
    {
        ranOnce = true;

        static char effectFilename[MAX_PATH] = "";
        static D3DXEFFECTDEFAULT effectDefaults[5];
        static char AmbientName[] = "MaterialAmbient";
        static char DiffuseName[] = "MaterialDiffuse";
        static char EmissiveName[] = "MaterialEmissive";
        static char SpecularName[] = "MaterialSpecular";
        static char PowerName[] = "MaterialPower";

        //StringCchPrintfA( effectFilename, MAX_PATH, "#%d", IDR_DEFAULTFX );

        effectInstance.pEffectFilename = effectFilename;
        effectInstance.NumDefaults = sizeof(effectDefaults)/sizeof(D3DXEFFECTDEFAULT);
        effectInstance.pDefaults = effectDefaults;

        effectDefaults[0].pParamName = AmbientName;
        effectDefaults[0].Type = D3DXEDT_FLOATS;
        effectDefaults[0].pValue = (FLOAT*) &(GetDefaultMaterial()->MatD3D.Ambient);
        effectDefaults[0].NumBytes = sizeof(D3DXCOLOR);

        effectDefaults[1].pParamName = DiffuseName;
        effectDefaults[1].Type = D3DXEDT_FLOATS;
        effectDefaults[1].pValue = (FLOAT*) &(GetDefaultMaterial()->MatD3D.Diffuse);
        effectDefaults[1].NumBytes = sizeof(D3DXCOLOR);

        effectDefaults[2].pParamName = SpecularName;
        effectDefaults[2].Type = D3DXEDT_FLOATS;
        effectDefaults[2].pValue = (FLOAT*) &(GetDefaultMaterial()->MatD3D.Specular);
        effectDefaults[2].NumBytes = sizeof(D3DXCOLOR);

        effectDefaults[3].pParamName = EmissiveName;
        effectDefaults[3].Type = D3DXEDT_FLOATS;
        effectDefaults[3].pValue = (FLOAT*) &(GetDefaultMaterial()->MatD3D.Emissive);
        effectDefaults[3].NumBytes = sizeof(D3DXCOLOR);

        effectDefaults[4].pParamName = PowerName;
        effectDefaults[4].Type = D3DXEDT_FLOATS;
        effectDefaults[4].pValue = &(GetDefaultMaterial()->MatD3D.Power);
        effectDefaults[4].NumBytes = sizeof(float);
    }

    return &effectInstance;
}

D3DXMATERIAL* Render::GetDefaultMaterial()
{
    static bool ranOnce = false;
    static D3DXMATERIAL material;

    if(ranOnce == false)
    {
        ranOnce = true;

        material.pTextureFilename = NULL;
        material.MatD3D.Ambient = D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f );
        material.MatD3D.Diffuse = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
        material.MatD3D.Emissive = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f );
        material.MatD3D.Specular = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
        material.MatD3D.Power = 16;
    }

    return &material;
}
