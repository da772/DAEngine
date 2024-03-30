#include <bgfx_shader.sh>
#include "samplers.sh"
#include "common.sh"

SAMPLER2D(s_texture, 0);
uniform vec4 s_ssLightPos;
uniform vec4 s_params;

struct PostprocessingCoefficients
{
    int num_samples;
    float density;
    float exposure;
    float decay;
    float weight;
};

vec3 radial_blur(PostprocessingCoefficients coefficients, sampler2D occlusion_map_sampler,vec2 texcoord, vec2 screen_space_position);

void main()
{
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
    vec2 texelSize = 1.0 / vec2(textureSize(s_texture, 0));

    PostprocessingCoefficients coefficients;
    coefficients.num_samples = int(s_ssLightPos.w);
    coefficients.density  = s_params.x;
    coefficients.exposure  = s_params.y;
    coefficients.decay  = s_params.z;
    coefficients.weight  = s_params.w;

    bool apply_radial_blur = s_ssLightPos.z > 0.0;

    gl_FragColor = vec4(0.0,0.0,0.0,0.0);
    if (apply_radial_blur)
    {
        gl_FragColor = vec4(radial_blur(coefficients, s_texture, texcoord, vec2(s_ssLightPos.x, s_ssLightPos.y)), 1.0);
    }
    else
    {
        gl_FragColor = texture2D(s_texture, texcoord);
    }
}

vec3 radial_blur(PostprocessingCoefficients coefficients, sampler2D occlusion_map_sampler,vec2 texcoord, vec2 screen_space_position)
{
    vec2 delta_tex_coord = (texcoord - screen_space_position) * coefficients.density * (1.0 / float(coefficients.num_samples));
    vec2 tex_coordinates = texcoord;
    vec3 color = texture2D(occlusion_map_sampler, tex_coordinates).rgb;
    float decay = 1.0;
    for (int i = 0; i < coefficients.num_samples; ++i)
    {
        tex_coordinates -= delta_tex_coord;
        vec3 current_sample = texture2D(occlusion_map_sampler, tex_coordinates).rgb;
        current_sample *= decay * coefficients.weight;
        color += current_sample;
        decay *= coefficients.decay;
    }

    return color * coefficients.exposure;
}
