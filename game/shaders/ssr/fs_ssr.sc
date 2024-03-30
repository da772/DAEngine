#include <bgfx_shader.sh>
#include "common.sh"

SAMPLER2D(u_texture1, 0);
SAMPLER2D(u_texture2, 1);
SAMPLER2D(u_texture3, 2);

vec3 normal_from_depth(sampler2D depthSampler, float depth, vec2 texcoords) {
  
  const vec2 offset1 = vec2(0.0,0.001);
  const vec2 offset2 = vec2(0.001,0.0);
  
  float depth1 = texture2D(depthSampler, texcoords + offset1).r;
  float depth2 = texture2D(depthSampler, texcoords + offset2).r;
  
  vec3 p1 = vec3(offset1, depth1 - depth);
  vec3 p2 = vec3(offset2, depth2 - depth);
  

  vec3 normal = cross(p1, p2);
  normal.z = -normal.z;
  
  return normalize(normal);
}

void main()
{
    vec2 texSize  = u_viewRect.zw;
    vec2 texCoord = gl_FragCoord.xy / texSize;
    float depthPos = texture2D(u_texture2, texCoord.xy).r;
    vec4 positionFrom = texture2D(u_texture3, texCoord.xy).xzyw;
    vec3 normal = normal_from_depth(u_texture2, depthPos, texCoord.xy);

    float maxDistance = 8;
    float resolution  = 0.3;
    int   steps       = 5;
    float thickness   = 0.5;

    vec4 uv = vec4(0.0, 0.0,0.0,0.0);

    //vec4 mask         = texture(maskTexture,     texCoord);

    if (  positionFrom.w <= 0.0
       // || mask.r         <= 0.0
        ) { discard; }

    vec3 unitPositionFrom = normalize(positionFrom.xyz);
    vec3 pivot            = normalize(reflect(unitPositionFrom, normal));

    vec4 positionTo = positionFrom;

    vec4 startView = vec4(positionFrom.xyz + (pivot *         0.0), 1.0);
    vec4 endView   = vec4(positionFrom.xyz + (pivot * maxDistance), 1.0);

    vec4 startFrag      = startView;
        startFrag      = mul(u_proj, startFrag);
        startFrag.xyz /= startFrag.w;
        startFrag.xy   = startFrag.xy * 0.5 + 0.5;
        startFrag.xy  *= texSize;

    vec4 endFrag      = endView;
        endFrag      = mul(u_proj, endFrag);
        endFrag.xyz /= endFrag.w;
        endFrag.xy   = endFrag.xy * 0.5 + 0.5;
        endFrag.xy  *= texSize;

    vec2 frag  = startFrag.xy;
        uv.xy = frag / texSize;

    float deltaX    = endFrag.x - startFrag.x;
    float deltaY    = endFrag.y - startFrag.y;
    float useX      = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
    float delta     = mix(abs(deltaY), abs(deltaX), useX) * clamp(resolution, 0.0, 1.0);
    vec2  increment = vec2(deltaX, deltaY) / max(delta, 0.001);

    float search0 = 0;
    float search1 = 0;

    int hit0 = 0;
    int hit1 = 0;

    float viewDistance = startView.y;
    float depth        = thickness;

    float i = 0;

    [unroll(75)]
    for (i = 0; i < int(delta); ++i) {
        frag      += increment;
        uv.xy      = frag / texSize;
        positionTo = texture2D(u_texture3, uv.xy);

        search1 =
        mix
            ( (frag.y - startFrag.y) / deltaY
            , (frag.x - startFrag.x) / deltaX
            , useX
            );

        search1 = clamp(search1, 0.0, 1.0);

        viewDistance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
        depth        = viewDistance - positionTo.y;

        if (depth > 0 && depth < thickness) {
        hit0 = 1;
        break;
        } else {
        search0 = search1;
        }
    }

    search1 = search0 + ((search1 - search0) / 2.0);

    steps *= hit0;

    [unroll(75)]
    for (i = 0; i < steps; ++i) {
        frag       = mix(startFrag.xy, endFrag.xy, search1);
        uv.xy      = frag / texSize;
        positionTo = texture2D(u_texture3, uv.xy).xzyw;

        viewDistance = (startView.y * endView.y) / mix(endView.y, startView.y, search1);
        depth        = viewDistance - positionTo.y;

        if (depth > 0 && depth < thickness) {
        hit1 = 1;
        search1 = search0 + ((search1 - search0) / 2);
        } else {
        float temp = search1;
        search1 = search1 + ((search1 - search0) / 2);
        search0 = temp;
        }
    }

    float visibility =
        hit1
        * positionTo.w
        * ( 1
        - max
            ( dot(-unitPositionFrom, pivot)
            , 0
            )
        )
        * ( 1
        - clamp
            ( depth / thickness
            , 0
            , 1
            )
        )
        * ( 1
        - clamp
            (   length(positionTo - positionFrom)
                / maxDistance
            , 0
            , 1
            )
        )
        * (uv.x < 0 || uv.x > 1 ? 0 : 1)
        * (uv.y < 0 || uv.y > 1 ? 0 : 1);

    visibility = clamp(visibility, 0, 1);

    uv.ba = vec2(visibility, visibility);

    gl_FragColor = vec4(texture2D(u_texture1, uv.xy).xyz, 1.0);
}
