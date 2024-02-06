$input a_position
$output v_worldpos

#include <bgfx_shader.sh>

void main()
{
    v_worldpos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
}
