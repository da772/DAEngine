/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"
#include "samplers.sh"

#if SHADOW_PACKED_DEPTH
SAMPLER2DARRAY(s_shadowMap0, SAMPLER_SHADOW_MAP_NEAR);
SAMPLER2DARRAY(s_shadowMap1, SAMPLER_SHADOW_MAP_MED);
SAMPLER2DARRAY(s_shadowMap2, SAMPLER_SHADOW_MAP_FAR);
#	define Sampler sampler2D
#else
SAMPLER2DSHADOW(s_shadowMap0, SAMPLER_SHADOW_MAP_NEAR);
SAMPLER2DSHADOW(s_shadowMap1, SAMPLER_SHADOW_MAP_MED);
SAMPLER2DSHADOW(s_shadowMap2, SAMPLER_SHADOW_MAP_FAR);
#	define Sampler sampler2DShadow
#endif // SHADOW_PACKED_DEPTH

#define CASCADE_COUNT 3

uniform mat4 u_sunLightMtx[CASCADE_COUNT];
uniform vec4 u_cascadePlaneDistances[CASCADE_COUNT];

vec2 lit(vec3 _ld, vec3 _n, vec3 _vd, float _exp)
{
	//diff
	float ndotl = dot(_n, _ld);

	//spec
	vec3 r = 2.0*ndotl*_n - _ld; //reflect(_ld, _n);
	float rdotv = dot(r, _vd);
	float spec = step(0.0, ndotl) * pow(max(0.0, rdotv), _exp) * (2.0 + _exp)/8.0;

	return max(vec2(ndotl, spec), 0.0);
}

float hardShadow(Sampler _sampler, vec3 texCoord, float _bias)
{
#if SHADOW_PACKED_DEPTH
	return step(texCoord.z-_bias, unpackRgbaToFloat(texture2D(_sampler, texCoord.xy) ) );
#else
	return shadow2D(_sampler, vec3(texCoord.xy, texCoord.z-_bias) );
#endif // SHADOW_PACKED_DEPTH
}

float PCF(Sampler _sampler, vec3 _shadowCoord, float _bias, vec2 _texelSize)
{
	bool outside = any(greaterThan(_shadowCoord.xy, vec2_splat(1.0))) || any(lessThan   (_shadowCoord.xy, vec2_splat(0.0)));

	if (outside)
	{
		return 1.0;
	}

	float result = 0.0;
	vec2 offset = _texelSize;

	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-1.5, -1.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-1.5, -0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-1.5,  0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-1.5,  1.5) * offset, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-0.5, -1.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-0.5, -0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-0.5,  0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(-0.5,  1.5) * offset, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(0.5, -1.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(0.5, -0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(0.5,  0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(0.5,  1.5) * offset, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(1.5, -1.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(1.5, -0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(1.5,  0.5) * offset, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec3(vec2(1.5,  1.5) * offset, 0.0), _bias);

	return result / 16.0;
}

float calculateShadow(Sampler _sampler, float _currentDepth, vec3 _shadowCoord, float _bias, vec2 _texelSize)
{
    // PCF
    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = shadow2D(_sampler, vec3(_shadowCoord.xy + vec2(x, y) * _texelSize, _shadowCoord.z-_bias));
            shadow += (_currentDepth - _bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return 1.0-shadow;
}

float shadowPass(vec3 fragPosWorldSpace, mat4 v_view, vec4 v_lightNormal)
{
	float shadowMapBias = mix(0.005, 0.0, v_lightNormal);

	vec4 fragPosViewSpace = mul(v_view, vec4(fragPosWorldSpace, 1.0));
	float depthValue = abs(fragPosViewSpace.z);

 	int layer = -1;
    for (int i = 0; i < CASCADE_COUNT-1; ++i)
    {
        if (depthValue < u_cascadePlaneDistances[i].x)
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = 2;
    }

	vec4 fragPosLightSpace = mul(u_sunLightMtx[layer], vec4(fragPosWorldSpace, 1.0));

	vec2 texelSize = vec2_splat(1.0/4096.0);
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    //projCoords = projCoords * 0.5 + 0.5;

	float currentDepth = projCoords.z;

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 1.0;
    }
	
	bool selection0 = layer == 0;
	bool selection1 = layer == 1;
	bool selection2 = layer == 2;

	float visibility = 1.0;

	const float biasModifier = 0.5f;
	shadowMapBias *= (1 / ((u_cascadePlaneDistances[layer].x) * biasModifier));

#if 0
	if (selection0)
	{
		return vec3(1.0,0.0,0.0);
	} 
	else if (selection1)
	{
		return vec3(0.0,1.0,0.0);
	}
	else if (selection2)
	{
		return vec3(0.0,0.0,1.0);
	}
#endif

	if (selection0)
	{
		visibility = calculateShadow(s_shadowMap0, currentDepth, projCoords, shadowMapBias, texelSize);
	} 
	else if (selection1)
	{
		visibility = calculateShadow(s_shadowMap1, currentDepth, projCoords, shadowMapBias, texelSize);
	}
	else if (selection2)
	{
		visibility = calculateShadow(s_shadowMap2, currentDepth, projCoords, shadowMapBias, texelSize);
	}

	return visibility;
	
}


#if 0
void main()
{
	float shadowMapBias = 0.005;
	vec3 color = vec3_splat(1.0);

	vec3 v  = v_view;
	vec3 vd = -normalize(v);
	vec3 n  = v_normal;
	vec3 l  = u_lightPos.xyz;
	vec3 ld = -normalize(l);

	vec2 lc = lit(ld, n, vd, 1.0);

	vec2 texelSize = vec2_splat(1.0/512.0);
	float visibility = PCF(s_shadowMap, v_shadowcoord, shadowMapBias, texelSize);

	vec3 ambient = 0.1 * color;
	vec3 brdf = (lc.x + lc.y) * color * visibility;

	vec3 final = toGamma(abs(ambient + brdf) );
	gl_FragColor = vec4(final, 1.0);
}
#endif