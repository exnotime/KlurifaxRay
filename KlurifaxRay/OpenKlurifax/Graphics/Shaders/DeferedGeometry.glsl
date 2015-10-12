#new_shader vertex
#version 330
layout (location = 0) in vec3 PosL;
layout (location = 1) in vec3 InNormal;
layout (location = 2) in vec2 InTex;
layout (location = 3) in vec3 InTangent;
uniform mat4  		worldViewProj;
uniform mat4  		world;

out vec2 Tex;
out vec3 Normal;
out vec3 Tangent;
out vec3 posW;
out mat3 TBN;

void main()
{
	gl_Position = worldViewProj * vec4(PosL, 1.0);
	Normal = normalize((world * vec4(InNormal,0.0f)).xyz);
	Tangent = normalize((world * vec4(InTangent,0.0f)).xyz);
	posW = (world * vec4(PosL,1.0)).xyz;
	Tex = InTex;

	vec3 normal = normalize(Normal);
	vec3 tangent = normalize(Tangent);
	tangent = normalize(tangent - dot(tangent,normal) * normal);
	vec3 bitangent = cross(tangent,normal);
	TBN = mat3(tangent,bitangent,normal);
}
#end_shader

#new_shader fragment
#version 330
in vec2 Tex;
in vec3 Normal;
in vec3 Tangent;
in vec3 posW;
in mat3 TBN;

struct SurfaceMaterial{
	vec3 BaseColor;
	float Diffuse;
	float Specular;
	vec3 ReflectedColor;
	float Fresnel;
	float Gloss;
	float Metalic;	//not implemented yet
	float SpecularPower;
};

uniform sampler2D	tex1;
uniform SurfaceMaterial Material;
uniform sampler2D	RougnessMap;
uniform samplerCube CubeTex;
uniform sampler2D	NormalMap;
uniform bool		UseTexture;
uniform bool		UseNormalMap;
uniform bool		UseRoughnessMap;
uniform bool		UseReflectionMap;
uniform float		Reflective;
uniform uint 		MaterialID;
uniform vec3		CamPos;

layout(location = 0) out vec3 FragmentColor;
layout(location = 1) out vec3 FragmentNormal;
layout(location = 2) out uint FragmentMatID;
layout(location = 3) out vec4 FragmentRoughness;



vec3 CalcBumpedNormal(){
	vec3 normal = normalize(Normal);
	vec3 tangent = normalize(Tangent);
	tangent = normalize(tangent - dot(tangent,normal) * normal);
	vec3 bitangent = cross(tangent,normal);

	vec3 bump = texture(NormalMap,Tex).xyz;
	bump = (bump * 2.0) - 1.0;

	vec3 newNormal;
	mat3 TBN = mat3(tangent,bitangent,normal);
	newNormal = TBN * bump;
	return normalize(newNormal);
}
void main()
{
	vec4 color = vec4(0,0,0,1);
	vec3 normal = vec3(0,0,0);

	FragmentMatID = MaterialID;
	if(UseTexture)
	{
		color = texture(tex1,Tex) * vec4(Material.BaseColor,1);//diffuse color
	} else {
		color.rgb = vec3(1,1,1) * Material.BaseColor.rgb;
	}

	if(UseRoughnessMap){
		FragmentRoughness.w = Material.Gloss * texture(RougnessMap,Tex).r;
	} else {
		FragmentRoughness.w = Material.Gloss;
	}

	if(UseNormalMap){
  		normal = texture(NormalMap,Tex).rgb * 2.0 - 1.0;
  		normal = normalize(TBN * normal);
	} else {
		normal = normalize(Normal);
	}
	FragmentNormal.xyz = normal;

	if(UseReflectionMap) {
		vec3 toEye = posW - CamPos;
		vec3 reflected = reflect(toEye,normal);
		FragmentRoughness.rgb = texture(CubeTex, reflected).rgb;
	} else {
		FragmentRoughness.rgb = Material.ReflectedColor;
	}

	if(color.a == 0.0f){
		discard;
	}
	 else {
		FragmentColor = color.rgb;
	}
}
#end_shader