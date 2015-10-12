#new_shader compute
#version 430 core
layout (local_size_x = 16,local_size_y = 16) in;
#define MaxNrOfPointlights 200
#define MaxNrOfMaterials 100
#define PI 3.14159265359f
#define EPS 0.000001f
#define saturate(x) clamp(x,0.0f,1.0f)

#include Geometry.txt
#include CollisionTests.txt
#include Lighting.txt

uniform PointLightBuffer{
	PointLight PointLights[MaxNrOfPointlights];
};
uniform MaterialBuffer{
	SurfaceMaterial Materials[MaxNrOfMaterials];
};

layout (rgba8,binding = 0) uniform writeonly image2D output_img;
uniform float width, height;
uniform vec3 camPos;
uniform vec3 camUp,camRight,camDir;
uniform samplerCube CubeTex;
uniform Sphere s;
uniform Triangle tri;
uniform int materialIndex;
uniform int NrOfPointlights;
uniform float gloss;

void main(){
	ivec2 pos = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);
	vec4 color = vec4(0.3,0.6,0.9,1);
	vec3 normal = vec3(0);

	vec2 uvtri;

	Ray r;
	r.o = camPos;
	r.d = normalize(pos.x * camRight + pos.y * (-camUp) + camDir);

	HitData h[2];
	RaySphear(r,s,h[0]);
	RayTriangle(r,tri,uvtri,h[1]);

	float minDepth = 10000.0f;
	bool hit = false;
	for(int i = 0; i < 2; ++i){
		if(h[i].Depth < minDepth){
			minDepth = h[i].Depth;
			color = h[i].Color;
			normal = h[i].Normal;
			hit = true;
		}
	}
	SurfaceMaterial mat = Materials[materialIndex];
	vec4 LightColor = vec4(0);
	if(minDepth < 10000.0f){
		vec3 posW = r.o + minDepth * r.d;
		for(int i = 0; i < NrOfPointlights; ++i){
			LightColor += CalcPointLight(mat,PointLights[i],normal,posW,camPos,mat.FresnelGlossMetalPower.y);
		}
		color = texture(CubeTex,reflect(-r.d,normal)) * mat.FresnelGlossMetalPower.y + (1.0f - mat.FresnelGlossMetalPower.y) * color;
		color *= vec4(mat.BaseColorDiffuse.xyz,1) * LightColor;
	}
	imageStore(output_img,pos.xy,color);

}	

#end_shader