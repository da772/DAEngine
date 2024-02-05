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
SAMPLER2DSHADOW(s_shadowMap3, SAMPLER_SHADOW_MAP_VFAR);
#	define Sampler sampler2DShadow
#endif // SHADOW_PACKED_DEPTH

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

float hardShadow(Sampler _sampler, vec4 _shadowCoord, float _bias)
{
	vec3 texCoord = _shadowCoord.xyz/_shadowCoord.w;
#if SHADOW_PACKED_DEPTH
	return step(texCoord.z-_bias, unpackRgbaToFloat(texture2D(_sampler, texCoord.xy) ) );
#else
	return shadow2D(_sampler, vec3(texCoord.xy, texCoord.z-_bias) );
#endif // SHADOW_PACKED_DEPTH
}

float PCF(Sampler _sampler, vec4 _shadowCoord, float _bias, vec2 _texelSize)
{
	vec2 texCoord = _shadowCoord.xy/_shadowCoord.w;

	bool outside = any(greaterThan(texCoord, vec2_splat(1.0)))
				|| any(lessThan   (texCoord, vec2_splat(0.0)))
				 ;

	if (outside)
	{
		return 1.0;
	}

	float result = 0.0;
	vec2 offset = _texelSize * _shadowCoord.w;

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

vec3 shadowPass(vec4 v_shadowcoord0, vec4 v_shadowcoord1, vec4 v_shadowcoord2, vec4 v_shadowcoord3)
{
	float shadowMapBias = 0.005;

	vec2 texelSize = vec2_splat(1.0/2048.0);

    /*  Debug cascaded shadow maps */
    /*
    vec3 visibility0 =  (1.0-PCF(s_shadowMap0, v_shadowcoord0, shadowMapBias, texelSize)) * vec3(1.0, 0.0, 0.0);
    vec3 visibility1 =  (1.0-PCF(s_shadowMap1, v_shadowcoord1, shadowMapBias, texelSize)) * vec3(0.0, 1.0, 0.0);
    vec3 visibility2 = (1.0-PCF(s_shadowMap2, v_shadowcoord2, shadowMapBias, texelSize)) * vec3(0.0, 0.0, 1.0);
    vec3 visibility3 = (1.0-PCF(s_shadowMap3, v_shadowcoord3, shadowMapBias, texelSize)) * vec3(1.0, 1.0, 0.0);
    
    return  vec4(visibility0 + visibility1 + visibility2 + visibility3, 1.0);
    
    */
	
    float visibility = PCF(s_shadowMap0, v_shadowcoord0, shadowMapBias, texelSize);
    visibility = min(visibility, PCF(s_shadowMap1, v_shadowcoord1, shadowMapBias, texelSize));
    visibility = min(visibility, PCF(s_shadowMap2, v_shadowcoord2, shadowMapBias, texelSize));
    visibility = min(visibility, PCF(s_shadowMap3, v_shadowcoord3, shadowMapBias, texelSize));

	float outVis = 1.0;
	for (int i = 0; i < 4; i++) {
		outVis -= 0.2*(1.0-visibility);
	}
	
	
	return outVis;
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