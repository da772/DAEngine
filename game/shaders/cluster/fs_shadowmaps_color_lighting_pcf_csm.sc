$input v_position, v_normal, v_viewDir, v_shadowcoord0, v_shadowcoord1, v_shadowcoord2, v_shadowcoord3

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#define SM_PCF 1
#define SM_CSM 1

#include "fs_shadowmaps_color_lighting.sh"

void main()
{
#include "fs_shadowmaps_color_lighting_main.sh"
}