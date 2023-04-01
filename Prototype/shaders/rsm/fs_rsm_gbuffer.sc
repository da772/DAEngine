$input v_normal,v_texcoord0

/*
 * Copyright 2016 Joseph Cherlin. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"

uniform vec4 u_tint;
SAMPLER2D(s_texColor,  0);
SAMPLER2D(s_texNormal,  1);

void main()
{
	vec3 normalWorldSpace = v_normal;

	// Write normal
	gl_FragData[0].xyz = normalWorldSpace.xyz; // Normal is already compressed to [0,1] so can fit in gbuffer
	gl_FragData[0].w = 0.0;

	// Write color
	gl_FragData[1] = texture2D(s_texColor, v_texcoord0);
}
