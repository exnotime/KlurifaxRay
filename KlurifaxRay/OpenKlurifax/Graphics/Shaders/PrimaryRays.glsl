#new_shader compute
#version 430 core
layout (local_size_x = 16,local_size_y = 16) in;

#include Geometry.txt

layout (std430,binding = 2) buffer RayBuffer{
	Ray Rays[];
};
uniform float width;
uniform vec3 camPos;
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 camDir;

void main(){
	ivec2 pos = ivec2(gl_GlobalInvocationID.x,gl_GlobalInvocationID.y);
	int index = int(pos.x + width * pos.y); 
	Ray r;
	r.o.xyz = camPos;
	r.d.xyz = normalize(pos.x * camRight + pos.y * (-camUp) + camDir);
	Rays[index] = r;
}	

#end_shader