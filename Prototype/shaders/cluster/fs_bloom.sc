#include <bgfx_shader.sh>
#include "samplers.sh"
#include "common.sh"

SAMPLER2D(s_texture, SAMPLER_DEFERRED_DEPTH);

uniform vec4 params;

void main()
{
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
    vec4 result = texture2D(s_texture, texcoord);

    float brightness = dot(result.rgb, params.xyz);//vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        gl_FragColor = vec4(result.rgb, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
