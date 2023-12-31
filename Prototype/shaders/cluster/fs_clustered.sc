$input v_worldpos, v_normal, v_tangent, v_texcoord0, v_shadowcoord, v_view

#define READ_MATERIAL

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>
#include "util.sh"
#include "pbr.sh"
#include "lights.sh"
#include "clusters.sh"
#include "colormap.sh"
#include "samplers.sh"

#if 1
#include "shadow.sh"
#endif

uniform vec4 u_camPos;

#if 1

void main()
{
    // the clustered shading fragment shader is almost identical to forward shading
    // first we determine the cluster id from the fragment's window coordinates
    // light count is read from the grid instead of a uniform
    // light indices are read and looped over starting from the grid offset

    PBRMaterial mat = pbrMaterial(v_texcoord0);
    vec3 N = convertTangentNormal(v_normal, v_tangent, mat.normal);
    mat.a = specularAntiAliasing(N, mat.a);

    vec3 camPos = u_camPos.xyz;
    vec3 fragPos = v_worldpos;

    vec3 V = normalize(camPos - fragPos);
    float NoV = abs(dot(N, V)) + 1e-5;

    if(whiteFurnaceEnabled())
    {
        mat.F0 = vec3_splat(1.0);
        vec3 msFactor = multipleScatteringFactor(mat, NoV);
        vec3 radianceOut = whiteFurnace(NoV, mat) * msFactor;
        gl_FragColor = vec4(radianceOut, 1.0);
        return;
    }

    vec3 msFactor = multipleScatteringFactor(mat, NoV);

    vec3 radianceOut = vec3_splat(0.0);

    uint cluster = getClusterIndex(gl_FragCoord);
    LightGrid grid = getLightGrid(cluster);
    for(uint i = 0; i < grid.pointLights; i++)
    {
        uint lightIndex = getGridLightIndex(grid.offset, i);
        PointLight light = getPointLight(lightIndex);
        float dist = distance(light.position, fragPos);
        float attenuation = smoothAttenuation(dist, light.radius);
        if(attenuation > 0.0)
        {
            vec3 L = normalize(light.position - fragPos);
            vec3 radianceIn = light.intensity * attenuation;
            float NoL = saturate(dot(N, L));
            radianceOut += BRDF(V, L, N, NoV, NoL, mat) * msFactor * radianceIn * NoL;
        }
    }

    // directional light
    {
        SunLight light = getSunLight();
        vec3 L = -light.direction.xyz;
        float NoL = saturate(dot(N, L));
        radianceOut += BRDF(V, L, N, NoV, NoL, mat) * msFactor * light.radiance * NoL;
    }


    radianceOut += getAmbientLight().irradiance * mat.diffuseColor * mat.occlusion;
    radianceOut += mat.emissive;

    //float shadow = ShadowCalculation(v_shadowcoord, v_normal, u_lightPos, v_worldpos);  

    float shadowMapBias = 0.005;
	vec3 color = radianceOut;
    vec3 v  = -v_view;
	vec3 vd = -normalize(v);
	vec3 n  = v_normal;
	vec3 l  = u_lightPos.xyz;
	vec3 ld = -normalize(l);

	vec2 lc = lit(ld, n, vd, 1.0);

	vec2 texelSize = vec2_splat(1.0/512.0);
	float visibility = PCF(s_shadowMap, v_shadowcoord, shadowMapBias, texelSize);

	vec3 ambient = 0.1 * color;
	vec3 brdf = (lc.x + lc.y) * color * visibility;

	vec3 final = toGamma(abs(radianceOut + brdf) );
    gl_FragColor = vec4(final, 1.0);
    //gl_FragColor.rgb = radianceOut;
    //gl_FragColor.a = mat.albedo.a;
}
#endif