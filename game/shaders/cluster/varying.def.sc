vec3 a_position  : POSITION;
vec3 a_normal    : NORMAL;
vec3 a_tangent   : TANGENT;
vec2 a_texcoord0 : TEXCOORD0;
ivec4 a_indices  : BLENDINDICES;
vec4 a_weight  : BLENDWEIGHT;

vec3 v_worldpos  : POSITION1 = vec3(0.0, 0.0, 0.0);
vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 0.0);
vec3 v_tangent   : TANGENT   = vec3(0.0, 0.0, 0.0);
vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);
vec4 v_position : POSITION2 = vec4(0.0,0.0,0,0);
vec2 v_screenPos : TEXCOORD5 = vec2(0.0, 0.0);
vec3 v_skyColor  : TEXCOORD6 = vec3(0.0, 0.0, 1.0);
vec3 v_viewDir   : TEXCOORD7 = vec3(0.0, 0.0, 1.0);