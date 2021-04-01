#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in float light;

out vec2 TexCoord;
out float fog_depth;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
	fog_depth = -((u_View * u_Model) * vec4(aPos, 1.0)).z;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}