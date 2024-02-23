$input a_position, a_normal, a_tangent, a_texcoord0
$output v_worldpos, v_normal, v_tangent, v_texcoord0, v_shadowcoord0, v_shadowcoord1,v_shadowcoord2,v_shadowcoord3

#include <bgfx_shader.sh>

uniform mat3 u_normalMatrix;
uniform mat4 u_sunLightMtx[4];

void main()
{
    v_worldpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    v_normal = mul(u_normalMatrix, a_normal);
    v_tangent = mul(u_model[0], vec4(a_tangent, 0.0)).xyz;
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    vec3 normal = v_normal * 2.0 - 1.0;
    const float shadowMapOffset = 0.001;
    
    vec4 posOffset = vec4(a_position + normal.xyz * shadowMapOffset, 1.0);

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
