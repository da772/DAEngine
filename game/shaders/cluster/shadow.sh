/*
 * Copyright 2013-2014 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#include "../common.sh"
#include "samplers.sh"

#include "./csm/fs_shadowmaps_color_lighting.sh"

#define u_ambientPass    u_params0.x
#define u_lightingPass   u_params0.y

#define u_shadowMapBias   u_params1.x
#define u_shadowMapParam0 u_params1.z
#define u_shadowMapParam1 u_params1.w

#define u_shadowMapShowCoverage u_params2.y
#define u_shadowMapTexelSize    u_params2.z

#define u_spotDirection   u_lightSpotDirectionInner.xyz
#define u_spotInner       u_lightSpotDirectionInner.w
#define u_lightAttnParams u_lightAttenuationSpotOuter.xyz
#define u_spotOuter       u_lightAttenuationSpotOuter.w

// Pcf
#define u_shadowMapPcfMode     u_shadowMapParam0
#define u_shadowMapNoiseAmount u_shadowMapParam1

// Vsm
#define u_shadowMapMinVariance     u_shadowMapParam0
#define u_shadowMapDepthMultiplier u_shadowMapParam1

// Esm
#define u_shadowMapHardness        u_shadowMapParam0
#define u_shadowMapDepthMultiplier u_shadowMapParam1

#define u_shadowMapOffset u_params1.y

uniform mat4 u_shadowMapMtx0;
uniform mat4 u_shadowMapMtx1;
uniform mat4 u_shadowMapMtx2;
uniform mat4 u_shadowMapMtx3;

float shadowPass(vec3 fragPosWorldSpace, mat4 v_view, vec4 v_lightNormal, vec3 v_normal)
{
	vec4 fragPosViewSpace = mul(v_view, vec4(fragPosWorldSpace, 1.0));
	float depthValue = abs(fragPosViewSpace.z);

	bool selection0 = depthValue < u_csmFarDistances.x;
	bool selection1 = depthValue < u_csmFarDistances.y;
	bool selection2 = depthValue < u_csmFarDistances.z;
	bool selection3 = depthValue < u_csmFarDistances.w;

	

	vec4 v_texcoord1 = mul(u_shadowMapMtx0, vec4(fragPosWorldSpace, 1.0));
	vec4 v_texcoord2 = mul(u_shadowMapMtx1, vec4(fragPosWorldSpace, 1.0));
	vec4 v_texcoord3 = mul(u_shadowMapMtx2, vec4(fragPosWorldSpace, 1.0));
	vec4 v_texcoord4 = mul(u_shadowMapMtx3, vec4(fragPosWorldSpace, 1.0));

	vec3 colorCoverage;
	float visibility = 1.0;

	vec2 texelSize = vec2_splat(u_shadowMapTexelSize);

	vec2 texcoord1 = v_texcoord1.xy/v_texcoord1.w;
	vec2 texcoord2 = v_texcoord2.xy/v_texcoord2.w;
	vec2 texcoord3 = v_texcoord3.xy/v_texcoord3.w;
	vec2 texcoord4 = v_texcoord4.xy/v_texcoord4.w;

	//selection0 = selection0 && all(lessThan(texcoord1, vec2_splat(0.95))) && all(greaterThan(texcoord1, vec2_splat(0.05)));
	//selection1 = selection1 && all(lessThan(texcoord2, vec2_splat(0.95))) && all(greaterThan(texcoord2, vec2_splat(0.05)));
	//selection2 = selection2 && all(lessThan(texcoord3, vec2_splat(0.95))) && all(greaterThan(texcoord3, vec2_splat(0.05)));
	//selection3 = selection3 && all(lessThan(texcoord4, vec2_splat(0.95))) && all(greaterThan(texcoord4, vec2_splat(0.05)));

	float bias = max(.05 * v_lightNormal, u_shadowMapBias);;

	const float biasModifier = 0.5;

	if (selection0)
	{
		vec4 shadowcoord = v_texcoord1;
		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		bias *= biasModifier;
		colorCoverage = vec3(-coverage, coverage, -coverage);
		visibility = computeVisibility(s_shadowMap0
						, shadowcoord
						, bias
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
		bias *= biasModifier;
		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(coverage, coverage, -coverage);
		visibility = computeVisibility(s_shadowMap1
						, shadowcoord
						, bias
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
		bias *= biasModifier;
		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(-coverage, -coverage, coverage);
		visibility = computeVisibility(s_shadowMap2
						, shadowcoord
						, bias
						, u_smSamplingParams
						, texelSize/3.0
						, u_shadowMapDepthMultiplier
						, u_shadowMapMinVariance
						, u_shadowMapHardness
						);
	}
	else if(selection3)//selection3
	{
		vec4 shadowcoord = v_texcoord4;
		bias *= biasModifier;
		float coverage = texcoordInRange(shadowcoord.xy/shadowcoord.w) * 0.4;
		colorCoverage = vec3(coverage, -coverage, -coverage);
		visibility = computeVisibility(s_shadowMap3
						, shadowcoord
						, bias
						, u_smSamplingParams
						, texelSize/4.0
						, u_shadowMapDepthMultiplier
						, u_shadowMapMinVariance
						, u_shadowMapHardness
						);
	}

 	
	return visibility;
}