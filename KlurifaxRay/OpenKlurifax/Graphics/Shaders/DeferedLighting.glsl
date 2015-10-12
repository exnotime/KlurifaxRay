#new_shader vertex
#version 330

void main()
{

}
#end_shader 

#new_shader geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 texcoord;

void main() 
{
    gl_Position = vec4( 1.0, 1.0, 0.0, 1.0 );
    texcoord = vec2( 1.0, 1.0 );
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0 );
    texcoord = vec2( 0.0, 1.0 ); 
    EmitVertex();

    gl_Position = vec4( 1.0,-1.0, 0.0, 1.0 );
    texcoord = vec2( 1.0, 0.0 ); 
    EmitVertex();

    gl_Position = vec4(-1.0,-1.0, 0.0, 1.0 );
    texcoord = vec2( 0.0, 0.0 ); 
    EmitVertex();

    EndPrimitive(); 
}
#end_shader 

#new_shader fragment
#version 330
#extension GL_ARB_shading_language_420pack : enable
#define saturate(x) clamp(x,0.0f,1.0f)
#define MaxNrOfPointlights 200
#define MaxNrOfDirlights 10
#define MaxNrOfSpotlights 50
#define MaxNrOfMaterials 100
//declarations
struct PointLight
{
	vec3 Position;
	float Range;
	vec4 Diffuse;
	vec4 Specular;
	vec4 Ambient;
	vec3 Att;
	float padd;
}; 
struct DirLight
{
	vec3 Direction;
	vec4 Diffuse;
	vec4 Specular;
	vec4 Ambient;
};
struct SpotLight
{
	vec3 Direction;
	float Cutoff; //angle of the light, store as cosine
	vec4 Diffuse;
	vec4 Specular;
	vec4 Ambient;
	vec3 Att;
	float Range;
	vec3 Position;
	float SpotExp; // falloff from center of spot
};
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
in vec2 texcoord;
uniform sampler2D	DepthTex;
uniform sampler2D 	NormalTex;
uniform sampler2D 	colorTex;
layout(binding = 1) uniform usampler2D  MaterialTex;
uniform sampler2D 	RoughnessTex;
uniform int NrOfPointlights;
uniform int NrOfDirlights;
uniform int NrOfSpotlights;
uniform float ScreenWidth;
uniform float ScreenHeight;
uniform mat4x4		invViewProj;
uniform vec3		camPos;
out vec4 FinalLightColor;
//Uniform buffers
layout(std140) uniform MaterialBuffer{
	SurfaceMaterial Materials[MaxNrOfMaterials];
};

layout(std140) uniform PointLightBuffer{
	PointLight PointLights[MaxNrOfPointlights];
};

layout(std140) uniform DirLightBuffer{
	DirLight DirLights[MaxNrOfDirlights];
};

layout(std140) uniform SpotLightBuffer{
	SpotLight SpotLights[MaxNrOfSpotlights];
};
//Functions
vec4 CalcPointLight(in SurfaceMaterial mat, in PointLight light, in vec3 normal, in vec3 posW, in vec3 eyePos, in float roughness)
{
	vec3 lightDir = posW - light.Position;
	float dist = length(lightDir);
	if(dist > light.Range)
	{
		return vec4(0);
	} 
	lightDir = normalize(lightDir);
	//attenuation
	float att = 1 / (light.Att.x + light.Att.y * dist + light.Att.z * dist * dist); 
	float diff = 0;
	float spec = 0;
	vec4 Diffuse,Specular;
	//half lambert diffuse shading
	diff = dot(normal, -lightDir) * 0.5 + 0.5;
	//normal lambert shading
	//diff = max(0, dot(-lightDir,normal));

	if(diff > 0){
		vec3 toEye = normalize(posW - eyePos);
		vec3 reflected = reflect(lightDir,normal);
		spec = max(0.0 , dot(toEye,reflected));
		spec = pow(spec,mat.SpecularPower); // to the power of shiny!
		//schlicks fresnel aproximation
		vec3 halfWayVector = normalize(toEye + lightDir);
		float hdotl = max(dot(lightDir,halfWayVector),0);
		float fresnel = mat.Fresnel + (1.0f - mat.Fresnel) * pow(1.0 - hdotl,5.0f);
		spec *= fresnel;
	}
	Diffuse = saturate(light.Diffuse * mat.Diffuse * diff * att);
	Specular = saturate(light.Specular * mat.Specular * spec * att * roughness);
	return (light.Ambient + Diffuse + Specular);
}

