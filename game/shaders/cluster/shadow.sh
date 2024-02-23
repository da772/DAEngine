/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "../common.sh"
#include "samplers.sh"

#define u_ambientPass    u_params0.x
#define u_lightingPass   u_params0.y

#define u_shadowMapBias   u_params1.x
#define u_shadowMapParam0 u_params1.z
#define u_shadowMapParam1 u_params1.w

#define u_shadowMapShowCoverage u_params2.y
#define u_shadowMapTexelSize    u_params2.z

// Pcf
#define u_shadowMapPcfMode     u_shadowMapParam0
#define u_shadowMapNoiseAmount u_shadowMapParam1

// Vsm
#define u_shadowMapMinVariance     u_shadowMapParam0
#define u_shadowMapDepthMultiplier u_shadowMapParam1

// Esm
#define u_shadowMapHardness        u_shadowMapParam0
#define u_shadowMapDepthMultiplier u_shadowMapParam1

uniform vec4 u_params1;
#define u_shadowMapOffset u_params1.y
uniform vec4 u_smSamplingParams;

SAMPLER2D(s_shadowMap0, SAMPLER_SHADOW_MAP_NEAR);
SAMPLER2D(s_shadowMap1, SAMPLER_SHADOW_MAP_MED);
SAMPLER2D(s_shadowMap2, SAMPLER_SHADOW_MAP_FAR);
SAMPLER2D(s_shadowMap3, SAMPLER_SHADOW_MAP_VFAR);

float hardShadow(sampler2D _sampler, vec4 _shadowCoord, float _bias)
{
	vec2 texCoord = _shadowCoord.xy/_shadowCoord.w;

	bool outside = any(greaterThan(texCoord, vec2_splat(1.0)))
				|| any(lessThan   (texCoord, vec2_splat(0.0)))
				 ;

	if (outside)
	{
		return 1.0;
	}

	float receiver = (_shadowCoord.z-_bias)/_shadowCoord.w;
	float occluder = unpackRgbaToFloat(texture2D(_sampler, texCoord) );

	float visibility = step(receiver, occluder);
	return visibility;
}


float PCF(sampler2D _sampler, vec4 _shadowCoord, float _bias, vec4 _pcfParams, vec2 _texelSize)
{
	float result = 0.0;
	vec2 offset = _pcfParams.zw * _texelSize * _shadowCoord.w;

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5,  1.5) * offset, 0.0, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5,  1.5) * offset, 0.0, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5,  1.5) * offset, 0.0, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5,  1.5) * offset, 0.0, 0.0), _bias);

	return result / 16.0;
}

float computeVisibility(sampler2D _sampler
					  , vec4 _shadowCoord
					  , float _bias
					  , vec4 _samplingParams
					  , vec2 _texelSize
					  , float _depthMultiplier
					  , float _minVariance
					  , float _hardness
					  )
{
	float visibility;
	vec4 shadowcoord = vec4(_shadowCoord.xy / _shadowCoord.w, _shadowCoord.z, 1.0);
	visibility = PCF(_sampler, shadowcoord, _bias, _samplingParams, _texelSize);
	//visibility = hardShadow(_sampler, shadowcoord, _bias);
	return visibility;
}

float texcoordInRange(vec2 _texcoord)
{
	bool inRange = all(greaterThan(_texcoord, vec2_splat(0.0)))
				&& all(lessThan   (_texcoord, vec2_splat(1.0)))
				 ;

	return float(inRange);
}

float shadowPass(vec4 v_texcoord1, vec4 v_texcoord2, vec4 v_texcoord3, vec4 v_texcoord4)
{
	vec3 colorCoverage;
	float visibility;

	vec2 texelSize = vec2_splat(1.0/2048.0);

   	//vec2 texelSize = vec2_splat(u_shadowMapTexelSize);

	vec2 texcoord1 = v_texcoord1.xy/v_texcoord1.w;
	vec2 texcoord2 = v_texcoord2.xy/v_texcoord2.w;
	vec2 texcoord3 = v_texcoord3.xy/v_texcoord3.w;
	vec2 texcoord4 = v_texcoord4.xy/v_texcoord4.w;

	bool selection0 = all(lessThan(texcoord1, vec2_splat(0.99))) && all(greaterThan(texcoord1, vec2_splat(0.01)));
	bool selection1 = all(lessThan(texcoord2, vec2_splat(0.99))) && all(greaterThan(texcoord2, vec2_splat(0.01)));
	bool selection2 = all(lessThan(texcoord3, vec2_splat(0.99))) && all(greaterThan(texcoord3, vec2_splat(0.01)));
	bool selection3 = all(lessThan(texcoord4, vec2_splat(0.99))) && all(greaterThan(texcoord4, vec2_splat(0.01)));

	if (selection0)
	{
		vec4 shadowcoord = v_texcoord1;

		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(-coverage, coverage, -coverage);
		visibility = computeVisibility(s_shadowMap0
						, shadowcoord
						, u_shadowMapBias
						, u_smSamplingParams
						, texelSize
						, u_shadowMapDepthMultiplier
						, u_shadowMapMinVariance
						, u_shadowMapHardness
						);
	}
	else if (selection1)
	{
		vec4 shadowcoord = v_texcoord2;

		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(coverage, coverage, -coverage);
		visibility = computeVisibility(s_shadowMap1
						, shadowcoord
						, u_shadowMapBias
						, u_smSamplingParams
						, texelSize/2.0
						, u_shadowMapDepthMultiplier
						, u_shadowMapMinVariance
						, u_shadowMapHardness
						);
	}
	else if (selection2)
	{
		vec4 shadowcoord = v_texcoord3;

		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(-coverage, -coverage, coverage);
		visibility = computeVisibility(s_shadowMap2
						, shadowcoord
						, u_shadowMapBias
						, u_smSamplingParams
						, texelSize/3.0
						, u_shadowMapDepthMultiplier
						, u_shadowMapMinVariance
						, u_shadowMapHardness
						);
	}
	else //selection3
	{
		vec4 shadowcoord = v_texcoord4;

		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(coverage, -coverage, -coverage);
		visibility = computeVisibility(s_shadowMap3
						, shadowcoord
						, u_shadowMapBias
						, u_smSamplingParams
						, texelSize/4.0
						, u_shadowMapDepthMultiplier
						, u_shadowMapMinVariance
						, u_shadowMapHardness
						);
	}
	
	
	return visibility;
}