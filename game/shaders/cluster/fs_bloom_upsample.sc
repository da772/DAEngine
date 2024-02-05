#include <bgfx_shader.sh>
#include "samplers.sh"
#include "common.sh"

SAMPLER2D(s_texture, SAMPLER_DEFERRED_DEPTH);

uniform vec4 u_pixelSize;
uniform vec4 u_intensity;

void main()
{
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
	vec2 halfpixel = u_pixelSize.xy;
	vec2 uv = texcoord.xy;

	vec4 sum = vec4_splat(0.0);

	sum += (2.0 / 16.0) * texture2D(s_texture, uv + vec2(-halfpixel.x,  0.0) );
	sum += (2.0 / 16.0) * texture2D(s_texture, uv + vec2( 0.0,          halfpixel.y) );
	sum += (2.0 / 16.0) * texture2D(s_texture, uv + vec2( halfpixel.x,  0.0) );
	sum += (2.0 / 16.0) * texture2D(s_texture, uv + vec2( 0.0,         -halfpixel.y) );

	sum += (1.0 / 16.0) * texture2D(s_texture, uv + vec2(-halfpixel.x, -halfpixel.y) );
	sum += (1.0 / 16.0) * texture2D(s_texture, uv + vec2(-halfpixel.x,  halfpixel.y) );
	sum += (1.0 / 16.0) * texture2D(s_texture, uv + vec2( halfpixel.x, -halfpixel.y) );
	sum += (1.0 / 16.0) * texture2D(s_texture, uv + vec2( halfpixel.x,  halfpixel.y) );

	sum += (4.0 / 16.0) * texture2D(s_texture, uv);

	gl_FragColor = u_intensity.x * sum;
}
