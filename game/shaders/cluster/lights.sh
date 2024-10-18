#ifndef LIGHTS_SH_HEADER_GUARD
#define LIGHTS_SH_HEADER_GUARD

#include <bgfx_compute.sh>
#include "samplers.sh"
#include "clusters.sh"
#include "pbr.sh"
#include "colormap.sh"

uniform vec4 u_lightCountVec;
#define u_pointLightCount uint(u_lightCountVec.x)

uniform vec4 u_ambientLightIrradiance;
uniform vec4 u_sunLightDirection;
uniform vec4 u_sunLightRadiance;

SAMPLER2D(s_ssao, SAMPLER_SSAO);

// for each light:
//   vec4 position (w is padding)
//   vec4 intensity + radius (xyz is intensity, w is radius)
BUFFER_RO(b_pointLights, vec4, SAMPLER_LIGHTS_POINTLIGHTS);

struct PointLight
{
    vec3 position;
    // this padding is necessary for Vulkan when using the struct in a shared workgroup array
    // otherwise memory reads/writes are corrupted
    // I can't find where this is required per the spec so I'll assume this is a bug with Nvidia drivers/HW
    float _padding;
    vec3 intensity;
    float radius;
    vec3 direction;
    float angle;
};

struct AmbientLight
{
    vec3 irradiance;
};

struct SunLight
{
    vec3 direction;
    vec3 radiance;
};

// primary source:
// https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
// also really good:
// https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf

float distanceAttenuation(float distance)
{
    // only for point lights

    // physics: inverse square falloff
    // to keep irradiance from reaching infinity at really close distances, stop at 1cm
    return 1.0 / max(distance * distance, 0.01 * 0.01);
}

float smoothAttenuation(float distance, float radius)
{
    // window function with smooth transition to 0
    // radius is arbitrary (and usually artist controlled)
    float nom = saturate(1.0 - pow(distance / radius, 4.0));
    return nom * nom * distanceAttenuation(distance);
}

uint pointLightCount()
{
    return u_pointLightCount;
}

PointLight getPointLight(uint i)
{
    PointLight light;
    light.position = b_pointLights[3 * i + 0].xyz;
    vec4 intensityRadiusVec = b_pointLights[3 * i + 1];
    light.intensity = intensityRadiusVec.xyz;
    light.radius = intensityRadiusVec.w;
    vec4 dir = b_pointLights[3 * i + 2];
    light.direction = dir.xyz;
    light.angle = dir.w;
    return light;
}

SunLight getSunLight()
{
    SunLight light;
    light.direction = u_sunLightDirection.xyz;
    light.radiance = u_sunLightRadiance.xyz;
    return light;
}

AmbientLight getAmbientLight()
{
    AmbientLight light;
    light.irradiance = u_ambientLightIrradiance.xyz;
    return light;
}

#ifdef INCLUDE_PASS
vec4 lightPass(vec3 v_worldpos, vec3 v_normal, vec3 v_tangent, vec2 v_texcoord0, vec4 u_camPos, vec4 gl_FragCoord, float visibility)
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
        return vec4(whiteFurnace(NoV, mat) * msFactor, 1.0);
    }

    vec3 msFactor = multipleScatteringFactor(mat, NoV);

    vec3 radianceOut = vec3_splat(0.0);

    uint cluster = getClusterIndex(vec4(u_viewRect.z-gl_FragCoord.x, gl_FragCoord.yzw));
    LightGrid grid = getLightGrid(cluster);
    for(uint i = 0; i < grid.pointLights; i++)
    {
        uint lightIndex = getGridLightIndex(grid.offset, i);
        PointLight light = getPointLight(lightIndex);
        float dist = distance(light.position, fragPos);

        float attenuation = smoothAttenuation(dist, light.radius);
       
        if (light.angle < 0.0) {
            if (attenuation > 0.0)
            {
                vec3 L = normalize(light.position - fragPos);
                vec3 radianceIn = light.intensity * attenuation;
                float NoL = saturate(dot(N, L));
                radianceOut += BRDF(V, L, N, NoV, NoL, mat) * msFactor * radianceIn * NoL;   
            }
        } 
        else 
        {
            vec3 L = normalize(light.position - fragPos);
            vec3 lightDir = normalize(light.direction.xyz);

            float cosAngle = dot(-lightDir, L);
            float angle = acos(cosAngle);

            if (angle <= light.angle) {
                vec3 radianceIn = light.intensity * min(1.0, attenuation);
                float NoL = max(0.1, saturate(dot(N, L)));
                radianceOut += BRDF(V, L, N, NoV, NoL, mat) * msFactor * radianceIn * NoL * attenuation;                   
            }
        }
    
    }

    // directional light
    {
        SunLight light = getSunLight();
        vec3 L = light.direction.xyz;
        float NoL = saturate(dot(N, L));
        radianceOut += BRDF(V, L, N, NoV, NoL, mat) * msFactor * light.radiance * NoL;
    }

    float ambientLight = getAmbientLight().irradiance * texture2D(s_ssao, gl_FragCoord.xy/u_viewRect.zw).r;
    radianceOut += ambientLight * mat.diffuseColor;
    radianceOut += radianceOut * visibility;
    radianceOut += mat.emissive;

    return vec4(radianceOut, mat.albedo.w);
}
#endif

#endif // LIGHTS_SH_HEADER_GUARD
