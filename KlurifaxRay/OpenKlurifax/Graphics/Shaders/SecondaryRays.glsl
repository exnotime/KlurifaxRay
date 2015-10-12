#new_shader compute
#version 430 core
#extension GL_ARB_bindless_texture : enable
layout (local_size_x = 8,local_size_y = 8) in;
#define SHADOW 1
#define DirLightShadowLength 15.0f
#define MaxNrOfPointlights 200
#define MaxNrOfDirlights 5
#define MaxNrOfMaterials 100
#define PI 3.14159265359f
#define EPS 0.000001f
#define saturate(x) clamp(x,0.0f,1.0f)
#define MaxSpheres 1000
#define MaxTriangles 1000
#define MaxDistance 10000

#define MaxNrOfModels 50
#define MaxNrIndices 100000
#define MaxNrVertices 50000
#define MaxMeshesPerModel 10

#define MaxNrOfTextures 200

#include Geometry.txt
#include CollisionTests.txt
#include Lighting.txt

layout(std140,binding = 0) uniform PointLightBuffer{
	PointLight PointLights[MaxNrOfPointlights];
};

layout(std140,binding = 4) uniform DirLightBuffer{
	DirLight DirLights[MaxNrOfDirlights];
};

layout(std140,binding = 1) uniform MaterialBuffer{
	SurfaceMaterial Materials[MaxNrOfMaterials];
};
layout (std430,binding = 2) buffer RayBuffer{
	Ray Rays[];
};

layout (std430, binding = 3) restrict readonly buffer PrimitiveBuffer{
	Sphere g_Spheres[MaxSpheres];
	Triangle g_Triangles[MaxTriangles];
};

layout (std430, binding = 5) restrict readonly buffer SamplerBuffer{
	uvec2 g_Textures[MaxNrOfTextures];
};

layout (std430, binding = 8) restrict readonly buffer ModelBuffer{
	Model g_Models[MaxNrOfModels];
	uint g_Indices[MaxNrIndices];
	Vertex g_Vertices[MaxNrVertices];
};

layout (rgba8,binding = 0) uniform image2D output_img;
uniform float width;
uniform samplerCube CubeTex;
uniform int materialIndex;
uniform int NrOfPointlights;
uniform int NrOfSpheres;
uniform int NrOfTriangles;
uniform int NrOfModels;
uniform int NrOfDirlights;
uniform vec3 camPos;
uniform int iteration;
uniform float brightness;

HitData Trace(in Ray r){
	HitData h;
	h.Depth = MaxDistance;
	h.Hit = false;
	HitData hTemp;
	int i = 0;
	for(; i < NrOfSpheres;++i){
		RaySphear(r,g_Spheres[i],hTemp);
		if(hTemp.Hit && hTemp.Depth < h.Depth){
			h = hTemp;
		}
	}
	vec2 uv;
	for(i = 0; i < NrOfTriangles; ++i){
		RayTriangle(r,g_Triangles[i],uv,hTemp);
		if(hTemp.Hit && hTemp.Depth < h.Depth){
			h = hTemp;
		}
	}
	//foreach model
		//foreach mesh
			//for(meshstart -> meshend)
				//trace()
				//check depth
			//endfor
		//endforeach
	//endforeach
	AABB aabb;
	bool hitModel = false;
	for(i = 0; i < NrOfModels;i++){
		aabb.Bounds[0] = g_Models[i].ModelMinBounds.xyz;
		aabb.Bounds[1] = g_Models[i].ModelMaxBounds.xyz;
		RayAABB(r,aabb,hTemp);
		//if hit model
		if(hTemp.Hit){
			for(int k = 0; k < g_Models[i].MaterialSizeM.y;k++){
				aabb.Bounds[0] = g_Models[i].MeshMinBounds[k].xyz;
				aabb.Bounds[1] = g_Models[i].MeshMaxBounds[k].xyz;
				RayAABB(r,aabb,hTemp);
				if(hTemp.Hit){
					ivec4 mesh = g_Models[i].Meshes[k];
					int meshIndexOffset = g_Models[i].IndexVertexSizeISizeV.x + mesh.y;

					for(int indice = meshIndexOffset; indice < meshIndexOffset + mesh.z; indice += 3){

						RayVertexTri(r,g_Vertices[g_Indices[indice]], g_Vertices[g_Indices[indice + 1]] , g_Vertices[g_Indices[indice + 2]],hTemp);

						if(hTemp.Hit && hTemp.Depth < h.Depth){
							h.Depth = hTemp.Depth;
							h.Normal = hTemp.Normal;
							h.Tangent = hTemp.Tangent;
							h.uv = hTemp.uv;
							h.DiffuseTex = g_Models[i].MeshTextures[k].x;
							h.NormalTex = g_Models[i].MeshTextures[k].y;
							hitModel = true;
						}

					} // end vertices
				}
			} //end mesh
		}
	} // end model
	if(hitModel){
		h.Color = texture(sampler2D(g_Textures[h.DiffuseTex - 1]),h.uv);
		if(h.NormalTex > 0){
			h.Normal = CalcBumpedNormal(h.Normal,h.Tangent,h.uv,sampler2D(g_Textures[h.NormalTex - 1]));
		}
	}
	return h;
}

