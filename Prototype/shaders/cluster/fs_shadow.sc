/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
$input v_position

#include "../common.sh"
uniform vec4 u_depthScaleOffset;  // for GL, map depth values into [0, 1] range
#define u_depthScale u_depthScaleOffset.x
#define u_depthOffset u_depthScaleOffset.y


void main()
{
	float depth = v_position.y/v_position.w * u_depthScale + u_depthOffset;
	gl_FragColor = packFloatToRgba(depth);
}