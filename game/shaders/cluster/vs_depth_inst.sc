$input a_position, i_data0, i_data1, i_data2, i_data3

#include <bgfx_shader.sh>

void main()
{
    mat4 model = mtxFromCols(i_data0, i_data1, i_data2, i_data3);
    gl_Position = mul(u_viewProj, mul(u_model[0], mul(model, vec4(a_position, 1.0))));
}