//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stdafx.h"
//#include <vector>
using namespace std;

namespace Sas
{
    enum Control
    {
        Control_None,
        Control_Any,
        Control_Numeric,
        Control_Slider,
        Control_Direction,
        Control_String,
        Control_ColorPicker,
        Control_FilePicker,
        Control_ListPicker,

        Control_END
    };

    extern const WCHAR* ControlStrings[];
}

#endif //_CONTROL_H_