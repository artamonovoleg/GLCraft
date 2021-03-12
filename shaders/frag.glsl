#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 camPos;

// texture samplers
uniform sampler2D texture1;

in float fog_depth;

const vec4 fog_color = vec4(0.3961, 0.3882, 0.4, 1.0);
const float fog_near = 30.0;
const float fog_far = 50.0;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);

	if (texColor.a < 0.1)
		discard;

	float fog =  smoothstep(fog_near, fog_far, fog_depth);
	FragColor = mix(texColor, fog_color, fog);
}