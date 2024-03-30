$input a_position, a_normal, a_tangent, a_texcoord0, a_indices, a_weight
$output v_worldpos, v_normal, v_tangent, v_texcoord0

#include <bgfx_shader.sh>

#define MAX_BONES 128

uniform mat3 u_normalMatrix;
uniform mat4 u_bones[MAX_BONES];

void main()
{
    mat4 boneTransform = u_bones[a_indices.x] * a_weight.x;
    boneTransform += u_bones[a_indices.y] *  a_weight.y;
    boneTransform += u_bones[a_indices.z] *  a_weight.z;
    boneTransform += u_bones[a_indices.w] *  a_weight.w;
    
    v_worldpos = mul(u_model[0], mul(boneTransform, vec4(a_position, 1.0)));
    v_normal = mul(u_normalMatrix, mul(boneTransform, a_normal));
    v_tangent = mul(u_model[0], mul(boneTransform, vec4(a_tangent, 0.0))).xyz;
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_modelViewProj, mul(boneTransform, vec4(a_position, 1.0)));
}