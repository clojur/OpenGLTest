#version 330 core

uniform sampler2D Texture0;
uniform int viewX;
uniform int viewY;
uniform int viewW;
uniform int viewH;
uniform float iTime;
out vec4 color;
in vec2 tc;
in vec3 pos;
float mySphere(in vec3 ro,in vec3 rd,in vec4 sph)
{

	float r=sph.w*sph.w;//°ë¾¶
	vec3 oo=ro-sph.xyz;//Æ«ÒÆ
	float a=dot(oo,oo);
	float b=dot(rd,rd);
	float c=dot(oo,rd);
	float p=(c*c-b*(a-r))/(b*b);
	if(p<0.0) return -1.0;
	return sqrt(p)-(c/b);
}
vec3 nsphere(in vec3 pos,in vec4 shpo)
{
	return (pos-shpo.xyz)/shpo.w;
}
vec3 nplane (in vec3 pos)
{
	return vec3(0.0,1.0,0.0);
}
float myPlane(in vec3 ro,in vec3 rd)
{
	return (-ro.y-1.0)/rd.y;
}
float intersect(in vec3 ro,in vec3 rd,out float resT,in vec4 spho)
{
	resT=1000.0f;
	float id=-1.0;
	float tp=myPlane(ro,rd);
	float ts=mySphere(ro,rd,spho);
	if(ts>0.0)
	{
		resT=ts;
		id=1.0;
	}
	if(tp>0.0&&tp<resT)
	{
		resT=tp;
		id=2.0;
	}
	return id;
}
void main()
{
	vec3 lightpos=vec3(0.57608);
	color=vec4(vec3(1.0),1.0f);
	float ar = viewW / viewH;
	vec2 uv=vec2(gl_FragCoord.x/viewW/ar,gl_FragCoord.y/viewH)*2.0-1.0;//uv[-1.0,1.0]
	vec3 ro = vec3(0.0,0.0,3.0);
	float nw = viewH / viewW;
	//uv.x *= nw;
	vec3 rd=normalize(vec3( uv,-1.0));
	float resT;
	vec4 spho = vec4(0.5*sin(iTime), 0.0, 0.5*cos(iTime), 1.0);
	float id=intersect(ro,rd,resT,spho);
	if(id>0.5&&id<1.5)
	{
		vec3 pos=ro+resT*rd;
		vec3 nor=nsphere(pos,spho);
		float amb=0.5+0.5*nor.y;
		float diff=clamp(dot(nor,lightpos),0.0,1.0);
		color=vec4(vec3(0.0,0.8,0.6)*diff*amb+amb*vec3(0.4,0.6,0.4),1.0);
	}
	else if(id>1.5)
	{
		vec3 pos=ro+resT*rd;
		vec3 nor=nplane(pos);
		float diff=clamp(dot(nor,lightpos),0.0,1.0);
		float amb=smoothstep(0.0,2.0+spho.w,length(pos.xz-spho.xz));
		color=vec4(vec3(amb+0.5),1.0);
	}

}