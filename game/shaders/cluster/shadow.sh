/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "samplers.sh"

#define SM_PCF 1
#define SM_LINEAR 1
#define SM_CSM 1

#include "fs_shadowmaps_color_lighting.sh"

SAMPLER2D(s_shadowMap0, SAMPLER_SHADOW_MAP_NEAR);
SAMPLER2D(s_shadowMap1, SAMPLER_SHADOW_MAP_MED);
SAMPLER2D(s_shadowMap2, SAMPLER_SHADOW_MAP_FAR);
SAMPLER2D(s_shadowMap3, SAMPLER_SHADOW_MAP_VFAR);

float shadowPass(vec4 v_shadowcoord0,vec4 v_shadowcoord1,vec4 v_shadowcoord2,vec4 v_shadowcoord3)
{
	#include "fs_shadowmaps_color_lighting_main.sh"
}