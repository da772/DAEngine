#include <bgfx_shader.sh>
#include "samplers.sh"
#include "common.sh"

SAMPLER2D(s_texture, SAMPLER_DEFERRED_DEPTH);

uniform vec4 u_pixelSize;

void main()
{
    vec2 halfpixel = 0.5 * vec2(u_pixelSize.x, u_pixelSize.y);
	vec2 oneepixel = 1.0 * vec2(u_pixelSize.x, u_pixelSize.y);
    vec2 texcoord = gl_FragCoord.xy / u_viewRect.zw;
	vec2 uv = texcoord.xy;

	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);

	sum += (4.0/32.0) * texture2D(s_texture, uv).rgba;

	sum += (4.0/32.0) * texture2D(s_texture, uv + vec2(-halfpixel.x, -halfpixel.y) );
	sum += (4.0/32.0) * texture2D(s_texture, uv + vec2(+halfpixel.x, +halfpixel.y) );
	sum += (4.0/32.0) * texture2D(s_texture, uv + vec2(+halfpixel.x, -halfpixel.y) );
	sum += (4.0/32.0) * texture2D(s_texture, uv + vec2(-halfpixel.x, +halfpixel.y) );

	sum += (2.0/32.0) * texture2D(s_texture, uv + vec2(+oneepixel.x, 0.0) );
	sum += (2.0/32.0) * texture2D(s_texture, uv + vec2(-oneepixel.x, 0.0) );
	sum += (2.0/32.0) * texture2D(s_texture, uv + vec2(0.0, +oneepixel.y) );
	sum += (2.0/32.0) * texture2D(s_texture, uv + vec2(0.0, -oneepixel.y) );

	sum += (1.0/32.0) * texture2D(s_texture, uv + vec2(+oneepixel.x, +oneepixel.y) );
	sum += (1.0/32.0) * texture2D(s_texture, uv + vec2(-oneepixel.x, +oneepixel.y) );
	sum += (1.0/32.0) * texture2D(s_texture, uv + vec2(+oneepixel.x, -oneepixel.y) );
	sum += (1.0/32.0) * texture2D(s_texture, uv + vec2(-oneepixel.x, -oneepixel.y) );

	gl_FragColor.xyzw = sum;
}
