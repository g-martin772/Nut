#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_VPM;
uniform mat4 u_MLM;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = u_VPM * u_MLM * vec4(a_Pos, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 v_TexCoord;

void main() {
	color = texture(u_Texture, v_TexCoord * 10.0) * u_Color;
}