vec4 CalcDirLight(in SurfaceMaterial mat, in DirLight light, in vec3 normal, in vec3 posW, in vec3 eyePos, in float roughness)
{
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float diff;
	float spec;
	//half lambert diffuse shading
	diff = dot(normal,normalize(light.Direction)) * 0.5f + 0.5f;
	//normal phong shading
	//diff = max(0,dot(normal,-light.Direction));
	if(diff > 0){
		vec3 reflected = reflect(light.Direction,normal);
		vec3 toEye = normalize(posW - eyePos);
		spec = max(0.0f,dot(toEye,reflected));
		spec = pow(spec,mat.SpecularPower);
		//fresnel
		float base = 1.0f - dot(toEye, reflected);
		float expo = pow(base, 5.0f);
		float fresnel = mix(0.028,1.0f,expo);
		spec *= fresnel;
	}
	Diffuse = light.Diffuse * mat.Diffuse * diff;
	Specular = light.Specular * mat.Specular * spec * roughness;
	return light.Ambient + Diffuse + Specular;

}

vec4 CalcSpotLight(in SurfaceMaterial mat, in SpotLight light, in vec3 normal, in vec3 posW, in vec3 eyePos, in float roughness)
{
	vec4 Diffuse;
	vec4 Specular;
	vec3 lightDir = light.Position - posW;
	float dist = length(lightDir);
	lightDir  = normalize(lightDir);
	float att = 1 / (light.Att.x + light.Att.y * dist + light.Att.z * dist * dist); 
	float spotCos = dot(lightDir,normalize(light.Direction));
	if(spotCos < light.Cutoff){
		return vec4(0,0,0,0);
	}else{
		att *= pow(spotCos,light.SpotExp);
	}
	float diff = 0;
	float spec = 0;
	//half lambert diffuse shading
	diff = dot(normal, -lightDir) * 0.5 + 0.5;
	//normal phong shading
	//diff = max(0,dot(normal,-lightDir));
	if(dot(normal, -lightDir) > 0){
		vec3 reflected = reflect(lightDir,normal);
		vec3 toEye = normalize(posW - eyePos);
		spec = max(0.0,dot(toEye,normalize(reflected)));
		spec = pow(spec,mat.SpecularPower); // to the power of shiny!
	}
	Diffuse = light.Diffuse * mat.Diffuse * diff * att;
	Specular = light.Specular * mat.Specular * spec * roughness * att;
	return (light.Ambient + Diffuse + Specular) ;
}
//main
void main()
{
	ivec2 texpos = ivec2(texcoord.x * ScreenWidth, texcoord.y * ScreenHeight);
	//recreate world position //thanks Johan!
	float x = (texcoord.x * 2) -1;
	float y = (texcoord.y * 2) -1;
	float z = (texelFetch(DepthTex,texpos,0).x * 2) - 1;
	
	vec4 projectedPos = vec4(x,y,z,1.0f);
	vec4 pos = invViewProj * projectedPos;
	vec3 posW = pos.xyz / pos.w;
	//dont light pixels we dont see
	if(z >= 1)
	{
		FinalLightColor = vec4(0.0);
	}
	else
	{
		vec3 normal = texelFetch(NormalTex,texpos,0).xyz;
		uint matID = texelFetch(MaterialTex,texpos,0).x;
		vec4 reflectionColor = texelFetch(RoughnessTex,texpos,0);
		float Rougness = reflectionColor.w;

		SurfaceMaterial material = Materials[matID];
		vec4 lightColor = vec4(0.0f);
		int i;
		for(i = 0; i < NrOfPointlights; ++i){
			lightColor += CalcPointLight(material,PointLights[i],normal,posW,camPos,Rougness);
		}
		for(i = 0; i < NrOfDirlights; ++i){
			lightColor += CalcDirLight(material,DirLights[i],normal,posW,camPos,Rougness);
		}
		for(i = 0; i < NrOfSpotlights; ++i){
			lightColor += CalcSpotLight(material,SpotLights[i],normal,posW,camPos,Rougness);
		}
		lightColor = saturate(lightColor);
		FinalLightColor = saturate(lightColor * (texture(colorTex,texcoord) + vec4(reflectionColor.rgb * Rougness,0.0f)));
		//FinalLightColor = lightColor * texture(colorTex,texcoord);
		//FinalLightColor = lightColor;
		//vec3 gamma = vec3(1.0f / 2.2f);
		//FinalLightColor.rgb = pow(FinalLightColor.rgb, gamma);
	}
}
#end_shader 