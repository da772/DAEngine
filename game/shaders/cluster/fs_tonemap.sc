#include <bgfx_shader.sh>
#include "tonemapping.sh"

uniform vec4 u_exposureVec;
#define u_exposure u_exposureVec.x

uniform vec4 u_tonemappingModeVec;
#define u_tonemappingMode int(u_tonemappingModeVec.x)

#define TONEMAP_NONE 0
#define TONEMAP_EXPONENTIAL 1
#define TONEMAP_REINHARD 2
#define TONEMAP_REINHARD_LUM 3
#define TONEMAP_HABLE 4
#define TONEMAP_DUIKER 5
#define TONEMAP_ACES 6
#define TONEMAP_ACES_LUM 7

SAMPLER2D(s_texColor, 0);
SAMPLER2D(s_texBloom, 1);
SAMPLER2D(s_texVolLight, 2);

void main()
{
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
    vec4 result = texture2D(s_texColor, texcoord);
    result.rgb *= u_exposure;

    switch(u_tonemappingMode)
    {
        default:
        case TONEMAP_NONE:
            result.rgb = saturate(result.rgb);
            break;
        case TONEMAP_EXPONENTIAL:
            result.rgb = tonemap_exponential(result.rgb);
            break;
        case TONEMAP_REINHARD:
            result.rgb = tonemap_reinhard(result.rgb);
            break;
        case TONEMAP_REINHARD_LUM:
            result.rgb = tonemap_reinhard_luminance(result.rgb);
            break;
        case TONEMAP_HABLE:
            result.rgb = tonemap_hable(result.rgb);
            break;
        case TONEMAP_DUIKER:
            result.rgb = tonemap_duiker(result.rgb);
            break;
        case TONEMAP_ACES:
            result.rgb = tonemap_aces(result.rgb);
            break;
        case TONEMAP_ACES_LUM:
            result.rgb = tonemap_aces_luminance(result.rgb);
            break;
    }

    vec4 bloomColor = vec4(texture2D(s_texBloom, texcoord).rgb, 0.0);
    vec4 volLightColor = vec4(texture2D(s_texVolLight, texcoord).rgb, 0.0);

    gl_FragColor = result + bloomColor + volLightColor;
}