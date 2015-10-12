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
    in vec2 texcoord;
    uniform sampler2D   InTex;
    out vec4 			FinalColor;


    void main()
    {
        FinalColor = texture2D(InTex,texcoord);
    }
#end_shader