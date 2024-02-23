$input a_position, a_normal
$output v_position, v_normal, v_viewDir, v_shadowcoord0, v_shadowcoord1, v_shadowcoord2, v_shadowcoord3

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"

uniform vec4 u_params1;
#define u_shadowMapOffset u_params1.y

uniform mat4 u_lightMtx;
uniform mat4 u_sunLightMtx[4];

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );

	vec4 normal = vec4(a_normal, 1.0) * 2.0 - 1.0;
	v_normal = normalize(mul(u_modelView, vec4(normal.xyz, 0.0) ).xyz);
	v_viewDir = mul(u_modelView, vec4(a_position, 1.0)).xyz;

	vec4 posOffset = vec4(a_position + normal.xyz * u_shadowMapOffset, 1.0);
	v_position = mul(u_modelView, posOffset);

	vec4 wpos = vec4(mul(u_model[0], posOffset).xyz, 1.0);
	v_shadowcoord0 = mul(u_sunLightMtx[0], wpos);
	v_shadowcoord1 = mul(u_sunLightMtx[1], wpos);
	v_shadowcoord2 = mul(u_sunLightMtx[2], wpos);
	v_shadowcoord3 = mul(u_sunLightMtx[3], wpos);

	v_shadowcoord0.z += 0.5;
	v_shadowcoord1.z += 0.5;
	v_shadowcoord2.z += 0.5;
	v_shadowcoord3.z += 0.5;
}