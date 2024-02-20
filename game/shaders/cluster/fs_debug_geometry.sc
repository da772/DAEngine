$input v_worldpos, v_normal

#include <bgfx_shader.sh>

uniform vec4 u_Color;

void main()
{
    vec4 color = u_Color;
    if (color.a <= 0.1) {
        color = vec4(v_normal, 1.f);
    }

    gl_FragColor = color;
}