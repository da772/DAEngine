#ifndef SAMPLERS_SH_HEADER_GUARD
#define SAMPLERS_SH_HEADER_GUARD

// shared

#define SAMPLER_PBR_ALBEDO_LUT 0

#define SAMPLER_PBR_BASECOLOR 1
#define SAMPLER_PBR_METALROUGHNESS 2
#define SAMPLER_PBR_NORMAL 3
#define SAMPLER_PBR_OCCLUSION 4
#define SAMPLER_PBR_EMISSIVE 5

#define SAMPLER_LIGHTS_POINTLIGHTS 6

#define SAMPLER_SSAO 11


// per renderer

#define SAMPLER_CLUSTERS_CLUSTERS 7
#define SAMPLER_CLUSTERS_LIGHTINDICES 8
#define SAMPLER_CLUSTERS_LIGHTGRID 9
#define SAMPLER_CLUSTERS_ATOMICINDEX 10

#define SAMPLER_DEFERRED_DIFFUSE_A 7
#define SAMPLER_DEFERRED_NORMAL 8
#define SAMPLER_DEFERRED_F0_METALLIC 9
#define SAMPLER_DEFERRED_EMISSIVE_OCCLUSION 10
#define SAMPLER_DEFERRED_DEPTH 11

#define SAMPLER_SHADOW_MAP_NEAR 12
#define SAMPLER_SHADOW_MAP_MED 13
#define SAMPLER_SHADOW_MAP_FAR 14
#define SAMPLER_SHADOW_MAP_VFAR 15

#endif // SAMPLERS_SH_HEADER_GUARD