$input a_position, a_normal, a_tangent, a_texcoord0, a_indices, a_weight
/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"

#define MAX_BONES 128
uniform mat4 u_bones[MAX_BONES];

void main()
{
	mat4 boneTransform = u_bones[a_indices.x] * a_weight.x;
    boneTransform += u_bones[a_indices.y] *  a_weight.y;
    boneTransform += u_bones[a_indices.z] *  a_weight.z;
    boneTransform += u_bones[a_indices.w] *  a_weight.w;
    
    gl_Position = mul(u_modelViewProj, mul(boneTransform, vec4(a_position, 1.0)));
}