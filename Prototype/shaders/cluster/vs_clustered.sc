$input a_position, a_normal, a_tangent, a_texcoord0
$output v_worldpos, v_normal, v_tangent, v_texcoord0, v_shadowcoord, v_view

#include <bgfx_shader.sh>

uniform mat3 u_normalMatrix;
uniform mat4 u_sunLightMtx;

void main()
{
    v_worldpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    v_normal = mul(u_normalMatrix, a_normal);
    v_tangent = mul(u_model[0], vec4(a_tangent, 0.0)).xyz;
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    vec3 normal = a_normal * 2.0 - 1.0;
    const float shadowMapOffset = 0.001;
	vec3 posOffset = a_position + normal * shadowMapOffset;
    v_shadowcoord = mul(u_sunLightMtx, vec4(posOffset, 1.0) );
}
