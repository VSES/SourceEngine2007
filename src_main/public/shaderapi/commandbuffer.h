//===== Copyright © 1996-2007, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
// This file defines a number of constants and structured which are used to build up a command
// buffer to pass to ShaderAPI for state setting and other operations.  Since the prupose of these
// command buffers is to minimize and optimize calls into shaderapi, their structure is not
// abstract - they are built out by the calling process.
//
//===========================================================================//

#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H


#ifdef _WIN32
#pragma once
#endif

// all commands defined with their struct
enum CommandBufferCommand_t
{
	// flow control commands.
	CBCMD_END = 0,											// end of stream
	CBCMD_JUMP = 1,											// int cmd, void *adr. jump to another
															// stream. Can be used to implement
															// non-sequentially allocated storage
	CBCMD_JSR = 2,											// int cmd, void *adr. subroutine call to another stream.

	// constant setting commands
	CBCMD_SET_PIXEL_SHADER_FLOAT_CONST = 256, // int cmd,int first_reg, int nregs, float values[nregs*4]


	CBCMD_SET_VERTEX_SHADER_FLOAT_CONST = 257, // int cmd,int first_reg, int nregs, float values[nregs*4]
	CBCMD_SET_VERTEX_SHADER_FLOAT_CONST_REF = 258, // int cmd,int first_reg, int nregs, &float values[nregs*4]
	CBCMD_SETPIXELSHADERFOGPARAMS = 259,					// int cmd, int regdest
	CBCMD_STORE_EYE_POS_IN_PSCONST = 260,					// int cmd, int regdest
	CBCMD_COMMITPIXELSHADERLIGHTING = 261,					// int cmd, int regdest
	CBCMD_SETPIXELSHADERSTATEAMBIENTLIGHTCUBE = 262,		// int cmd, int regdest
	CBCMD_SETAMBIENTCUBEDYNAMICSTATEVERTEXSHADER = 263,		// int cmd
	CBCMD_SET_DEPTH_FEATHERING_CONST = 264,					// int cmd, int constant register, float blend scale

	// texture binding
	CBCMD_BIND_STANDARD_TEXTURE = 512,						// cmd, sampler, texture id
	CBCMD_BIND_SHADERAPI_TEXTURE_HANDLE = 513,				// cmd, sampler, texture handle

	// shaders
	CBCMD_SET_PSHINDEX = 1024,								// cmd, idx
	CBCMD_SET_VSHINDEX = 1025,								// cmd, idx


};










#endif // commandbuffer_h
