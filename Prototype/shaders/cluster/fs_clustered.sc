$input v_worldpos, v_normal, v_tangent, v_texcoord0, v_shadowcoord0, v_shadowcoord1,v_shadowcoord2,v_shadowcoord3,v_view

#define READ_MATERIAL
#define INCLUDE_PASS

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>
#include "util.sh"
#include "lights.sh"
#include "samplers.sh"
#include "shadow.sh"

uniform vec4 u_camPos;

void main()
{
    // Lighting pass
    vec3 radianceOut = lightPass(v_worldpos, v_normal, v_tangent, v_texcoord0, u_camPos, gl_FragCoord);
    // Shadow mapping
    radianceOut = shadowPass(radianceOut, v_view, v_normal, u_lightPos, v_shadowcoord0, v_shadowcoord1, v_shadowcoord2, v_shadowcoord3);

    // Output
    gl_FragColor = vec4(radianceOut, 1.0);
}