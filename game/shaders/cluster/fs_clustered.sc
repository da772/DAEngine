$input v_worldpos, v_normal, v_tangent, v_texcoord0

#define READ_MATERIAL
#define INCLUDE_PASS
#define SM_LINEAR 1
#define SM_PCF 1
#define SM_CSM 1

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>
#include "util.sh"
#include "lights.sh"
#include "samplers.sh"
#include "shadow.sh"
#include "pbr.sh"

uniform vec4 u_camPos;

void main()
{
    // Shadow mapping   
    float visibility = shadowPass(v_worldpos, u_view, saturate(dot(normalize(v_normal), u_sunLightDirection)), v_normal);

    // Lighting pass
    vec3 radianceOut = lightPass(v_worldpos, v_normal, v_tangent, v_texcoord0, u_camPos, gl_FragCoord, visibility);

    // Output
    OUT0 = vec4(radianceOut, 1.0);
    OUT1 = vec4(0.0,0.0,0.0,1.0);
    OUT2 = v_worldpos;
    OUT3 = vec4(1.0,0.0, 0.0,0.0);
}