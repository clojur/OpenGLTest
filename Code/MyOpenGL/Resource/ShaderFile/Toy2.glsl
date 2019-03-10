#version 330 core
uniform int viewX;
uniform int viewY;
uniform int viewW;
uniform int viewH;
uniform float iTime;
uniform int mouseX;
uniform int mouseY;

vec2 iResolution=vec2(viewW,viewH);
vec3 iMouse=vec3(mouseX,mouseY,1.0);
#define resolution (iResolution.xy)
#define t (iTime * .3)

// Simplest Lambert+Phong material shading model parameters
struct mat_t {
	vec3 diffuse;
	vec4 specular;
};

// There are two separate distance fields, each consists of 6 metaballs and a plane.
#define N 6
vec3 b1[N];
vec3 b2[N];

// Four lights with their positions and colors
vec3 l1pos = 1.*vec3(6., 3., -7.);
vec3 l2pos = 1.*vec3(-7., 1., 6.);
vec3 l3pos = 1.*vec3(7., -4., 7.);
vec3 l4pos = vec3(0., 5., 25.);
vec3 l1color = 40.*vec3(1., .7, .7);
vec3 l2color = 20.*vec3(.7, .7, 1.);
vec3 l3color = 60.*vec3(.7, 1., .7);
vec3 l4color = vec3(100.);

// Distance function approximation for the first field only
float t1(vec3 p) {
	float v = 0.;
	for (int i = 0; i < N; ++i) {
		vec3 b = p - b1[i];
		// metaball field used here is a simple sum of inverse-square distances to metaballs centers
		// all numeric constants are empirically tuned
		v += 5. / dot(b, b);
	}
	// add top y=12 (red) plane
	float d = 12. - p.y; v += 3. / (d*d);
	return v;
}

// Second field distance function is basically the same, but uses b2[] metaballs centers and y=-12 plane
float t2(vec3 p) {
	float v = 0.;
	for (int i = 0; i < N; ++i) {
		vec3 b = p - b2[i];
		v += 5. / dot(b, b);
	}
	float d = 12. + p.y; v += 3. / (d*d);
	return v;
}

// "Repulsive" distance functions which account for both fields
float w1(vec3 p) { return 1. - t1(p) + t2(p); }
float w2(vec3 p) { return 1. + t1(p) - t2(p); }

// Combined world function that picks whichever field is the closest one
float world(vec3 p) {
	return min(w1(p), w2(p));
}

vec3 normal(vec3 p) {
	vec2 e = vec2(.001, 0.);
	return normalize(vec3(
		world(p + e.xyy) - world(p - e.xyy),
		world(p + e.yxy) - world(p - e.yxy),
		world(p + e.yyx) - world(p - e.yyx)));
}

// Material is picked based on which field's distance function is the smallest
mat_t material(vec3 p) {
	mat_t m;
	m.diffuse = vec3(0.);
	m.specular = vec4(0.);
	if (w1(p) < w2(p)) {
		m.diffuse = vec3(.92, .027, .027);
		m.specular = vec4(mix(vec3(1.), m.diffuse, .7), 2000.);
	}
	else {
		m.diffuse = vec3(.14, .17, 1.57);
		m.specular = vec4(mix(vec3(1.), m.diffuse, .5), 100.);
	}
	return m;
}

float iter = 0.;

// Raymarcher
float trace(in vec3 O, in vec3 D, in float L, in float Lmax) {
	// The usual "step along the ray" loop
	float Lp = L;
	for (int i = 0; i < 40; ++i) {
		iter = float(i);
		float d = world(O + D * L);
		if (d < .01*L || L > Lmax) break;
		// Store previous point (see below)
		Lp = L;

		// Overstep a bit to find intersetion faster (metaball fields aren't cheap)
		L += d * 1.5;
	}

	if (L < Lmax) {
		// Binary search for more exact intersestion position
		// Needed to fix artifacts due to overstep
		for (int i = 0; i < 5; ++i) {
			float Lm = (Lp + L) * .5;
			if (world(O + D * Lm) < .01*Lm) L = Lm; else Lp = Lm;
		}
	}
	iter /= 32.;
	return L;
}

