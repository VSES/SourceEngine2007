push @output, "vs.1.1																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(1)\n";
# DYNAMIC: "DOWATERFOG"				"0..1"																								
push @output, ";------------------------------------																								; LINEINFO(macros.vsh)(1)\n";
push @output, "; RULES FOR AUTHORING VERTEX SHADERS:																								; LINEINFO(macros.vsh)(2)\n";
push @output, ";------------------------------------																								; LINEINFO(macros.vsh)(3)\n";
push @output, "; - never use \"def\" . . .set constants in code instead. . our constant shadowing will break otherwise.																								; LINEINFO(macros.vsh)(4)\n";
 	push @output, ";	(same goes for pixel shaders)																								; LINEINFO(macros.vsh)(5)\n";
push @output, "; - use cN notation instead of c[N] notation. .makes grepping for registers easier.																								; LINEINFO(macros.vsh)(6)\n";
    push @output, ";   The only exception is c[a0.x+blah] where you have no choice.																								; LINEINFO(macros.vsh)(7)\n";
$g_NumRegisters = 12;																								
push @output, "; NOTE: These must match the same values in vsh_prep.pl!																								; LINEINFO(macros.vsh)(10)\n";
$vPos				= "v0";																								
$vBoneWeights		= "v1";																								
$vBoneIndices		= "v2";																								
$vNormal			= "v3";																								
$vColor				= "v5";																								
$vSpecular			= "v6";																								
$vTexCoord0			= "v7";																								
$vTexCoord1			= "v8";																								
$vTexCoord2			= "v9";																								
$vTexCoord3			= "v10";																								
$vTangentS			= "v11";																								
$vTangentT			= "v12";																								
$vUserData			= "v14";																								
if( $g_dx9 )																								
{																								
	if( $g_usesPos )																								
	{																								
		push @output, "		dcl_position $vPos;																								; LINEINFO(macros.vsh)(29)\n";
	}																								
	if( $g_usesBoneWeights )																								
	{																								
		push @output, "		dcl_blendweight $vBoneWeights;																								; LINEINFO(macros.vsh)(34)\n";
	}																								
	if( $g_usesBoneIndices )																								
	{																								
		push @output, "		dcl_blendindices $vBoneIndices;																								; LINEINFO(macros.vsh)(38)\n";
	}																								
	if( $g_usesNormal )																								
	{																								
		push @output, "		dcl_normal $vNormal;																								; LINEINFO(macros.vsh)(42)\n";
	}																								
	if( $g_usesColor )																								
	{																								
		push @output, "		dcl_color0 $vColor;																								; LINEINFO(macros.vsh)(46)\n";
	}																								
	if( $g_usesSpecular )																								
	{																								
		push @output, "		dcl_color1 $vSpecular;																								; LINEINFO(macros.vsh)(50)\n";
	}																								
	if( $g_usesTexCoord0 )																								
	{																								
		push @output, "		dcl_texcoord0 $vTexCoord0;																								; LINEINFO(macros.vsh)(54)\n";
	}																								
	if( $g_usesTexCoord1 )																								
	{																								
		push @output, "		dcl_texcoord1 $vTexCoord1;																								; LINEINFO(macros.vsh)(58)\n";
	}																								
	if( $g_usesTexCoord2 )																								
	{																								
		push @output, "		dcl_texcoord2 $vTexCoord2;																								; LINEINFO(macros.vsh)(62)\n";
	}																								
	if( $g_usesTexCoord3 )																								
	{																								
		push @output, "		dcl_texcoord3 $vTexCoord3;																								; LINEINFO(macros.vsh)(66)\n";
	}																								
	if( $g_usesTangentS )																								
	{																								
		push @output, "		dcl_tangent $vTangentS;																								; LINEINFO(macros.vsh)(70)\n";
	}																								
	if( $g_usesTangentT )																								
	{																								
		push @output, "		dcl_binormal0 $vTangentT;																								; LINEINFO(macros.vsh)(74)\n";
	}																								
	if( $g_usesUserData )																								
	{																								
		push @output, "		dcl_tangent $vUserData;																								; LINEINFO(macros.vsh)(78)\n";
	}																								
}																								
# NOTE: These should match g_LightCombinations in vertexshaderdx8.cpp!																								
# NOTE: Leave this on single lines or shit might blow up.																								
@g_staticLightTypeArray = ( "none", "static", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "static", "static", "static", "static", "static", "static", "static", "static", "static", "static" );																								
@g_ambientLightTypeArray = ( "none", "none", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", 	"ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient", "ambient" );																								
@g_localLightType1Array = ( "none", "none", "none", "spot", "point", "directional", "spot", "spot", "spot", "point", "point", "directional", "none", "spot", "point", "directional", "spot", "spot", "spot", "point", "point", "directional" );																								
@g_localLightType2Array = ( "none", "none", "none", "none", "none", "none", "spot", "point", "directional", "point", "directional", "directional", "none", "none", "none", "none", "spot", "point", "directional", "point", "directional", "directional" );																								
$cConstants0		= "c0";																								
$cZero				= "c0.x";																								
$cOne				= "c0.y";																								
$cTwo				= "c0.z";																								
$cHalf				= "c0.w";																								
$cConstants1		    = "c1";																								
$cOOGamma			    = "c1.x"; # 1/2.2																								
$cOtherOverbrightFactor = "c1.y"; # overbright																								
$cOneThird			    = "c1.z"; # 1/3																								
$cOverbrightFactor      = "c1.w"; # 1/overbright																								
$cEyePos			= "c2";																								
$cWaterZ			= "c2.w";																								
$cEyePosWaterZ		= "c2";																								
$cLightIndex		= "c3";																								
$cLight0Offset		= "c3.x"; # 27																								
$cLight1Offset		= "c3.y"; # 32																								
$cColorToIntScale	= "c3.z"; # matrix array offset = 3.0f * 255.0f + 0.01 (epsilon ensures floor yields desired result)																								
$cModel0Index		= "c3.w"; # base for start of skinning matrices																								
push @output, "; NOTE: These must match the same values in vsh_prep.pl!																								; LINEINFO(macros.vsh)(111)\n";
$cModelViewProj0	= "c4";																								
$cModelViewProj1	= "c5";																								
$cModelViewProj2	= "c6";																								
$cModelViewProj3	= "c7";																								
$cViewProj0			= "c8";																								
$cViewProj1			= "c9";																								
$cViewProj2			= "c10";																								
$cViewProj3			= "c11";																								
push @output, "; currently unused																								; LINEINFO(macros.vsh)(122)\n";
push @output, "; c12, c13																								; LINEINFO(macros.vsh)(123)\n";
$SHADER_SPECIFIC_CONST_10 = "c14";																								
$SHADER_SPECIFIC_CONST_11 = "c15";																								
$cFogParams			= "c16";																								
$cFogEndOverFogRange = "c16.x";																								
$cFogOne			= "c16.y";																								
$cFogMaxDensity		= "c16.z";																								
$cOOFogRange		= "c16.w"; # (1/(fogEnd-fogStart))																								
$cViewModel0		= "c17";																								
$cViewModel1		= "c18";																								
$cViewModel2		= "c19";																								
$cViewModel3		= "c20";																								
$cAmbientColorPosX	= "c21";																								
$cAmbientColorNegX	= "c22";																								
$cAmbientColorPosY	= "c23";																								
$cAmbientColorNegY	= "c24";																								
$cAmbientColorPosZ	= "c25";																								
$cAmbientColorNegZ	= "c26";																								
$cAmbientColorPosXOffset = "21";																								
$cAmbientColorPosYOffset = "23";																								
$cAmbientColorPosZOffset = "25";																								
$cLight0DiffColor	= "c27";																								
$cLight0Dir			= "c28";																								
$cLight0Pos			= "c29";																								
$cLight0SpotParams  = "c30"; # [ exponent, stopdot, stopdot2, 1 / (stopdot - stopdot2)																								
$cLight0Atten		= "c31"; # [ constant, linear, quadratic, 0.0f ]																								
$cLight1DiffColor	= "c32";																								
$cLight1Dir			= "c33";																								
$cLight1Pos			= "c34";																								
$cLight1SpotParams  = "c35"; # [ exponent, stopdot, stopdot2, 1 / (stopdot - stopdot2)																								
$cLight1Atten		= "c36"; # [ constant, linear, quadratic, 0.0f ]																								
$cModulationColor	= "c37";																								
$SHADER_SPECIFIC_CONST_0  = "c38";																								
$SHADER_SPECIFIC_CONST_1  = "c39";																								
$SHADER_SPECIFIC_CONST_2  = "c40";																								
$SHADER_SPECIFIC_CONST_3  = "c41";																								
$SHADER_SPECIFIC_CONST_4  = "c42";																								
$SHADER_SPECIFIC_CONST_5  = "c43";																								
$SHADER_SPECIFIC_CONST_6  = "c44";																								
$SHADER_SPECIFIC_CONST_7  = "c45";																								
$SHADER_SPECIFIC_CONST_8  = "c46";																								
$SHADER_SPECIFIC_CONST_9  = "c47";																								
push @output, "; $SHADER_SPECIFIC_CONST_10 is c14																								; LINEINFO(macros.vsh)(174)\n";
push @output, "; $SHADER_SPECIFIC_CONST_11 is c15																								; LINEINFO(macros.vsh)(175)\n";
push @output, "; There are 16 model matrices for skinning																								; LINEINFO(macros.vsh)(177)\n";
push @output, "; NOTE: These must match the same values in vsh_prep.pl!																								; LINEINFO(macros.vsh)(178)\n";
$cModel0			= "c48";																								
$cModel1			= "c49";																								
$cModel2			= "c50";																								
sub OutputUsedRegisters																								
{																								
	local( $i );																								
	push @output, "	; USED REGISTERS																								; LINEINFO(macros.vsh)(186)\n";
	for( $i = 0; $i < $g_NumRegisters; $i++ )																								
	{																								
		if( $g_allocated[$i] )																								
		{																								
			push @output, "			; $g_allocatedname[$i] = r$i																								; LINEINFO(macros.vsh)(191)\n";
		}																								
	}																								
	push @output, "	;																								; LINEINFO(macros.vsh)(194)\n";
}																								
sub AllocateRegister																								
{																								
	local( *reg ) = shift;																								
	local( $regname ) = shift;																								
	local( $i );																								
	for( $i = 0; $i < $g_NumRegisters; $i++ )																								
	{																								
		if( !$g_allocated[$i] )																								
		{																								
			$g_allocated[$i] = 1;																								
			$g_allocatedname[$i] = $regname;																								
			push @output, "			; AllocateRegister $regname = r$i																								; LINEINFO(macros.vsh)(208)\n";
			$reg = "r$i";																								
			&OutputUsedRegisters();																								
			return;																								
		}																								
	}																								
	push @output, "	; Out of registers allocating $regname!																								; LINEINFO(macros.vsh)(214)\n";
	$reg = "rERROR_OUT_OF_REGISTERS";																								
	&OutputUsedRegisters();																								
}																								
push @output, "; pass in a reference to a var that contains a register. . ie \$var where var will constain \"r1\", etc																								; LINEINFO(macros.vsh)(219)\n";
sub FreeRegister																								
{																								
	local( *reg ) = shift;																								
	local( $regname ) = shift;																								
	push @output, "	; FreeRegister $regname = $reg																								; LINEINFO(macros.vsh)(224)\n";
	if( $reg =~ m/rERROR_DEALLOCATED/ )																								
	{																								
		push @output, "		; $regname already deallocated																								; LINEINFO(macros.vsh)(227)\n";
		push @output, "		; $reg = \"rALREADY_DEALLOCATED\";																								; LINEINFO(macros.vsh)(228)\n";
		&OutputUsedRegisters();																								
		return;																								
	}																								
 	push @output, ";	if( $regname ne g_allocatedname[$reg] )																								; LINEINFO(macros.vsh)(232)\n";
 	push @output, ";	{																								; LINEINFO(macros.vsh)(233)\n";
 		push @output, ";		; Error freeing $reg																								; LINEINFO(macros.vsh)(234)\n";
 		push @output, ";		mov compileerror, freed unallocated register $regname																								; LINEINFO(macros.vsh)(235)\n";
 	push @output, ";	}																								; LINEINFO(macros.vsh)(236)\n";
	if( ( $reg =~ m/r(.*)/ ) )																								
	{																								
		$g_allocated[$1] = 0;																								
	}																								
	$reg = "rERROR_DEALLOCATED";																								
	&OutputUsedRegisters();																								
}																								
sub CheckUnfreedRegisters()																								
{																								
	local( $i );																								
	for( $i = 0; $i < $g_NumRegisters; $i++ )																								
	{																								
		if( $g_allocated[$i] )																								
		{																								
			print "ERROR: r$i allocated to $g_allocatedname[$i] at end of program\n";																								
			$g_allocated[$i] = 0;																								
		}																								
	}																								
}																								
sub Normalize																								
{																								
	local( $r ) = shift;																								
	push @output, "	dp3 $r.w, $r, $r																								; LINEINFO(macros.vsh)(262)\n";
	push @output, "	rsq $r.w, $r.w																								; LINEINFO(macros.vsh)(263)\n";
	push @output, "	mul $r, $r, $r.w																								; LINEINFO(macros.vsh)(264)\n";
}																								
sub Cross																								
{																								
	local( $result ) = shift;																								
	local( $a ) = shift;																								
	local( $b ) = shift;																								
	push @output, "	mul $result.xyz, $a.yzx, $b.zxy																								; LINEINFO(macros.vsh)(273)\n";
	push @output, "	mad $result.xyz, -$b.yzx, $a.zxy, $result																								; LINEINFO(macros.vsh)(274)\n";
}																								
sub RangeFog																								
{																								
	local( $projPos ) = shift;																								
	push @output, "	;------------------------------																								; LINEINFO(macros.vsh)(281)\n";
	push @output, "	; Regular range fog																								; LINEINFO(macros.vsh)(282)\n";
	push @output, "	;------------------------------																								; LINEINFO(macros.vsh)(283)\n";
	push @output, "	; oFog.x = 1.0f = no fog																								; LINEINFO(macros.vsh)(285)\n";
	push @output, "	; oFog.x = 0.0f = full fog																								; LINEINFO(macros.vsh)(286)\n";
	push @output, "	; compute fog factor f = (fog_end - dist)*(1/(fog_end-fog_start))																								; LINEINFO(macros.vsh)(287)\n";
	push @output, "	; this is == to: (fog_end/(fog_end-fog_start) - dist/(fog_end-fog_start)																								; LINEINFO(macros.vsh)(288)\n";
	push @output, "	; which can be expressed with a single mad instruction!																								; LINEINFO(macros.vsh)(289)\n";
	push @output, "	; Compute |projPos|																								; LINEINFO(macros.vsh)(291)\n";
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	dp3 $tmp.x, $projPos.xyw, $projPos.xyw																								; LINEINFO(macros.vsh)(294)\n";
	push @output, "	rsq $tmp.x, $tmp.x																								; LINEINFO(macros.vsh)(295)\n";
	push @output, "	rcp $tmp.x, $tmp.x																								; LINEINFO(macros.vsh)(296)\n";
	if( $g_dx9 )																								
	{																								
		push @output, "		mad $tmp, -$tmp.x, $cOOFogRange, $cFogEndOverFogRange																								; LINEINFO(macros.vsh)(300)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(301)\n";
		push @output, "		max oFog, $tmp.x, $cFogMaxDensity																								; LINEINFO(macros.vsh)(302)\n";
	}																								
	else																								
	{																								
		push @output, "		mad $tmp, -$tmp.x, $cOOFogRange, $cFogEndOverFogRange																								; LINEINFO(macros.vsh)(306)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(307)\n";
		push @output, "		max oFog.x, $tmp.x, $cFogMaxDensity																								; LINEINFO(macros.vsh)(308)\n";
	}																								
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub DepthFog																								
{																								
	local( $projPos ) = shift;																								
	local( $dest ) = shift;																								
	if ( $dest eq "" )																								
	{																								
		$dest = "oFog";																								
	}																								
	push @output, "	;------------------------------																								; LINEINFO(macros.vsh)(323)\n";
	push @output, "	; Regular range fog																								; LINEINFO(macros.vsh)(324)\n";
	push @output, "	;------------------------------																								; LINEINFO(macros.vsh)(325)\n";
	push @output, "	; oFog.x = 1.0f = no fog																								; LINEINFO(macros.vsh)(327)\n";
	push @output, "	; oFog.x = 0.0f = full fog																								; LINEINFO(macros.vsh)(328)\n";
	push @output, "	; compute fog factor f = (fog_end - dist)*(1/(fog_end-fog_start))																								; LINEINFO(macros.vsh)(329)\n";
	push @output, "	; this is == to: (fog_end/(fog_end-fog_start) - dist/(fog_end-fog_start)																								; LINEINFO(macros.vsh)(330)\n";
	push @output, "	; which can be expressed with a single mad instruction!																								; LINEINFO(macros.vsh)(331)\n";
	push @output, "	; Compute |projPos|																								; LINEINFO(macros.vsh)(333)\n";
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" );																								
	if( $g_dx9 )																								
	{																								
		push @output, "		mad $tmp, -$projPos.w, $cOOFogRange, $cFogEndOverFogRange																								; LINEINFO(macros.vsh)(339)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(340)\n";
		push @output, "		max $dest, $tmp.x, $cFogMaxDensity																								; LINEINFO(macros.vsh)(341)\n";
	}																								
	else																								
	{																								
		push @output, "		mad $tmp, -$projPos.w, $cOOFogRange, $cFogEndOverFogRange																								; LINEINFO(macros.vsh)(345)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(346)\n";
		push @output, "		max $dest.x, $tmp.x, $cFogMaxDensity																								; LINEINFO(macros.vsh)(347)\n";
	}																								
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub WaterRangeFog																								
{																								
	push @output, "	; oFog.x = 1.0f = no fog																								; LINEINFO(macros.vsh)(355)\n";
	push @output, "	; oFog.x = 0.0f = full fog																								; LINEINFO(macros.vsh)(356)\n";
	push @output, "	; only $worldPos.z is used out of worldPos																								; LINEINFO(macros.vsh)(358)\n";
	local( $worldPos ) = shift;																								
	local( $projPos ) = shift;																								
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	; This is simple similar triangles. Imagine a line passing from the point directly vertically																								; LINEINFO(macros.vsh)(365)\n";
	push @output, "	; and another line passing from the point to the eye position.																								; LINEINFO(macros.vsh)(366)\n";
	push @output, "	; Let d = total distance from point to the eye																								; LINEINFO(macros.vsh)(367)\n";
	push @output, "	; Let h = vertical distance from the point to the eye																								; LINEINFO(macros.vsh)(368)\n";
	push @output, "	; Let hw = vertical distance from the point to the water surface																								; LINEINFO(macros.vsh)(369)\n";
	push @output, "	; Let dw = distance from the point to a point on the water surface that lies along the ray from point to eye																								; LINEINFO(macros.vsh)(370)\n";
	push @output, "	; Therefore d/h = dw/hw by similar triangles, or dw = d * hw / h.																								; LINEINFO(macros.vsh)(371)\n";
	push @output, "	; d = |projPos|, h = eyepos.z - worldPos.z, hw = waterheight.z - worldPos.z, dw = what we solve for																								; LINEINFO(macros.vsh)(372)\n";
	push @output, "	; Now, tmp.x = hw, and tmp.y = h																								; LINEINFO(macros.vsh)(374)\n";
	push @output, "	add $tmp.xy, $cEyePosWaterZ.wz, -$worldPos.z																								; LINEINFO(macros.vsh)(375)\n";
	push @output, "	; if $tmp.x < 0, then set it to 0																								; LINEINFO(macros.vsh)(377)\n";
	push @output, "	; This is the equivalent of moving the vert to the water surface if it's above the water surface																								; LINEINFO(macros.vsh)(378)\n";
	push @output, "	max $tmp.x, $tmp.x, $cZero																								; LINEINFO(macros.vsh)(379)\n";
	push @output, "	; Compute 1 / |projPos| = 1/d																								; LINEINFO(macros.vsh)(381)\n";
	push @output, "	dp3 $tmp.z, $projPos.xyw, $projPos.xyw																								; LINEINFO(macros.vsh)(382)\n";
	push @output, "	rsq $tmp.z, $tmp.z																								; LINEINFO(macros.vsh)(383)\n";
	push @output, "	; Now we have h/d																								; LINEINFO(macros.vsh)(385)\n";
	push @output, "	mul $tmp.z, $tmp.z, $tmp.y																								; LINEINFO(macros.vsh)(386)\n";
	push @output, "	; Now we have d/h																								; LINEINFO(macros.vsh)(388)\n";
	push @output, "	rcp $tmp.w, $tmp.z																								; LINEINFO(macros.vsh)(389)\n";
	push @output, "	; We finally have d * hw / h																								; LINEINFO(macros.vsh)(391)\n";
	push @output, "	; $tmp.w is now the distance that we see through water.																								; LINEINFO(macros.vsh)(392)\n";
	push @output, "	mul $tmp.w, $tmp.x, $tmp.w																								; LINEINFO(macros.vsh)(393)\n";
	if( $g_dx9 )																								
	{																								
		push @output, "		mad $tmp, -$tmp.w, $cOOFogRange, $cFogOne																								; LINEINFO(macros.vsh)(397)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(398)\n";
		push @output, "		max oFog, $tmp.x, $cFogMaxDensity																								; LINEINFO(macros.vsh)(399)\n";
	}																								
	else																								
	{																								
		push @output, "		mad $tmp, -$tmp.w, $cOOFogRange, $cFogOne																								; LINEINFO(macros.vsh)(403)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(404)\n";
		push @output, "		max oFog.x, $tmp.x, $cFogMaxDensity																								; LINEINFO(macros.vsh)(405)\n";
	}																								
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub WaterDepthFog																								
{																								
	push @output, "	; oFog.x = 1.0f = no fog																								; LINEINFO(macros.vsh)(413)\n";
	push @output, "	; oFog.x = 0.0f = full fog																								; LINEINFO(macros.vsh)(414)\n";
	push @output, "	; only $worldPos.z is used out of worldPos																								; LINEINFO(macros.vsh)(416)\n";
	local( $worldPos ) = shift;																								
	local( $projPos ) = shift;																								
	local( $dest ) = shift;																								
	if ( $dest eq "" )																								
	{																								
		$dest = "oFog";																								
	}																								
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	; This is simple similar triangles. Imagine a line passing from the point directly vertically																								; LINEINFO(macros.vsh)(429)\n";
	push @output, "	; and another line passing from the point to the eye position.																								; LINEINFO(macros.vsh)(430)\n";
	push @output, "	; Let d = total distance from point to the eye																								; LINEINFO(macros.vsh)(431)\n";
	push @output, "	; Let h = vertical distance from the point to the eye																								; LINEINFO(macros.vsh)(432)\n";
	push @output, "	; Let hw = vertical distance from the point to the water surface																								; LINEINFO(macros.vsh)(433)\n";
	push @output, "	; Let dw = distance from the point to a point on the water surface that lies along the ray from point to eye																								; LINEINFO(macros.vsh)(434)\n";
	push @output, "	; Therefore d/h = dw/hw by similar triangles, or dw = d * hw / h.																								; LINEINFO(macros.vsh)(435)\n";
	push @output, "	; d = projPos.w, h = eyepos.z - worldPos.z, hw = waterheight.z - worldPos.z, dw = what we solve for																								; LINEINFO(macros.vsh)(436)\n";
	push @output, "	; Now, tmp.x = hw, and tmp.y = h																								; LINEINFO(macros.vsh)(438)\n";
	push @output, "	add $tmp.xy, $cEyePosWaterZ.wz, -$worldPos.z																								; LINEINFO(macros.vsh)(439)\n";
	push @output, "	; if $tmp.x < 0, then set it to 0																								; LINEINFO(macros.vsh)(441)\n";
	push @output, "	; This is the equivalent of moving the vert to the water surface if it's above the water surface																								; LINEINFO(macros.vsh)(442)\n";
	push @output, "	max $tmp.x, $tmp.x, $cZero																								; LINEINFO(macros.vsh)(443)\n";
	push @output, "	; Now we have 1/h																								; LINEINFO(macros.vsh)(445)\n";
	push @output, "	rcp $tmp.z, $tmp.y																								; LINEINFO(macros.vsh)(446)\n";
	push @output, "	; Now we have d/h																								; LINEINFO(macros.vsh)(448)\n";
	push @output, "	mul $tmp.w, $projPos.w, $tmp.z																								; LINEINFO(macros.vsh)(449)\n";
	push @output, "	; We finally have d * hw / h																								; LINEINFO(macros.vsh)(451)\n";
	push @output, "	; $tmp.w is now the distance that we see through water.																								; LINEINFO(macros.vsh)(452)\n";
	push @output, "	mul $tmp.w, $tmp.x, $tmp.w																								; LINEINFO(macros.vsh)(453)\n";
	if( $g_dx9 )																								
	{																								
		push @output, "		mad $tmp, -$tmp.w, $cOOFogRange, $cFogOne																								; LINEINFO(macros.vsh)(457)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(458)\n";
		push @output, "		max $dest, $tmp.x, $cZero																								; LINEINFO(macros.vsh)(459)\n";
	}																								
	else																								
	{																								
		push @output, "		mad $tmp, -$tmp.w, $cOOFogRange, $cFogOne																								; LINEINFO(macros.vsh)(463)\n";
		push @output, "		min $tmp, $tmp, $cOne																								; LINEINFO(macros.vsh)(464)\n";
		push @output, "		max $dest.x, $tmp.x, $cZero																								; LINEINFO(macros.vsh)(465)\n";
	}																								
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(macros.vsh)(472)\n";
push @output, "; Main fogging routine																								; LINEINFO(macros.vsh)(473)\n";
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(macros.vsh)(474)\n";
sub CalcFog																								
{																								
	if( !defined $DOWATERFOG )																								
	{																								
		die "CalcFog called without using \$DOWATERFOG\n";																								
	}																								
	my $fogType;																								
	if( $DOWATERFOG == 0 )																								
	{																								
		$fogType = "rangefog";																										
	}																								
	else																								
	{																								
		$fogType = "heightfog";																								
	}																								
#	print "\$fogType = $fogType\n";																								
	push @output, "	; CalcFog																								; LINEINFO(macros.vsh)(493)\n";
	local( $worldPos ) = shift;																								
	local( $projPos ) = shift;																								
	local( $dest ) = shift;																								
	if ( $dest eq "" )																								
	{																								
		$dest = "oFog";																								
	}																								
	if( $fogType eq "rangefog" )																								
	{																								
		&DepthFog( $projPos, $dest );																								
	}																								
	elsif( $fogType eq "heightfog" )																								
	{																								
		&WaterDepthFog( $worldPos, $projPos, $dest );																								
	}																								
	else																								
	{																								
		die;																								
	}																									
}																								
sub CalcRangeFog																								
{																								
	push @output, "	; CalcFog																								; LINEINFO(macros.vsh)(519)\n";
	local( $worldPos ) = shift;																								
	local( $projPos ) = shift;																								
	if( $DOWATERFOG == 0 )																								
	{																								
		&RangeFog( $projPos );																								
	}																								
	elsif( $DOWATERFOG == 1 )																								
	{																								
		&WaterRangeFog( $worldPos, $projPos );																								
	}																								
	else																								
	{																								
		die;																								
	}																									
}																								
sub GammaToLinear																								
{																								
	local( $gamma ) = shift;																								
	local( $linear ) = shift;																								
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	; Is rcp more expensive than just storing 2.2 somewhere and doing a mov?																								; LINEINFO(macros.vsh)(545)\n";
	push @output, "	rcp $gamma.w, $cOOGamma							; $gamma.w = 2.2																								; LINEINFO(macros.vsh)(546)\n";
	push @output, "	lit $linear.z, $gamma.zzzw						; r0.z = linear blue																								; LINEINFO(macros.vsh)(547)\n";
	push @output, "	lit $tmp.z, $gamma.yyyw							; r2.z = linear green																								; LINEINFO(macros.vsh)(548)\n";
	push @output, "	mov $linear.y, $tmp.z							; r0.y = linear green																								; LINEINFO(macros.vsh)(549)\n";
	push @output, "	lit $tmp.z, $gamma.xxxw							; r2.z = linear red																								; LINEINFO(macros.vsh)(550)\n";
	push @output, "	mov $linear.x, $tmp.z							; r0.x = linear red																								; LINEINFO(macros.vsh)(551)\n";
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub LinearToGamma																								
{																								
	local( $linear ) = shift;																								
	local( $gamma ) = shift;																								
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	mov $linear.w, $cOOGamma						; $linear.w = 1.0/2.2																								; LINEINFO(macros.vsh)(564)\n";
	push @output, "	lit $gamma.z, $linear.zzzw						; r0.z = gamma blue																								; LINEINFO(macros.vsh)(565)\n";
	push @output, "	lit $tmp.z, $linear.yyyw						; r2.z = gamma green																								; LINEINFO(macros.vsh)(566)\n";
	push @output, "	mov $gamma.y, $tmp.z							; r0.y = gamma green																								; LINEINFO(macros.vsh)(567)\n";
	push @output, "	lit $tmp.z, $linear.xxxw						; r2.z = gamma red																								; LINEINFO(macros.vsh)(568)\n";
	push @output, "	mov $gamma.x, $tmp.z							; r0.x = gamma red																								; LINEINFO(macros.vsh)(569)\n";
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub ComputeReflectionVector																								
{																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	local( $reflectionVector ) = shift;																								
	local( $vertToEye ); &AllocateRegister( \$vertToEye, "\$vertToEye" );																								
	local( $tmp ); &AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	; compute reflection vector r = 2 * (n dot v) n - v																								; LINEINFO(macros.vsh)(583)\n";
	push @output, "	sub $vertToEye.xyz, $cEyePos.xyz, $worldPos  ; $tmp1 = v = c - p																								; LINEINFO(macros.vsh)(584)\n";
	push @output, "	dp3 $tmp, $worldNormal, $vertToEye			; $tmp = n dot v																								; LINEINFO(macros.vsh)(585)\n";
	push @output, "	mul $tmp.xyz, $tmp.xyz, $worldNormal	; $tmp = (n dot v ) n																								; LINEINFO(macros.vsh)(586)\n";
	push @output, "	mad $reflectionVector.xyz, $tmp, $cTwo, -$vertToEye																								; LINEINFO(macros.vsh)(587)\n";
	&FreeRegister( \$vertToEye, "\$vertToEye" );																								
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub ComputeSphereMapTexCoords																								
{																								
	local( $reflectionVector ) = shift;																								
	local( $sphereMapTexCoords ) = shift;																								
	local( $tmp ); &AllocateRegister( \$tmp, "\$tmp" );																								
	push @output, "	; transform reflection vector into view space																								; LINEINFO(macros.vsh)(600)\n";
	push @output, "	dp3 $tmp.x, $reflectionVector, $cViewModel0																								; LINEINFO(macros.vsh)(601)\n";
	push @output, "	dp3 $tmp.y, $reflectionVector, $cViewModel1																								; LINEINFO(macros.vsh)(602)\n";
	push @output, "	dp3 $tmp.z, $reflectionVector, $cViewModel2																								; LINEINFO(macros.vsh)(603)\n";
	push @output, "	; generate <rx ry rz+1>																								; LINEINFO(macros.vsh)(605)\n";
	push @output, "	add $tmp.z, $tmp.z, $cOne																								; LINEINFO(macros.vsh)(606)\n";
	push @output, "	; find 1 / the length of r2																								; LINEINFO(macros.vsh)(608)\n";
	push @output, "	dp3 $tmp.w, $tmp, $tmp																								; LINEINFO(macros.vsh)(609)\n";
	push @output, "	rsq $tmp.w, $tmp.w																								; LINEINFO(macros.vsh)(610)\n";
	push @output, "	; r1 = r2/|r2| + 1																								; LINEINFO(macros.vsh)(612)\n";
	push @output, "	mad $tmp.xy, $tmp.w, $tmp, $cOne																								; LINEINFO(macros.vsh)(613)\n";
	push @output, "	mul $sphereMapTexCoords.xy, $tmp.xy, $cHalf																								; LINEINFO(macros.vsh)(614)\n";
	&FreeRegister( \$tmp, "\$tmp" );																								
}																								
sub SkinPosition																								
{																								
#	print "\$SKINNING = $SKINNING\n";																								
	local( $worldPos ) = shift;																								
	if( !defined $SKINNING )																								
	{																								
		die "using \$SKINNING without defining.\n";																								
	}																								
	if( $SKINNING == 0 )																								
	{																								
		push @output, "		;																								; LINEINFO(macros.vsh)(631)\n";
		push @output, "		; 0 bone skinning (4 instructions)																								; LINEINFO(macros.vsh)(632)\n";
		push @output, "		;																								; LINEINFO(macros.vsh)(633)\n";
		push @output, "		; Transform position into world space																								; LINEINFO(macros.vsh)(634)\n";
		push @output, "		; position																								; LINEINFO(macros.vsh)(635)\n";
		push @output, "		dp4 $worldPos.x, $vPos, $cModel0																								; LINEINFO(macros.vsh)(636)\n";
		push @output, "		dp4 $worldPos.y, $vPos, $cModel1																								; LINEINFO(macros.vsh)(637)\n";
		push @output, "		dp4 $worldPos.z, $vPos, $cModel2																								; LINEINFO(macros.vsh)(638)\n";
		push @output, "		mov $worldPos.w, $cOne																								; LINEINFO(macros.vsh)(639)\n";
	} 																								
	else																								
	{																								
		push @output, "		;																								; LINEINFO(macros.vsh)(643)\n";
		push @output, "		; 3 bone skinning  (19 instructions)																								; LINEINFO(macros.vsh)(644)\n";
		push @output, "		;																								; LINEINFO(macros.vsh)(645)\n";
		local( $boneIndices );																								
		local( $blendedMatrix0 );																								
		local( $blendedMatrix1 );																								
		local( $blendedMatrix2 );																								
		local( $localPos );																								
		&AllocateRegister( \$boneIndices, "\$boneIndices" );																								
		&AllocateRegister( \$blendedMatrix0, "\$blendedMatrix0" );																								
		&AllocateRegister( \$blendedMatrix1, "\$blendedMatrix1" );																								
		&AllocateRegister( \$blendedMatrix2, "\$blendedMatrix2" );																								
		push @output, "		; Transform position into world space using all bones																								; LINEINFO(macros.vsh)(656)\n";
		push @output, "		; denormalize d3dcolor to matrix index																								; LINEINFO(macros.vsh)(657)\n";
		push @output, "		mad $boneIndices, $vBoneIndices, $cColorToIntScale, $cModel0Index																								; LINEINFO(macros.vsh)(658)\n";
		if ( $g_x360 )																								
		{																								
			push @output, "			mov $boneIndices, $boneIndices.zyxw																								; LINEINFO(macros.vsh)(661)\n";
		}																								
		push @output, "		; r11 = boneindices at this point																								; LINEINFO(macros.vsh)(664)\n";
		push @output, "		; first matrix																								; LINEINFO(macros.vsh)(665)\n";
		push @output, "		mov a0.x, $boneIndices.z																								; LINEINFO(macros.vsh)(666)\n";
		push @output, "		mul $blendedMatrix0, $vBoneWeights.x, c[a0.x]																								; LINEINFO(macros.vsh)(667)\n";
		push @output, "		mul $blendedMatrix1, $vBoneWeights.x, c[a0.x+1]																								; LINEINFO(macros.vsh)(668)\n";
		push @output, "		mul $blendedMatrix2, $vBoneWeights.x, c[a0.x+2]																								; LINEINFO(macros.vsh)(669)\n";
		push @output, "		; second matrix																								; LINEINFO(macros.vsh)(670)\n";
		push @output, "		mov a0.x, $boneIndices.y																								; LINEINFO(macros.vsh)(671)\n";
		push @output, "		mad $blendedMatrix0, $vBoneWeights.y, c[a0.x], $blendedMatrix0																								; LINEINFO(macros.vsh)(672)\n";
		push @output, "		mad $blendedMatrix1, $vBoneWeights.y, c[a0.x+1], $blendedMatrix1																								; LINEINFO(macros.vsh)(673)\n";
		push @output, "		mad $blendedMatrix2, $vBoneWeights.y, c[a0.x+2], $blendedMatrix2																								; LINEINFO(macros.vsh)(674)\n";
		push @output, "		; Calculate third weight																								; LINEINFO(macros.vsh)(676)\n";
		push @output, "		; compute 1-(weight1+weight2) to calculate weight2																								; LINEINFO(macros.vsh)(677)\n";
		push @output, "		; Use $boneIndices.w as a temp since we aren't using it for anything.																								; LINEINFO(macros.vsh)(678)\n";
		push @output, "		add $boneIndices.w, $vBoneWeights.x, $vBoneWeights.y																								; LINEINFO(macros.vsh)(679)\n";
		push @output, "		sub $boneIndices.w, $cOne, $boneIndices.w																								; LINEINFO(macros.vsh)(680)\n";
		push @output, "		; third matrix																								; LINEINFO(macros.vsh)(682)\n";
		push @output, "		mov a0.x, $boneIndices.x																								; LINEINFO(macros.vsh)(683)\n";
		push @output, "		mad $blendedMatrix0, $boneIndices.w, c[a0.x], $blendedMatrix0																								; LINEINFO(macros.vsh)(684)\n";
		push @output, "		mad $blendedMatrix1, $boneIndices.w, c[a0.x+1], $blendedMatrix1																								; LINEINFO(macros.vsh)(685)\n";
		push @output, "		mad $blendedMatrix2, $boneIndices.w, c[a0.x+2], $blendedMatrix2																								; LINEINFO(macros.vsh)(686)\n";
		push @output, "		dp4 $worldPos.x, $vPos, $blendedMatrix0																								; LINEINFO(macros.vsh)(688)\n";
		push @output, "		dp4 $worldPos.y, $vPos, $blendedMatrix1																								; LINEINFO(macros.vsh)(689)\n";
		push @output, "		dp4 $worldPos.z, $vPos, $blendedMatrix2																								; LINEINFO(macros.vsh)(690)\n";
		push @output, "		mov $worldPos.w, $cOne																								; LINEINFO(macros.vsh)(691)\n";
		&FreeRegister( \$boneIndices, "\$boneIndices" );																								
		&FreeRegister( \$blendedMatrix0, "\$blendedMatrix0" );																								
		&FreeRegister( \$blendedMatrix1, "\$blendedMatrix1" );																								
		&FreeRegister( \$blendedMatrix2, "\$blendedMatrix2" );																								
	}																								
}																								
sub SkinPositionAndNormal																								
{																								
#	print "\$SKINNING = $SKINNING\n";																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	if( !defined $SKINNING )																								
	{																								
		die "using \$SKINNING without defining.\n";																								
	}																								
	if( $SKINNING == 0 )																								
	{																								
		push @output, "		;																								; LINEINFO(macros.vsh)(714)\n";
		push @output, "		; 0 bone skinning (13 instructions)																								; LINEINFO(macros.vsh)(715)\n";
		push @output, "		;																								; LINEINFO(macros.vsh)(716)\n";
		push @output, "		; Transform position + normal + tangentS + tangentT into world space																								; LINEINFO(macros.vsh)(717)\n";
		push @output, "		; position																								; LINEINFO(macros.vsh)(718)\n";
		push @output, "		dp4 $worldPos.x, $vPos, $cModel0																								; LINEINFO(macros.vsh)(719)\n";
		push @output, "		dp4 $worldPos.y, $vPos, $cModel1																								; LINEINFO(macros.vsh)(720)\n";
		push @output, "		dp4 $worldPos.z, $vPos, $cModel2																								; LINEINFO(macros.vsh)(721)\n";
		push @output, "		mov $worldPos.w, $cOne																								; LINEINFO(macros.vsh)(722)\n";
		push @output, "		; normal																								; LINEINFO(macros.vsh)(723)\n";
		push @output, "		dp3 $worldNormal.x, $vNormal, $cModel0																								; LINEINFO(macros.vsh)(724)\n";
		push @output, "		dp3 $worldNormal.y, $vNormal, $cModel1																								; LINEINFO(macros.vsh)(725)\n";
		push @output, "		dp3 $worldNormal.z, $vNormal, $cModel2																								; LINEINFO(macros.vsh)(726)\n";
	}																								
	else																								
	{																								
		local( $boneIndices );																								
		local( $blendedMatrix0 );																								
		local( $blendedMatrix1 );																								
		local( $blendedMatrix2 );																								
		local( $localPos );																								
		local( $localNormal );																								
		local( $normalLength );																								
		local( $ooNormalLength );																								
		&AllocateRegister( \$boneIndices, "\$boneIndices" );																								
		&AllocateRegister( \$blendedMatrix0, "\$blendedMatrix0" );																								
		&AllocateRegister( \$blendedMatrix1, "\$blendedMatrix1" );																								
		&AllocateRegister( \$blendedMatrix2, "\$blendedMatrix2" );																								
		push @output, "		; Transform position into world space using all bones																								; LINEINFO(macros.vsh)(743)\n";
		push @output, "		; denormalize d3dcolor to matrix index																								; LINEINFO(macros.vsh)(744)\n";
		push @output, "		mad $boneIndices, $vBoneIndices, $cColorToIntScale, $cModel0Index																								; LINEINFO(macros.vsh)(745)\n";
		if ( $g_x360 )																								
		{																								
			push @output, "			mov $boneIndices, $boneIndices.zyxw																								; LINEINFO(macros.vsh)(748)\n";
		}																								
		push @output, "		; r11 = boneindices at this point																								; LINEINFO(macros.vsh)(751)\n";
		push @output, "		; first matrix																								; LINEINFO(macros.vsh)(752)\n";
		push @output, "		mov a0.x, $boneIndices.z																								; LINEINFO(macros.vsh)(753)\n";
		push @output, "		mul $blendedMatrix0, $vBoneWeights.x, c[a0.x]																								; LINEINFO(macros.vsh)(754)\n";
		push @output, "		mul $blendedMatrix1, $vBoneWeights.x, c[a0.x+1]																								; LINEINFO(macros.vsh)(755)\n";
		push @output, "		mul $blendedMatrix2, $vBoneWeights.x, c[a0.x+2]																								; LINEINFO(macros.vsh)(756)\n";
		push @output, "		; second matrix																								; LINEINFO(macros.vsh)(757)\n";
		push @output, "		mov a0.x, $boneIndices.y																								; LINEINFO(macros.vsh)(758)\n";
		push @output, "		mad $blendedMatrix0, $vBoneWeights.y, c[a0.x], $blendedMatrix0																								; LINEINFO(macros.vsh)(759)\n";
		push @output, "		mad $blendedMatrix1, $vBoneWeights.y, c[a0.x+1], $blendedMatrix1																								; LINEINFO(macros.vsh)(760)\n";
		push @output, "		mad $blendedMatrix2, $vBoneWeights.y, c[a0.x+2], $blendedMatrix2																								; LINEINFO(macros.vsh)(761)\n";
		push @output, "		; Calculate third weight																								; LINEINFO(macros.vsh)(763)\n";
		push @output, "		; compute 1-(weight1+weight2) to calculate weight2																								; LINEINFO(macros.vsh)(764)\n";
		push @output, "		; Use $boneIndices.w as a temp since we aren't using it for anything.																								; LINEINFO(macros.vsh)(765)\n";
		push @output, "		add $boneIndices.w, $vBoneWeights.x, $vBoneWeights.y																								; LINEINFO(macros.vsh)(766)\n";
		push @output, "		sub $boneIndices.w, $cOne, $boneIndices.w																								; LINEINFO(macros.vsh)(767)\n";
		push @output, "		; third matrix																								; LINEINFO(macros.vsh)(769)\n";
		push @output, "		mov a0.x, $boneIndices.x																								; LINEINFO(macros.vsh)(770)\n";
		push @output, "		mad $blendedMatrix0, $boneIndices.w, c[a0.x], $blendedMatrix0																								; LINEINFO(macros.vsh)(771)\n";
		push @output, "		mad $blendedMatrix1, $boneIndices.w, c[a0.x+1], $blendedMatrix1																								; LINEINFO(macros.vsh)(772)\n";
		push @output, "		mad $blendedMatrix2, $boneIndices.w, c[a0.x+2], $blendedMatrix2																								; LINEINFO(macros.vsh)(773)\n";
		push @output, "		dp4 $worldPos.x, $vPos, $blendedMatrix0																								; LINEINFO(macros.vsh)(775)\n";
		push @output, "		dp4 $worldPos.y, $vPos, $blendedMatrix1																								; LINEINFO(macros.vsh)(776)\n";
		push @output, "		dp4 $worldPos.z, $vPos, $blendedMatrix2																								; LINEINFO(macros.vsh)(777)\n";
		push @output, "		mov $worldPos.w, $cOne																								; LINEINFO(macros.vsh)(778)\n";
		push @output, "		; normal																								; LINEINFO(macros.vsh)(780)\n";
		push @output, "		dp3 $worldNormal.x, $vNormal, $blendedMatrix0																								; LINEINFO(macros.vsh)(781)\n";
		push @output, "		dp3 $worldNormal.y, $vNormal, $blendedMatrix1																								; LINEINFO(macros.vsh)(782)\n";
		push @output, "		dp3 $worldNormal.z, $vNormal, $blendedMatrix2																								; LINEINFO(macros.vsh)(783)\n";
		&FreeRegister( \$boneIndices, "\$boneIndices" );																								
		&FreeRegister( \$blendedMatrix0, "\$blendedMatrix0" );																								
		&FreeRegister( \$blendedMatrix1, "\$blendedMatrix1" );																								
		&FreeRegister( \$blendedMatrix2, "\$blendedMatrix2" );																								
	}																									
}																								
sub SkinPositionNormalAndTangentSpace																								
{																								
#	print "\$SKINNING = $SKINNING\n";																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	local( $worldTangentS ) = shift;																								
	local( $worldTangentT ) = shift;																								
	local( $userData );																								
	local( $localPos );																								
	local( $localNormal );																								
	local( $normalLength );																								
	local( $ooNormalLength );																								
	if( !defined $SKINNING )																								
	{																								
		die "using \$SKINNING without defining.\n";																								
	}																								
# X360TBD: needed for compressed vertex format																								
#	if ( $g_x360 )																								
#	{																								
#		&AllocateRegister( \$userData, "\$userData" );																								
#		; remap compressed range [0..1] to [-1..1]																								
#		mad $userData, $vUserData, $cTwo, -$cOne																								
#	}																								
	if( $SKINNING == 0 )																								
	{																								
		push @output, "		;																								; LINEINFO(macros.vsh)(820)\n";
		push @output, "		; 0 bone skinning (13 instructions)																								; LINEINFO(macros.vsh)(821)\n";
		push @output, "		;																								; LINEINFO(macros.vsh)(822)\n";
		push @output, "		; Transform position + normal + tangentS + tangentT into world space																								; LINEINFO(macros.vsh)(823)\n";
		push @output, "		dp4 $worldPos.x, $vPos, $cModel0																								; LINEINFO(macros.vsh)(824)\n";
		push @output, "		dp4 $worldPos.y, $vPos, $cModel1																								; LINEINFO(macros.vsh)(825)\n";
		push @output, "		dp4 $worldPos.z, $vPos, $cModel2																								; LINEINFO(macros.vsh)(826)\n";
		push @output, "		mov $worldPos.w, $cOne																								; LINEINFO(macros.vsh)(827)\n";
		push @output, "		; normal																								; LINEINFO(macros.vsh)(829)\n";
		push @output, "		dp3 $worldNormal.x, $vNormal, $cModel0																								; LINEINFO(macros.vsh)(830)\n";
		push @output, "		dp3 $worldNormal.y, $vNormal, $cModel1																								; LINEINFO(macros.vsh)(831)\n";
		push @output, "		dp3 $worldNormal.z, $vNormal, $cModel2																								; LINEINFO(macros.vsh)(832)\n";
# X360TBD: needed for compressed vertex format																								
#		if ( $g_x360 )																								
#		{																								
#			; tangents																								
#			dp3 $worldTangentS.x, $userData, $cModel0																								
#			dp3 $worldTangentS.y, $userData, $cModel1																								
#			dp3 $worldTangentS.z, $userData, $cModel2																								
#																								
#			; calculate tangent t via cross( N, S ) * S[3]																								
#			&Cross( $worldTangentT, $worldNormal, $worldTangentS );																								
#			mul $worldTangentT.xyz, $userData.w, $worldTangentT.xyz																								
#		}																								
#		else																								
		{																								
			push @output, "			; tangents																								; LINEINFO(macros.vsh)(848)\n";
			push @output, "			dp3 $worldTangentS.x, $vUserData, $cModel0																								; LINEINFO(macros.vsh)(849)\n";
			push @output, "			dp3 $worldTangentS.y, $vUserData, $cModel1																								; LINEINFO(macros.vsh)(850)\n";
			push @output, "			dp3 $worldTangentS.z, $vUserData, $cModel2																								; LINEINFO(macros.vsh)(851)\n";
			push @output, "			; calculate tangent t via cross( N, S ) * S[3]																								; LINEINFO(macros.vsh)(853)\n";
			&Cross( $worldTangentT, $worldNormal, $worldTangentS );																								
			push @output, "			mul $worldTangentT.xyz, $vUserData.w, $worldTangentT.xyz																								; LINEINFO(macros.vsh)(855)\n";
		}																								
	}																								
	else																								
	{																								
		local( $boneIndices );																								
		local( $blendedMatrix0 );																								
		local( $blendedMatrix1 );																								
		local( $blendedMatrix2 );																								
		&AllocateRegister( \$boneIndices, "\$boneIndices" );																								
		&AllocateRegister( \$blendedMatrix0, "\$blendedMatrix0" );																								
		&AllocateRegister( \$blendedMatrix1, "\$blendedMatrix1" );																								
		&AllocateRegister( \$blendedMatrix2, "\$blendedMatrix2" );																								
		push @output, "		; Transform position into world space using all bones																								; LINEINFO(macros.vsh)(869)\n";
		push @output, "		; denormalize d3dcolor to matrix index																								; LINEINFO(macros.vsh)(870)\n";
		push @output, "		mad $boneIndices, $vBoneIndices, $cColorToIntScale, $cModel0Index																								; LINEINFO(macros.vsh)(871)\n";
		if ( $g_x360 )																								
		{																								
			push @output, "			mov $boneIndices, $boneIndices.zyxw																								; LINEINFO(macros.vsh)(874)\n";
		}																								
		push @output, "		; r11 = boneindices at this point																								; LINEINFO(macros.vsh)(877)\n";
		push @output, "		; first matrix																								; LINEINFO(macros.vsh)(878)\n";
		push @output, "		mov a0.x, $boneIndices.z																								; LINEINFO(macros.vsh)(879)\n";
		push @output, "		mul $blendedMatrix0, $vBoneWeights.x, c[a0.x]																								; LINEINFO(macros.vsh)(880)\n";
		push @output, "		mul $blendedMatrix1, $vBoneWeights.x, c[a0.x+1]																								; LINEINFO(macros.vsh)(881)\n";
		push @output, "		mul $blendedMatrix2, $vBoneWeights.x, c[a0.x+2]																								; LINEINFO(macros.vsh)(882)\n";
		push @output, "		; second matrix																								; LINEINFO(macros.vsh)(883)\n";
		push @output, "		mov a0.x, $boneIndices.y																								; LINEINFO(macros.vsh)(884)\n";
		push @output, "		mad $blendedMatrix0, $vBoneWeights.y, c[a0.x], $blendedMatrix0																								; LINEINFO(macros.vsh)(885)\n";
		push @output, "		mad $blendedMatrix1, $vBoneWeights.y, c[a0.x+1], $blendedMatrix1																								; LINEINFO(macros.vsh)(886)\n";
		push @output, "		mad $blendedMatrix2, $vBoneWeights.y, c[a0.x+2], $blendedMatrix2																								; LINEINFO(macros.vsh)(887)\n";
		push @output, "		; Calculate third weight																								; LINEINFO(macros.vsh)(889)\n";
		push @output, "		; compute 1-(weight1+weight2) to calculate weight2																								; LINEINFO(macros.vsh)(890)\n";
		push @output, "		; Use $boneIndices.w as a temp since we aren't using it for anything.																								; LINEINFO(macros.vsh)(891)\n";
		push @output, "		add $boneIndices.w, $vBoneWeights.x, $vBoneWeights.y																								; LINEINFO(macros.vsh)(892)\n";
		push @output, "		sub $boneIndices.w, $cOne, $boneIndices.w																								; LINEINFO(macros.vsh)(893)\n";
		push @output, "		; third matrix																								; LINEINFO(macros.vsh)(895)\n";
		push @output, "		mov a0.x, $boneIndices.x																								; LINEINFO(macros.vsh)(896)\n";
		push @output, "		mad $blendedMatrix0, $boneIndices.w, c[a0.x], $blendedMatrix0																								; LINEINFO(macros.vsh)(897)\n";
		push @output, "		mad $blendedMatrix1, $boneIndices.w, c[a0.x+1], $blendedMatrix1																								; LINEINFO(macros.vsh)(898)\n";
		push @output, "		mad $blendedMatrix2, $boneIndices.w, c[a0.x+2], $blendedMatrix2																								; LINEINFO(macros.vsh)(899)\n";
		push @output, "		; position																								; LINEINFO(macros.vsh)(901)\n";
		push @output, "		dp4 $worldPos.x, $vPos, $blendedMatrix0																								; LINEINFO(macros.vsh)(902)\n";
		push @output, "		dp4 $worldPos.y, $vPos, $blendedMatrix1																								; LINEINFO(macros.vsh)(903)\n";
		push @output, "		dp4 $worldPos.z, $vPos, $blendedMatrix2																								; LINEINFO(macros.vsh)(904)\n";
		push @output, "		mov $worldPos.w, $cOne																								; LINEINFO(macros.vsh)(905)\n";
		push @output, "		; normal																								; LINEINFO(macros.vsh)(907)\n";
		push @output, "		dp3 $worldNormal.x, $vNormal, $blendedMatrix0																								; LINEINFO(macros.vsh)(908)\n";
		push @output, "		dp3 $worldNormal.y, $vNormal, $blendedMatrix1																								; LINEINFO(macros.vsh)(909)\n";
		push @output, "		dp3 $worldNormal.z, $vNormal, $blendedMatrix2																								; LINEINFO(macros.vsh)(910)\n";
# X360TBD: needed for compressed vertex format																								
#		if ( $g_x360 )																								
#		{																								
#			; tangents																								
#			dp3 $worldTangentS.x, $userData, $blendedMatrix0																								
#			dp3 $worldTangentS.y, $userData, $blendedMatrix1																								
#			dp3 $worldTangentS.z, $userData, $blendedMatrix2																								
#																								
#			; calculate tangent t via cross( N, S ) * S[3]																								
#			&Cross( $worldTangentT, $worldNormal, $worldTangentS );																								
#			mul $worldTangentT.xyz, $userData.w, $worldTangentT.xyz																								
#		}																								
#		else																								
		{																								
			push @output, "			; tangents																								; LINEINFO(macros.vsh)(926)\n";
			push @output, "			dp3 $worldTangentS.x, $vUserData, $blendedMatrix0																								; LINEINFO(macros.vsh)(927)\n";
			push @output, "			dp3 $worldTangentS.y, $vUserData, $blendedMatrix1																								; LINEINFO(macros.vsh)(928)\n";
			push @output, "			dp3 $worldTangentS.z, $vUserData, $blendedMatrix2																								; LINEINFO(macros.vsh)(929)\n";
			push @output, "			; calculate tangent t via cross( N, S ) * S[3]																								; LINEINFO(macros.vsh)(931)\n";
			&Cross( $worldTangentT, $worldNormal, $worldTangentS );																								
			push @output, "			mul $worldTangentT.xyz, $vUserData.w, $worldTangentT.xyz																								; LINEINFO(macros.vsh)(933)\n";
		}																								
		&FreeRegister( \$boneIndices, "\$boneIndices" );																								
		&FreeRegister( \$blendedMatrix0, "\$blendedMatrix0" );																								
		&FreeRegister( \$blendedMatrix1, "\$blendedMatrix1" );																								
		&FreeRegister( \$blendedMatrix2, "\$blendedMatrix2" );																								
	}																								
# X360TBD: needed for compressed vertex format																								
#	if ( $g_x360 )																								
#	{																								
#		&FreeRegister( \$userData, "\$userData" );																								
#	}																								
}																								
sub ColorClamp																								
{																								
	push @output, "	; ColorClamp; stomps $color.w																								; LINEINFO(macros.vsh)(951)\n";
	local( $color ) = shift;																								
	local( $dst ) = shift;																								
	push @output, "	; Get the max of RGB and stick it in W																								; LINEINFO(macros.vsh)(955)\n";
	push @output, "	max $color.w, $color.x, $color.y																								; LINEINFO(macros.vsh)(956)\n";
	push @output, "	max $color.w, $color.w, $color.z																								; LINEINFO(macros.vsh)(957)\n";
	push @output, "	; get the greater of one and the max color.																								; LINEINFO(macros.vsh)(959)\n";
	push @output, "	max $color.w, $color.w, $cOne																								; LINEINFO(macros.vsh)(960)\n";
	push @output, "	rcp $color.w, $color.w																								; LINEINFO(macros.vsh)(962)\n";
	push @output, "	mul $dst.xyz, $color.w, $color.xyz																								; LINEINFO(macros.vsh)(963)\n";
}																								
sub AmbientLight																								
{																								
	local( $worldNormal ) = shift;																								
	local( $linearColor ) = shift;																								
	local( $add ) = shift;																								
	push @output, "	; Ambient lighting																								; LINEINFO(macros.vsh)(972)\n";
	&AllocateRegister( \$nSquared, "\$nSquared" );																								
	&AllocateRegister( \$isNegative, "\$isNegative" );																								
	push @output, "	mul $nSquared.xyz, $worldNormal.xyz, $worldNormal.xyz				; compute n times n																								; LINEINFO(macros.vsh)(976)\n";
	push @output, "	slt $isNegative.xyz, $worldNormal.xyz, $cZero				; Figure out whether each component is >0																								; LINEINFO(macros.vsh)(977)\n";
	push @output, "	mov a0.x, $isNegative.x																								; LINEINFO(macros.vsh)(978)\n";
	if( $add )																								
	{																								
		push @output, "		mad $linearColor.xyz, $nSquared.x, c[a0.x + $cAmbientColorPosXOffset], $linearColor			; $linearColor = normal[0]*normal[0] * box color of appropriate x side																								; LINEINFO(macros.vsh)(981)\n";
	}																								
	else																								
	{																								
		push @output, "		mul $linearColor.xyz, $nSquared.x, c[a0.x + $cAmbientColorPosXOffset]			; $linearColor = normal[0]*normal[0] * box color of appropriate x side																								; LINEINFO(macros.vsh)(985)\n";
	}																								
	push @output, "	mov a0.x, $isNegative.y																								; LINEINFO(macros.vsh)(987)\n";
	push @output, "	mad $linearColor.xyz, $nSquared.y, c[a0.x + $cAmbientColorPosYOffset], $linearColor																								; LINEINFO(macros.vsh)(988)\n";
	push @output, "	mov a0.x, $isNegative.z																								; LINEINFO(macros.vsh)(989)\n";
	push @output, "	mad $linearColor.xyz, $nSquared.z, c[a0.x + $cAmbientColorPosZOffset], $linearColor																								; LINEINFO(macros.vsh)(990)\n";
	&FreeRegister( \$isNegative, "\$isNegative" );																								
	&FreeRegister( \$nSquared, "\$nSquared" );																								
}																								
sub DirectionalLight																								
{																								
	local( $worldNormal ) = shift;																								
	local( $linearColor ) = shift;																								
	local( $add ) = shift;																								
	&AllocateRegister( \$nDotL, "\$nDotL" ); # FIXME: This only needs to be a scalar																								
	push @output, "	; NOTE: Gotta use -l here, since light direction = -l																								; LINEINFO(macros.vsh)(1004)\n";
	push @output, "	; DIRECTIONAL LIGHT																								; LINEINFO(macros.vsh)(1005)\n";
	push @output, "	; compute n dot l																								; LINEINFO(macros.vsh)(1006)\n";
	push @output, "	dp3 $nDotL.x, -c[a0.x + 1], $worldNormal																								; LINEINFO(macros.vsh)(1007)\n";
	if ( $HALF_LAMBERT == 0 )																								
	{																								
		push @output, "		; lambert																								; LINEINFO(macros.vsh)(1011)\n";
		push @output, "		max $nDotL.x, $nDotL.x, c0.x				; Clamp to zero																								; LINEINFO(macros.vsh)(1012)\n";
	}																								
	elsif ( $HALF_LAMBERT == 1 )																								
	{																								
		push @output, "		; half-lambert																								; LINEINFO(macros.vsh)(1016)\n";
		push @output, "		mad $nDotL.x, $nDotL.x, $cHalf, $cHalf		; dot = (dot * 0.5 + 0.5)^2																								; LINEINFO(macros.vsh)(1017)\n";
		push @output, "		mul $nDotL.x, $nDotL.x, $nDotL.x																								; LINEINFO(macros.vsh)(1018)\n";
	}																								
	else																								
	{																								
		die "\$HALF_LAMBERT is hosed\n";																								
	}																								
	if( $add )																								
	{																								
		push @output, "		mad $linearColor.xyz, c[a0.x], $nDotL.x, $linearColor																								; LINEINFO(macros.vsh)(1027)\n";
	}																								
	else																								
	{																								
		push @output, "		mul $linearColor.xyz, c[a0.x], $nDotL.x																								; LINEINFO(macros.vsh)(1031)\n";
	}																								
	&FreeRegister( \$nDotL, "\$nDotL" );																								
}																								
sub PointLight																								
{																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	local( $linearColor ) = shift;																								
	local( $add ) = shift;																								
	local( $lightDir );																								
	&AllocateRegister( \$lightDir, "\$lightDir" );																								
	push @output, "	; POINT LIGHT																								; LINEINFO(macros.vsh)(1047)\n";
	push @output, "	; compute light direction																								; LINEINFO(macros.vsh)(1048)\n";
	push @output, "	sub $lightDir, c[a0.x+2], $worldPos																								; LINEINFO(macros.vsh)(1049)\n";
	local( $lightDistSquared );																								
	local( $ooLightDist );																								
	&AllocateRegister( \$lightDistSquared, "\$lightDistSquared" );																								
	&AllocateRegister( \$ooLightDist, "\$ooLightDist" );																								
	push @output, "	; normalize light direction, maintain temporaries for attenuation																								; LINEINFO(macros.vsh)(1056)\n";
	push @output, "	dp3 $lightDistSquared, $lightDir, $lightDir																								; LINEINFO(macros.vsh)(1057)\n";
	push @output, "	rsq $ooLightDist, $lightDistSquared.x																								; LINEINFO(macros.vsh)(1058)\n";
	push @output, "	mul $lightDir, $lightDir, $ooLightDist.x																								; LINEINFO(macros.vsh)(1059)\n";
	local( $attenuationFactors );																								
	&AllocateRegister( \$attenuationFactors, "\$attenuationFactors" );																								
	push @output, "	; compute attenuation amount (r2 = 'd*d d*d d*d d*d', r3 = '1/d 1/d 1/d 1/d')																								; LINEINFO(macros.vsh)(1064)\n";
	push @output, "	dst $attenuationFactors, $lightDistSquared, $ooLightDist						; r4 = ( 1, d, d*d, 1/d )																								; LINEINFO(macros.vsh)(1065)\n";
	&FreeRegister( \$lightDistSquared, "\$lightDistSquared" );																								
	&FreeRegister( \$ooLightDist, "\$ooLightDist" );																								
	local( $attenuation );																								
	&AllocateRegister( \$attenuation, "\$attenuation" );																								
	push @output, "	dp3 $attenuation, $attenuationFactors, c[a0.x+4]				; r3 = atten0 + d * atten1 + d*d * atten2																								; LINEINFO(macros.vsh)(1070)\n";
	push @output, "	rcp $lightDir.w, $attenuation						; $lightDir.w = 1 / (atten0 + d * atten1 + d*d * atten2)																								; LINEINFO(macros.vsh)(1072)\n";
	&FreeRegister( \$attenuationFactors, "\$attenuationFactors" );																								
	&FreeRegister( \$attenuation, "\$attenuation" );																								
	local( $tmp );																								
	&AllocateRegister( \$tmp, "\$tmp" ); # FIXME : really only needs to be a scalar																								
	push @output, "	; compute n dot l, fold in distance attenutation																								; LINEINFO(macros.vsh)(1080)\n";
	push @output, "	dp3 $tmp.x, $lightDir, $worldNormal																								; LINEINFO(macros.vsh)(1081)\n";
	if ( $HALF_LAMBERT == 0 )																								
	{																								
		push @output, "		; lambert																								; LINEINFO(macros.vsh)(1085)\n";
		push @output, "		max $tmp.x, $tmp.x, c0.x				; Clamp to zero																								; LINEINFO(macros.vsh)(1086)\n";
	}																								
	elsif ( $HALF_LAMBERT == 1 )																								
	{																								
		push @output, "		; half-lambert																								; LINEINFO(macros.vsh)(1090)\n";
		push @output, "		mad $tmp.x, $tmp.x, $cHalf, $cHalf		; dot = (dot * 0.5 + 0.5)^2																								; LINEINFO(macros.vsh)(1091)\n";
		push @output, "		mul $tmp.x, $tmp.x, $tmp.x																								; LINEINFO(macros.vsh)(1092)\n";
	}																								
	else																								
	{																								
		die "\$HALF_LAMBERT is hosed\n";																								
	}																								
	push @output, "	mul $tmp.x, $tmp.x, $lightDir.w																								; LINEINFO(macros.vsh)(1099)\n";
	if( $add )																								
	{																								
		push @output, "		mad $linearColor.xyz, c[a0.x], $tmp.x, $linearColor																								; LINEINFO(macros.vsh)(1102)\n";
	}																								
	else																								
	{																								
		push @output, "		mul $linearColor.xyz, c[a0.x], $tmp.x																								; LINEINFO(macros.vsh)(1106)\n";
	}																								
	&FreeRegister( \$lightDir, "\$lightDir" );																								
	&FreeRegister( \$tmp, "\$tmp" ); # FIXME : really only needs to be a scalar																								
}																								
sub SpotLight																								
{																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	local( $linearColor ) = shift;																								
	local( $add ) = shift;																								
	local( $lightDir );																								
	&AllocateRegister( \$lightDir, "\$lightDir" );																								
	push @output, "	; SPOTLIGHT																								; LINEINFO(macros.vsh)(1123)\n";
	push @output, "	; compute light direction																								; LINEINFO(macros.vsh)(1124)\n";
	push @output, "	sub $lightDir, c[a0.x+2], $worldPos																								; LINEINFO(macros.vsh)(1125)\n";
	local( $lightDistSquared );																								
	local( $ooLightDist );																								
	&AllocateRegister( \$lightDistSquared, "\$lightDistSquared" );																								
	&AllocateRegister( \$ooLightDist, "\$ooLightDist" );																								
	push @output, "	; normalize light direction, maintain temporaries for attenuation																								; LINEINFO(macros.vsh)(1132)\n";
	push @output, "	dp3 $lightDistSquared, $lightDir, $lightDir																								; LINEINFO(macros.vsh)(1133)\n";
	push @output, "	rsq $ooLightDist, $lightDistSquared.x																								; LINEINFO(macros.vsh)(1134)\n";
	push @output, "	mul $lightDir, $lightDir, $ooLightDist.x																								; LINEINFO(macros.vsh)(1135)\n";
	local( $attenuationFactors );																								
	&AllocateRegister( \$attenuationFactors, "\$attenuationFactors" );																								
	push @output, "	; compute attenuation amount (r2 = 'd*d d*d d*d d*d', r3 = '1/d 1/d 1/d 1/d')																								; LINEINFO(macros.vsh)(1140)\n";
	push @output, "	dst $attenuationFactors, $lightDistSquared, $ooLightDist						; r4 = ( 1, d, d*d, 1/d )																								; LINEINFO(macros.vsh)(1141)\n";
	&FreeRegister( \$lightDistSquared, "\$lightDistSquared" );																								
	&FreeRegister( \$ooLightDist, "\$ooLightDist" );																								
	local( $attenuation );	&AllocateRegister( \$attenuation, "\$attenuation" );																								
	push @output, "	dp3 $attenuation, $attenuationFactors, c[a0.x+4]				; r3 = atten0 + d * atten1 + d*d * atten2																								; LINEINFO(macros.vsh)(1147)\n";
	push @output, "	rcp $lightDir.w, $attenuation						; r1.w = 1 / (atten0 + d * atten1 + d*d * atten2)																								; LINEINFO(macros.vsh)(1148)\n";
	&FreeRegister( \$attenuationFactors, "\$attenuationFactors" );																								
	&FreeRegister( \$attenuation, "\$attenuation" );																								
	local( $litSrc ); &AllocateRegister( \$litSrc, "\$litSrc" );																								
	local( $tmp ); &AllocateRegister( \$tmp, "\$tmp" ); # FIXME - only needs to be scalar																								
	push @output, "	; compute n dot l																								; LINEINFO(macros.vsh)(1156)\n";
	push @output, "	dp3 $litSrc.x, $worldNormal, $lightDir																								; LINEINFO(macros.vsh)(1157)\n";
	if ( $HALF_LAMBERT == 0 )																								
	{																								
		push @output, "		; lambert																								; LINEINFO(macros.vsh)(1161)\n";
		push @output, "		max $litSrc.x, $litSrc.x, c0.x				; Clamp to zero																								; LINEINFO(macros.vsh)(1162)\n";
	}																								
	elsif ( $HALF_LAMBERT == 1 )																								
	{																								
		push @output, "		; half-lambert																								; LINEINFO(macros.vsh)(1166)\n";
		push @output, "		mad $litSrc.x, $litSrc.x, $cHalf, $cHalf	; dot = (dot * 0.5 + 0.5) ^ 2																								; LINEINFO(macros.vsh)(1167)\n";
		push @output, "		mul $litSrc.x, $litSrc.x, $litSrc.x																								; LINEINFO(macros.vsh)(1168)\n";
	}																								
	else																								
	{																								
		die "\$HALF_LAMBERT is hosed\n";																								
	}																								
	push @output, "	; compute angular attenuation																								; LINEINFO(macros.vsh)(1175)\n";
	push @output, "	dp3 $tmp.x, c[a0.x+1], -$lightDir				; dot = -delta * spot direction																								; LINEINFO(macros.vsh)(1176)\n";
	push @output, "	sub $litSrc.y, $tmp.x, c[a0.x+3].z				; r2.y = dot - stopdot2																								; LINEINFO(macros.vsh)(1177)\n";
	&FreeRegister( \$tmp, "\$tmp" );																								
	push @output, "	mul $litSrc.y, $litSrc.y, c[a0.x+3].w			; r2.y = (dot - stopdot2) / (stopdot - stopdot2)																								; LINEINFO(macros.vsh)(1179)\n";
	push @output, "	mov $litSrc.w, c[a0.x+3].x						; r2.w = exponent																								; LINEINFO(macros.vsh)(1180)\n";
	local( $litDst ); &AllocateRegister( \$litDst, "\$litDst" );																								
	push @output, "	lit $litDst, $litSrc							; r3.y = N dot L or 0, whichever is bigger																								; LINEINFO(macros.vsh)(1182)\n";
	&FreeRegister( \$litSrc, "\$litSrc" );																								
													push @output, "													; r3.z = pow((dot - stopdot2) / (stopdot - stopdot2), exponent)																								; LINEINFO(macros.vsh)(1184)\n";
	push @output, "	min $litDst.z, $litDst.z, $cOne		 			; clamp pow() to 1																								; LINEINFO(macros.vsh)(1185)\n";
	local( $tmp1 ); &AllocateRegister( \$tmp1, "\$tmp1" );																								
	local( $tmp2 ); &AllocateRegister( \$tmp2, "\$tmp2" );  # FIXME - could be scalar																								
	push @output, "	; fold in distance attenutation with other factors																								; LINEINFO(macros.vsh)(1190)\n";
	push @output, "	mul $tmp1, c[a0.x], $lightDir.w																								; LINEINFO(macros.vsh)(1191)\n";
	push @output, "	mul $tmp2.x, $litDst.y, $litDst.z																								; LINEINFO(macros.vsh)(1192)\n";
	if( $add )																								
	{																								
		push @output, "		mad $linearColor.xyz, $tmp1, $tmp2.x, $linearColor																								; LINEINFO(macros.vsh)(1195)\n";
	}																								
	else																								
	{																								
		push @output, "		mul $linearColor.xyz, $tmp1, $tmp2.x																								; LINEINFO(macros.vsh)(1199)\n";
	}																								
	&FreeRegister( \$lightDir, "\$lightDir" );																								
	&FreeRegister( \$litDst, "\$litDst" );																								
	&FreeRegister( \$tmp1, "\$tmp1" );																								
	&FreeRegister( \$tmp2, "\$tmp2" );																								
}																								
sub DoLight																								
{																								
	local( $lightType ) = shift;																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	local( $linearColor ) = shift;																								
	local( $add ) = shift;																								
	if( $lightType eq "spot" )																								
	{																								
		&SpotLight( $worldPos, $worldNormal, $linearColor, $add );																								
	}																								
	elsif( $lightType eq "point" )																								
	{																								
		&PointLight( $worldPos, $worldNormal, $linearColor, $add );																								
	}																								
	elsif( $lightType eq "directional" )																								
	{																								
		&DirectionalLight( $worldNormal, $linearColor, $add );																								
	}																								
	else																								
	{																								
		die "don't know about light type \"$lightType\"\n";																								
	}																								
}																								
sub DoLighting																								
{																								
	if( !defined $LIGHT_COMBO )																								
	{																								
		die "DoLighting called without using \$LIGHT_COMBO\n";																								
	}																								
	if ( !defined $HALF_LAMBERT )																								
	{																								
		die "DoLighting called without using \$HALF_LAMBERT\n";																								
	}																								
	my $staticLightType = $g_staticLightTypeArray[$LIGHT_COMBO];																								
	my $ambientLightType = $g_ambientLightTypeArray[$LIGHT_COMBO];																								
	my $localLightType1 = $g_localLightType1Array[$LIGHT_COMBO];																								
	my $localLightType2 = $g_localLightType2Array[$LIGHT_COMBO];																								
#	print "\$staticLightType = $staticLightType\n";																								
#	print "\$ambientLightType = $ambientLightType\n";																								
#	print "\$localLightType1 = $localLightType1\n";																								
#	print "\$localLightType2 = $localLightType2\n";																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	push @output, "	; special case for no lighting																								; LINEINFO(macros.vsh)(1258)\n";
	if( $staticLightType eq "none" && $ambientLightType eq "none" &&																								
		$localLightType1 eq "none" && $localLightType2 eq "none" )																								
	{																								
		push @output, "		; Have to write something here since debug d3d runtime will barf otherwise.																								; LINEINFO(macros.vsh)(1262)\n";
		push @output, "		mov oD0, $cOne																								; LINEINFO(macros.vsh)(1263)\n";
		return;																								
	}																								
	push @output, "	; special case for static lighting only																								; LINEINFO(macros.vsh)(1267)\n";
	push @output, "	; Don't need to bother converting to linear space in this case.																								; LINEINFO(macros.vsh)(1268)\n";
	if( $staticLightType eq "static" && $ambientLightType eq "none" &&																								
		$localLightType1 eq "none" && $localLightType2 eq "none" )																								
	{																								
		push @output, "		mov oD0, $vSpecular																								; LINEINFO(macros.vsh)(1272)\n";
		return;																								
	}																								
	local( $linearColor ); &AllocateRegister( \$linearColor, "\$linearColor" );
	local( $gammaColor ); &AllocateRegister( \$gammaColor, "\$gammaColor" );
	local( $add ) = 0;																								
	if( $staticLightType eq "static" )																								
	{																								
		push @output, "		; The static lighting comes in in gamma space and has also been premultiplied by $cOverbrightFactor																								; LINEINFO(macros.vsh)(1282)\n";
		push @output, "		; need to get it into																								; LINEINFO(macros.vsh)(1283)\n";
		push @output, "		; linear space so that we can do adds.																								; LINEINFO(macros.vsh)(1284)\n";
		push @output, "		rcp $gammaColor.w, $cOverbrightFactor																								; LINEINFO(macros.vsh)(1285)\n";
		push @output, "		mul $gammaColor.xyz, $vSpecular, $gammaColor.w																								; LINEINFO(macros.vsh)(1286)\n";
		&GammaToLinear( $gammaColor, $linearColor );																								
		$add = 1;																								
	}																								
	if( $ambientLightType eq "ambient" )																								
	{																								
		&AmbientLight( $worldNormal, $linearColor, $add );																								
		$add = 1;																								
	}																								
	if( $localLightType1 ne "none" )																								
	{																								
		push @output, "		mov a0.x, $cLight0Offset																								; LINEINFO(macros.vsh)(1299)\n";
		&DoLight( $localLightType1, $worldPos, $worldNormal, $linearColor, $add );																								
		$add = 1;																								
	}																								
	if( $localLightType2 ne "none" )																								
	{																								
		push @output, "		mov a0.x, $cLight1Offset																								; LINEINFO(macros.vsh)(1306)\n";
		&DoLight( $localLightType2, $worldPos, $worldNormal, $linearColor, $add );																								
		$add = 1;																								
	}																								
	push @output, "	;------------------------------------------------------------------------------																								; LINEINFO(macros.vsh)(1311)\n";
	push @output, "	; Output color (gamma correction)																								; LINEINFO(macros.vsh)(1312)\n";
	push @output, "	;------------------------------------------------------------------------------																								; LINEINFO(macros.vsh)(1313)\n";
	&LinearToGamma( $linearColor, $gammaColor );																								
	if( 0 )																								
	{																								
		push @output, "		mul oD0.xyz, $gammaColor.xyz, $cOverbrightFactor																								; LINEINFO(macros.vsh)(1318)\n";
	}																								
	else																								
	{																								
		push @output, "		mul $gammaColor.xyz, $gammaColor.xyz, $cOverbrightFactor																								; LINEINFO(macros.vsh)(1322)\n";
		&ColorClamp( $gammaColor, "oD0" );																								
	}																								
 	push @output, ";	mov oD0.xyz, $linearColor																								; LINEINFO(macros.vsh)(1326)\n";
	push @output, "	mov oD0.w, $cOne				; make sure all components are defined																								; LINEINFO(macros.vsh)(1327)\n";
	&FreeRegister( \$linearColor, "\$linearColor" );
	&FreeRegister( \$gammaColor, "\$gammaColor" );
}																								
sub DoDynamicLightingToLinear																								
{																								
	local( $worldPos ) = shift;																								
	local( $worldNormal ) = shift;																								
	local( $linearColor ) = shift;																								
	if( !defined $LIGHT_COMBO )																								
	{																								
		die "DoLighting called without using \$LIGHT_COMBO\n";																								
	}																								
	if ( !defined $HALF_LAMBERT )																								
	{																								
		die "DoLighting called without using \$HALF_LAMBERT\n";																								
	}																								
	my $staticLightType = $g_staticLightTypeArray[$LIGHT_COMBO];																								
	my $ambientLightType = $g_ambientLightTypeArray[$LIGHT_COMBO];																								
	my $localLightType1 = $g_localLightType1Array[$LIGHT_COMBO];																								
	my $localLightType2 = $g_localLightType2Array[$LIGHT_COMBO];																								
	# No lights at all. . note that we don't even consider static lighting here.																								
	if( $ambientLightType eq "none" &&																								
		$localLightType1 eq "none" && $localLightType2 eq "none" )																								
	{																								
		push @output, "		mov $linearColor, $cZero																								; LINEINFO(macros.vsh)(1357)\n";
		return;																								
	}																								
	local( $add ) = 0;																								
	if( $ambientLightType eq "ambient" )																								
	{																								
		&AmbientLight( $worldNormal, $linearColor, $add );																								
		$add = 1;																								
	}																								
	if( $localLightType1 ne "none" )																								
	{																								
		push @output, "		mov a0.x, $cLight0Offset																								; LINEINFO(macros.vsh)(1370)\n";
		&DoLight( $localLightType1, $worldPos, $worldNormal, $linearColor, $add );																								
		$add = 1;																								
	}																								
	if( $localLightType2 ne "none" )																								
	{																								
		push @output, "		mov a0.x, $cLight1Offset																								; LINEINFO(macros.vsh)(1377)\n";
		&DoLight( $localLightType2, $worldPos, $worldNormal, $linearColor, $add );																								
		$add = 1;																								
	}																								
}																								
sub NotImplementedYet																								
{																								
	&AllocateRegister( \$projPos, "\$projPos" );																								
	push @output, "	dp4 $projPos.x, $worldPos, $cViewProj0																								; LINEINFO(macros.vsh)(1386)\n";
	push @output, "	dp4 $projPos.y, $worldPos, $cViewProj1																								; LINEINFO(macros.vsh)(1387)\n";
	push @output, "	dp4 $projPos.z, $worldPos, $cViewProj2																								; LINEINFO(macros.vsh)(1388)\n";
	push @output, "	dp4 $projPos.w, $worldPos, $cViewProj3																								; LINEINFO(macros.vsh)(1389)\n";
	push @output, "	mov oPos, $projPos																								; LINEINFO(macros.vsh)(1390)\n";
	&FreeRegister( \$projPos, "\$projPos" );																								
	exit;																								
}																								
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(7)\n";
push @output, "; Vertex blending																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(8)\n";
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(9)\n";
&AllocateRegister( \$projPos, "\$projPos" );																								
push @output, "; Transform position from object to projection space																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(13)\n";
push @output, "dp4 $projPos.x, $vPos, $cModelViewProj0																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(14)\n";
push @output, "dp4 $projPos.y, $vPos, $cModelViewProj1																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(15)\n";
push @output, "dp4 $projPos.z, $vPos, $cModelViewProj2																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(16)\n";
push @output, "dp4 $projPos.w, $vPos, $cModelViewProj3																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(17)\n";
push @output, "mov oPos, $projPos																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(19)\n";
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(21)\n";
push @output, "; Fog																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(22)\n";
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(23)\n";
local( $worldPos ); &AllocateRegister( \$worldPos, "\$worldPos" );
if( $DOWATERFOG == 1 )																								
{																								
	push @output, "	; Get the worldpos z component only since that's all we need for height fog																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(28)\n";
	push @output, "	dp4 $worldPos.z, $vPos, $cModel2																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(29)\n";
}																								
&CalcFog( $worldPos, $projPos );																								
&FreeRegister( \$worldPos, "\$worldPos" );
&FreeRegister( \$projPos, "\$projPos" );																								
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(36)\n";
push @output, "; Texture coordinates																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(37)\n";
push @output, ";------------------------------------------------------------------------------																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(38)\n";
push @output, "; Compute the texture coordinates given the offset between																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(40)\n";
push @output, "; each bumped lightmap																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(41)\n";
&AllocateRegister( \$offset, "\$offset" );																								
push @output, "mov $offset.xy, $vTexCoord2																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(43)\n";
push @output, "dp4 oT0.x, $vTexCoord0, $SHADER_SPECIFIC_CONST_0																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(44)\n";
push @output, "dp4 oT0.y, $vTexCoord0, $SHADER_SPECIFIC_CONST_1																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(45)\n";
push @output, "add oT1.xy, $offset, $vTexCoord1																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(46)\n";
push @output, "mad oT2.xy, $offset, $cTwo, $vTexCoord1																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(47)\n";
local( $three ); &AllocateRegister( \$three, "\$three" );
push @output, "add $three, $cOne, $cTwo																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(49)\n";
push @output, "mad oT3.xy, $offset, $three, $vTexCoord1																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(50)\n";
&FreeRegister( \$three, "\$three" );
push @output, "dp4 oT4.x, $vTexCoord0, $SHADER_SPECIFIC_CONST_2																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(52)\n";
push @output, "dp4 oT4.y, $vTexCoord0, $SHADER_SPECIFIC_CONST_3																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(53)\n";
push @output, "dp4 oT5.x, $vTexCoord0, $SHADER_SPECIFIC_CONST_4																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(54)\n";
push @output, "dp4 oT5.y, $vTexCoord0, $SHADER_SPECIFIC_CONST_5																								; LINEINFO(LightmappedGeneric_BumpmappedLightmap_Blend_ps14.vsh)(55)\n";
&FreeRegister( \$offset, "\$offset" );																								
