
#include "StdAfx.h"

D3DXMATRIX& ConvertGMatrix(D3DXMATRIX& dst, const GMatrix& src)
{
    for (int r=0; r<4; r++)
        for (int c=0; c<4; c++)
            dst(r, c) = src[r][c];

    return dst;
}

GMatrix& ConvertD3dMatrix(GMatrix& dst, const D3DXMATRIX& src)
{
    for (int r=0; r<4; r++)
        for (int c=0; c<4; c++)
            dst[r][c] = src(r, c);

    return dst;
}


const char* MakeExportableName(const char* name, char* buf, int bufCount)
{
    if (SUCCEEDED(StringCchCopy(buf, bufCount, name)))
    {
        for (char* c=buf; *c; c++)
        {
            if (!isalnum(*c) && *c != '_')
            {
                *c = '_';
            }
        }
    }
    else
    {
        buf[0] = 0;
    }

    return buf;
}

const char* ParamTypeName(int type)
{
    switch (type)
    {
    case TYPE_FLOAT: return "float";
    case TYPE_INT: return "int";
    case TYPE_RGBA: return "rgba";
    case TYPE_POINT3: return "point3";
    case TYPE_BOOL: return "bool";
	case TYPE_ANGLE: return "angle";
	case TYPE_PCNT_FRAC: return "pcntfrac";
	case TYPE_WORLD: return "world";
	case TYPE_STRING: return "string";
	case TYPE_FILENAME: return "filename";
	case TYPE_HSV: return "hsv";
	case TYPE_COLOR_CHANNEL: return "colorchannel";
	case TYPE_TIMEVALUE: return "timevalue";
	case TYPE_RADIOBTN_INDEX: return "radiobuttonindex";
	case TYPE_MTL: return "mtl";
	case TYPE_TEXMAP: return "texmap";
	case TYPE_BITMAP: return "bitmap";
	case TYPE_INODE: return "node";
	case TYPE_REFTARG: return "referencetarget";
	case TYPE_INDEX: return "index";
	case TYPE_MATRIX3: return "matrix3";
	case TYPE_PBLOCK2: return "pblock2";
	case TYPE_POINT4: return "point4";
	case TYPE_FRGBA: return "frgba";
	case TYPE_FLOAT_TAB: return "floattab";
	case TYPE_INT_TAB: return "inttab";
	case TYPE_RGBA_TAB: return "rgbatab";
	case TYPE_POINT3_TAB: return "point3tab";
	case TYPE_BOOL_TAB: return "booltab";
	case TYPE_ANGLE_TAB: return "angletab";
	case TYPE_PCNT_FRAC_TAB: return "pcntfractab";
	case TYPE_WORLD_TAB: return "worldtab";
	case TYPE_STRING_TAB: return "stringtab";
	case TYPE_FILENAME_TAB: return "filenametab";
	case TYPE_HSV_TAB: return "hsvtab";
	case TYPE_COLOR_CHANNEL_TAB: return "colorchanneltab";
	case TYPE_TIMEVALUE_TAB: return "timevaluetab";
	case TYPE_RADIOBTN_INDEX_TAB: return "radiobuttonindextab";
	case TYPE_MTL_TAB: return "mtltab";
	case TYPE_TEXMAP_TAB: return "texmaptab";
	case TYPE_BITMAP_TAB: return "bitmaptab";
	case TYPE_INODE_TAB: return "nodetab";
	case TYPE_REFTARG_TAB: return "referencetargettab";
	case TYPE_INDEX_TAB: return "indextab";
	case TYPE_MATRIX3_TAB: return "matrix3tab";
	case TYPE_PBLOCK2_TAB: return "pblock2tab";
	case TYPE_POINT4_TAB: return "point4tab";
	case TYPE_FRGBA_TAB: return "frgbatab";
    default: return "unknown type";
    }
}

bool FindMaterialParameter(Mtl* mtl, const TCHAR* paramName, IParamBlock2** opb, ParamID* opid)
{
    int numPbs = mtl->NumParamBlocks();
    for (int pb=0; pb<numPbs; pb++)
    {
        IParamBlock2* pb2 = mtl->GetParamBlock(pb);
        for (int p=0; p<pb2->NumParams(); p++)
        {
            ParamID pid = pb2->IndextoID(p);
            ParamDef& def = pb2->GetParamDef(pid);

            if (_tcsicmp(def.int_name, paramName) == 0)
            {
                *opb = pb2;
                *opid = pid;

                return true;
            }
        }
    }

    return false;
}

