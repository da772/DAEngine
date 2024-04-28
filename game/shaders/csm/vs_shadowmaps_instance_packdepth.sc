$input a_position,i_data0, i_data1, i_data2, i_data3
$output v_position

/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "csmCommon.sh"

void main()
{
    mat4 model = mtxFromCols(i_data0, i_data1, i_data2, i_data3);

	v_position = mul(u_model[0], mul(model, vec4(a_position, 1.0)));
    gl_Position = mul(u_viewProj, v_position);
}