// Simpler tracing function that tries to determine whether the point is in shadow
float shadowtrace(vec3 p, vec3 d, float lmax) {
	float v = .02;
	// Take a few samples between the point and the light position and chech
	// whether they are "inside" geometry
	for (int i = 1; i < 9; ++i) {
		v = min(v, world(p + d*lmax*float(i) / 12.));
	}
	return smoothstep(.0, .02, v);

}

// Compute shading color from a single light
vec3 enlight(vec3 p, vec3 v, vec3 n, mat_t m, vec3 lpos, vec3 lcolor) {
	vec3 ldir = lpos - p; // direction from the point to the light source
	float ldist2 = dot(ldir, ldir); // square distance to the light source
	float ldist = sqrt(ldist2); // distance
	ldir /= ldist; // ldir is now normalized
	float shadow = shadowtrace(p, ldir, ldist - .2); // whether the point is in shadow
	return shadow * lcolor * (
		// Diffuse term (simple Lambert)
		m.diffuse * max(0., dot(n, ldir))
		// Specular term (Phong-ish)
		+ m.specular.rgb * pow(max(0., dot(normalize(ldir - v), n)), m.specular.w)
		// Specular energy conservation? (or something, it was 3 years ago :D)
		* (m.specular.w + 2.) * (m.specular.w + 4.) / (24. * (m.specular.w + pow(2., -m.specular.w / 2.)))
		) / ldist2;
}

// A ridiculous hack to display light sources
// Basic idea is to check whether the ray is "close enough" to the light source position in screen space
// Which is done by calculating the angle between the actual pixel ray and a vector from origin to the light source
vec3 lightball(vec3 lpos, vec3 lcolor, vec3 O, vec3 D, float L) {
	vec3 ldir = lpos - O;
	float ldist = length(ldir);
	if (ldist > L) return vec3(0.);
	float pw = pow(max(0., dot(normalize(ldir), D)), 20000.);
	return (normalize(lcolor) + vec3(1.)) * pw;
}

void main() {
	// Calculate metaball trajectories (all empirical)
	for (int i = 0; i < N; ++i) {
		float fi = float(i)*.7;
		b1[i] = vec3(3.7*sin(t + fi), 1. + 10.*cos(t*1.1 + fi), 2.3*sin(t*2.3 + fi));
		fi = float(i)*1.2;
		b2[i] = vec3(4.4*cos(t*.4 + fi), -1. - 10.*cos(t*0.7 + fi), -2.1*sin(t*1.3 + fi));
	}

	vec2 uv = gl_FragCoord.xy / resolution.xy * 2. - 1.;
	uv.x *= resolution.x / resolution.y;

	vec3 O = vec3(0., 0., 20.);
	vec3 D = normalize(vec3(uv, -1.6));

	if (iMouse.z > 0.)
	{
		vec2 m = iMouse.xy / iResolution.xy*2. - 1.;
		float a = -m.x * 2. * 3.1415926;
		float s = sin(a), c = cos(a);
		O = vec3(s*20., -m.y*10., c*20.);
		vec3 fw = normalize(vec3(0.) - O);
		vec3 rg = cross(fw, vec3(0., 1., 0.));
		D = normalize(mat3(rg, cross(rg, fw), -fw) * D);
	}

	// Raymarch
	float L = trace(O, D, 0., 40.);
	vec3 color = vec3(0.);
	if (L < 40.) {
		// If a hit is detected, then get the position
		vec3 p = O + D * L;
		// And normal
		vec3 n = normal(p);
		// And material
		mat_t m = material(p);

		// Ambient term (I don't think it is visible much)
		color = .001 * m.diffuse * n;

		// Shade for all lights
		color += enlight(p, D, n, m, l1pos, l1color);
		color += enlight(p, D, n, m, l2pos, l2color);
		color += enlight(p, D, n, m, l3pos, l3color);
		color += enlight(p, D, n, m, l4pos, l4color);

		// Fog-like hack-ish attenuation, so the only the center of the scene is visible
		color *= (1. - smoothstep(10., 20., length(p)));
	}
	else L = 100.;

	// Add light sources hack
	color += lightball(l1pos, l1color, O, D, L);
	color += lightball(l2pos, l2color, O, D, L);
	color += lightball(l3pos, l3color, O, D, L);
	color += lightball(l4pos, l4color, O, D, L);

	// Gamma correct and out
	gl_FragColor = vec4(pow(color, vec3(.7)), 1.0);
}