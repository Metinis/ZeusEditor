#version 410 core

#define PI 3.14159265359
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_AOMap;
uniform samplerCube u_IrradianceMap;
uniform samplerCube u_PrefilterMap;
uniform sampler2D u_BRDFLUT;

layout(std140) uniform Globals {
  vec3 u_LightDir;
  vec3 u_CameraPos;
//float u_Time;
  vec3 u_AmbientColor;
};

layout(std140) uniform Material {
  vec4 u_Albedo;     //RGB in xyz, ignore w
  vec4 u_Params;     //x = metallic, y = roughness, z = ao, w = if 0, non metal, if > 0 metal
};

in vec4 v_Color;
in vec2 v_UV;
in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 fragColor;

float ggxDistribution(float nDotH, float roughness){
  // a = surface roucghness, when 0, smooth, when 1, rough
  // D = a^2 / PI * (dot(normal, half)^2 * (a^2-1) + 1)^2
  float alpha2 = roughness * roughness * roughness * roughness;
  float d = nDotH * nDotH * (alpha2 - 1) + 1;
  float ggxdistrib = alpha2 / (PI * d * d);
  return ggxdistrib;
}
float geomSmith(float dp, float roughness){
  // G = dot(normal, view) / (dot(normal, view)*(1 - K) + K)
  // K = ((a+1)^2) / 8
  float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
  float denom = dp * (1 - k) + k;
  return dp / denom;
}
vec3 schlickFresnel(float vDotH, vec3 F0, float roughness){
  //F = F0 + (1-F0)*(1-dot(view, half))^5
  //F0 = base reflectivity of the surface
  //vec3 F = F0 + (1 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5);
  return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5.0);
  //return F;
}
//kd + ks = 1 always
vec3 getNormalFromMapObjectSpace() {
  vec3 N = texture(u_NormalMap, v_UV).rgb;
  N = N * 2.0 - 1.0; // remap [0,1] -> [-1,1]
  return normalize(N);
}

void main() {
  vec3 albedo = pow(texture(u_AlbedoMap, v_UV).rgb, vec3(2.2)) * u_Albedo.rgb;
  vec3 normal = texture(u_NormalMap, v_UV).rgb;
  float metallic = texture(u_MetallicMap, v_UV).r * u_Params.x;
  float roughness = texture(u_RoughnessMap, v_UV).r * u_Params.y;
  float ao = texture(u_AOMap, v_UV).r + u_Params.z * u_Params.z;

  //vec3 N = normal;
  vec3 N = normalize(v_Normal);
  vec3 V = normalize(u_CameraPos - v_FragPos);
  vec3 L = normalize(-u_LightDir);
  vec3 H = normalize(V + L);

  float nDotL = max(dot(N, L), 0.0);
  float nDotV = max(dot(N, V), 0.0);
  float nDotH = max(dot(N, H), 0.0);
  float vDotH = max(dot(V, H), 0.0);



  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);
  /*if(u_Params.w > 0.0){
    F0 = albedo;
  }*/
  vec3 F = schlickFresnel(vDotH, F0, roughness);

  float G = geomSmith(nDotV, roughness) * geomSmith(nDotL, roughness);

  float D = ggxDistribution(nDotH, roughness);

  vec3 numerator = D * G * F;
  float denominator = 4.0 * nDotV * nDotL + 0.001;
  vec3 specular = numerator / denominator;

  vec3 kd = (1.0 - F) * (1.0 - metallic);
  vec3 diffuse = kd * albedo / PI;

  vec3 R = reflect(-V, N);
  const float MAX_REFLECTION_LOD = 4.0;
  vec3 prefilteredColor = textureLod(u_PrefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
  vec2 brdf  = texture(u_BRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
  vec3 specularIBL = prefilteredColor * (F * brdf.x + brdf.y);

  vec3 lightColor = vec3(1.0);
  vec3 irradiance = texture(u_IrradianceMap, N).rgb;
  vec3 diffuseIBL = irradiance * albedo;
  vec3 ambient = (diffuseIBL + specularIBL) * ao;

  vec3 color = (diffuse + specular) * lightColor * nDotL + ambient;
  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0/2.2)); //gamma correcltion

  fragColor = vec4(color, 1.0);
}

