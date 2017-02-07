#version 330 core
#define PI 3.14159265358979323846

layout( location = 0 ) out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 Color;
uniform float screenWidth;
uniform float time;

const float shininess = 50.0;



float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

void main () {

	vec3 normal   = normalize(Normal);							
	vec3 lightDir = normalize(vec3(0.0, 2.0, 0.0) - Position);
	vec3 viewDir  = normalize(-Position);
		
	float lightIntensity = 0.6/length(lightDir);
	lightDir = normalize(lightDir);

	vec3 white = vec3(1.0, 1.0, 1.0);

	
	FragColor = vec4(resultLight, 1.0);
}


