//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef _SAS_H_
#define _SAS_H_
#pragma warning(disable: 4995)
#include <string>
#pragma warning(default: 4995)


namespace Sas
{
    enum SASVARIABLE_TYPE 
    {
        SASVT_BOOL,
        SASVT_INT,
        SASVT_FLOAT,
        SASVT_STRING,
        SASVT_TEXTURE,
        SASVT_TEXTURE1D,
        SASVT_TEXTURE2D,
        SASVT_TEXTURE3D,
        SASVT_TEXTURECUBE,
        SASVT_INVALID = -1,
    }; 

    enum SASVARIABLE_CLASS
    {
        SASVC_SCALAR,
        SASVC_VECTOR,
        SASVC_MATRIX,
        SASVC_OBJECT,
        SASVC_STRUCT,
        SASVC_ARRAY,
        SASVC_INVALID = -1, 
    };


    void ReleaseStaticResources();


}

#include "DataType.h"
#include "Unit.h"
//#include "ResourceCache.h"
#include "FilePath.h"
#include "Value.h"
#include "BindAddress.h"
#include "BoundValueTable.h"
#include "Host.h"
#include "Parameter.h"
#include "Effect.h"
#include "Integrator.h"
#include "Effect9.h"
#include "Effect10.h"

#endif //_SAS_H_