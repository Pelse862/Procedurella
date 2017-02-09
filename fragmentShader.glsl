#version 330 core
#define PI 3.14159265358979323846

layout( location = 0 ) out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 colorGround;
uniform vec3 colorPeaks;

uniform float altitude;

const float shininess = 50.0;

in float elevation;



void main () {

	vec3 normal   = normalize(Normal);							
	vec3 lightDir = normalize(vec3(0.0, 2.0, 0.0) - Position);
	vec3 viewDir  = normalize(-Position);
	//vec3 color = Color;	
	vec3 pos = Position;
	float lightIntensity = 0.6/length(lightDir);
	lightDir = normalize(lightDir);

	vec3 white = vec3(1.0, 1.0, 1.0);

	//Diffuse part-----------
	//float diff = max(dot(lightDir, normal), 0.0);
	//vec3 diffuse = diff * Color * lightIntensity;

	vec3 C_g = colorGround;
	vec3 C_p = colorPeaks;

	//specular part-------------
	//vec3 H = normalize(lightDir + viewDir);
	//float NdH = max(dot(H, normal), 0.0);
	//float spec = pow(NdH, shininess);
	//vec3 specular = spec * white;
	// Ambient-------------
	//color = (abs(pos.x)+abs(pos.y)+abs(pos.))>0.25f ? color : vec3(0.f,0.f,1.f) ;

	 //loat middle = step(0.0f, 0.075f, elevation);
	 vec3 diffuse = elevation <= 0.00 ? C_g : C_p;


	
	FragColor = vec4(diffuse, 1.0);
}


