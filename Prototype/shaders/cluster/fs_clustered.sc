$input v_worldpos, v_normal, v_tangent, v_texcoord0, v_shadowcoord0, v_shadowcoord1,v_shadowcoord2,v_shadowcoord3

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
    // Shadow mapping   
    float visibility = shadowPass(v_shadowcoord0, v_shadowcoord1, v_shadowcoord2, v_shadowcoord3);

    // Lighting pass
    vec3 radianceOut = lightPass(v_worldpos, v_normal, v_tangent, v_texcoord0, u_camPos, gl_FragCoord, visibility);

    // Output
    gl_FragColor = vec4(radianceOut, 1.0);
    
}