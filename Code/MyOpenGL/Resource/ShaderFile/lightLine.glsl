#version 330 core
uniform int viewX;
uniform int viewY;
uniform int viewW;
uniform int viewH;
uniform float iTime;
uniform int mouseX;
uniform int mouseY;
vec3 COLOR1 = vec3(0.0, 0.0, 0.3);
vec3 COLOR2 = vec3(0.5, 0.0, 0.0);
float BLOCK_WIDTH = 0.01;
out vec4 color;

void draw_line()
{
}
void main()
{
	vec2 uv =vec2(gl_FragCoord.x/viewW,gl_FragCoord.y/viewH);
	// To create the BG pattern
	vec3 final_color = vec3(1.0);
	vec3 bg_color = vec3(0.0);
	vec3 wave_color = vec3(0.0);
	
	float c1 = mod(uv.x, 2.0 * BLOCK_WIDTH);
	c1 = step(BLOCK_WIDTH, c1);
	
	float c2 = mod(uv.y, 2.0 * BLOCK_WIDTH);
	c2 = step(BLOCK_WIDTH, c2);
	
	bg_color = mix(uv.x * COLOR1, uv.y * COLOR2, c1 * c2);
	
	
	// To create the waves
	float wave_width = 0.01;
	uv  = -1.0 + 2.0 * uv;
	uv.y += 0.0;
	//for(float i = 0.0; i < 10.0; i++) 
	//{
		//uv.y += (0.07 * sin(uv.x + i/7.0 + iTime ));
		if(uv.y<=0.0)
			uv.y+=sqrt(0.64-(uv.x*uv.x));
		else
			uv.y-=sqrt(0.64-(uv.x*uv.x));

		wave_width = abs(1/(uv.y*650));
		wave_color += vec3(wave_width * 1.9, wave_width, wave_width * 1.5);
	//}
	
	//final_color = bg_color + wave_color;
	final_color=wave_color;
	
	color = vec4(final_color, 1.0);
}