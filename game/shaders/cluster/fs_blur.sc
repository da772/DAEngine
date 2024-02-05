#include <bgfx_shader.sh>
#include "samplers.sh"
#include "common.sh"

SAMPLER2D(s_texture, SAMPLER_DEFERRED_DEPTH);

void main()
{
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
    
    vec2 texelSize = 1.0 / vec2(textureSize(s_texture, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture2D(s_texture, texcoord + offset).r;
        }
    }
    gl_FragColor = result / (4.0 * 4.0);
}