void main(){
	ivec2 pos = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);
	int index = int(pos.x + pos.y * width);
	vec4 color = vec4(1,1,1,1);
	vec3 normal = vec3(0);
	//clear image first time
	if(iteration == 1){
		imageStore(output_img,pos.xy,color);
	}
	vec2 uvtri;
	Ray r = Rays[index];
	HitData h = Trace(r);
	color = h.Color;
	normal = h.Normal;

	vec3 posW = r.o.xyz + h.Depth * r.d.xyz;
	if(h.Depth < MaxDistance){
		vec4 LightColor = vec4(0);
		
		SurfaceMaterial mat = Materials[materialIndex];

#if SHADOW == 1
		Ray ShadowRay;
		ShadowRay.o = vec4(posW + (r.d.xyz * -0.01f),1);
		for(int i = 0; i < NrOfPointlights; ++i){
			vec3 srayDir =  PointLights[i].Position - ShadowRay.o.xyz;
			ShadowRay.d = vec4(normalize(srayDir),1);
			float sRayDepth = Trace(ShadowRay).Depth;

			if(sRayDepth >= length(srayDir)){
				LightColor += CalcPointLight(mat,PointLights[i],normal,posW,camPos,mat.FresnelGlossMetalPower.y);
			} else {
				LightColor -= 0.1f;
			}
		}

		for(int i = 0; i < NrOfDirlights; ++i){
			LightColor += CalcDirLight(mat,DirLights[i],normal,posW,camPos,mat.FresnelGlossMetalPower.y);
			/*
			ShadowRay.d = -DirLights[i].Direction;
			float sRayDepth = Trace(ShadowRay).Depth;

			if(sRayDepth >= DirLightShadowLength){
				LightColor += CalcDirLight(mat,DirLights[i],normal,posW,camPos,mat.FresnelGlossMetalPower.y);
			} else {
				LightColor -= 0.1f;
			}
			*/
		}

#elif SHADOW == 0
		for(int i = 0; i < NrOfPointlights; ++i){
			LightColor += CalcPointLight(mat,PointLights[i],normal,posW,camPos,mat.FresnelGlossMetalPower.y);
		}
		for(int i = 0; i < NrOfDirlights; ++i){
			LightColor += CalcDirLight(mat,DirLights[i],normal,posW,camPos,mat.FresnelGlossMetalPower.y);
		}

#endif
		color *= vec4(mat.BaseColorDiffuse.xyz,1.0f);
		color = (texture(CubeTex,reflect(-r.d.xyz,normal)) * mat.FresnelGlossMetalPower.y + (1.0f - mat.FresnelGlossMetalPower.y) * color);
		Ray rout;
		rout.d = vec4(normalize(reflect(r.d.xyz,normal)),1);
		rout.o = vec4(posW + (rout.d.xyz * 0.001f),1);
		Rays[index] = rout;
		vec4 oldColor = imageLoad(output_img,pos.xy);
		LightColor = saturate(LightColor); 
		//float lerp = 1.0f / float(iteration * 2);
		//LightColor = ChangeBrightness(LightColor,brightness);
		//color = mix(oldColor,color,lerp) * LightColor;
		color = color * oldColor * LightColor;
	} else {
		Ray rout;
		rout.d = vec4(normalize(reflect(r.d.xyz,normal)),1);
		rout.o = vec4(posW + (rout.d.xyz * 0.001f),1);
		Rays[index] = rout;
		return;
	}
	imageStore(output_img,pos.xy,color);
}	

#end_shader