#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 v_Position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Position;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//vec4 texColor = texture(u_Texture, v_Position.xy * 4);
	//vec4 texColor = texture(u_Texture, vec2(1.0f, 0.0f));
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor + u_Color;
	color = vec4(1.0);
}
