$input a_position, a_normal, a_tangent, a_texcoord0, i_data0, i_data1, i_data2, i_data3
$output v_worldpos, v_normal, v_tangent, v_texcoord0

#include <bgfx_shader.sh>

uniform mat3 u_normalMatrix;

void main()
{
    mat4 model = mtxFromCols(i_data0, i_data1, i_data2, i_data3);

	v_worldpos = mul(u_model[0], mul(model, vec4(a_position, 1.0)));
    v_normal = a_normal;
    v_tangent = mul(model, vec4(a_tangent, 0.0)).xyz;
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_viewProj, vec4(v_worldpos, 1.0));
}