#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;
out vec2 tc;
uniform sampler2D Texture0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	vec3 pos=position;
	tc=vec2(texCoord.x,1-texCoord.y);
	vec3 vcol=texture(Texture0,tc).rgb;
	float gray=0.2990*vcol.r+0.5870*vcol.g+0.1140*vcol.b;

	vcol.r*=20.0;
	//gl_Position=projection*view*model*vec4(pos,1.0f);
	//vec4(pos,1.0f)=inverse(projection*view*model)*gl_Position;
	gl_Position=vec4(pos,1.0f);

}