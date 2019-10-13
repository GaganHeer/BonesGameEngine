#version 330

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outColor;

uniform sampler2D tex;

struct DirectionalLight {
	vec3 direction;
	vec3 diffuseColour;
	vec3 specColour;
};

uniform vec3 cameraPos;

uniform float specPower;

uniform vec3 ambientLight;

uniform DirectionalLight dirLight;

void main() {
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(-dirLight.direction);
	vec3 V = normalize(cameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = ambientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0) {
		vec3 Diffuse = dirLight.diffuseColour * NdotL;
		vec3 Specular = dirLight.specColour * pow(max(0.0, dot(R, V)), specPower);
		Phong += Diffuse + Specular;
	}
	outColor = texture(tex, fragTexCoord) * vec4(Phong, 1.0f);
}