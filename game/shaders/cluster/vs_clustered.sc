$input a_position, a_normal, a_tangent, a_texcoord0
$output v_worldpos, v_normal, v_tangent, v_texcoord0, v_shadowcoord0, v_shadowcoord1,v_shadowcoord2

#include <bgfx_shader.sh>

uniform mat3 u_normalMatrix;
uniform mat4 u_sunLightMtx[3];

void main()
{
    v_worldpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    v_normal = mul(u_normalMatrix, a_normal);
    v_tangent = mul(u_model[0], vec4(a_tangent, 0.0)).xyz;
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    vec3 normal = v_normal * 2.0 - 1.0;
    const float shadowMapOffset = 0.001;
	vec3 posOffset = v_worldpos + normal * shadowMapOffset;
    v_shadowcoord0 = mul(u_sunLightMtx[0], vec4(posOffset, 1.0) );
    v_shadowcoord1 = mul(u_sunLightMtx[1], vec4(posOffset, 1.0) );
    v_shadowcoord2 = mul(u_sunLightMtx[2], vec4(posOffset, 1.0) );
}
