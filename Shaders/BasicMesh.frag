// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

uniform sampler2D depthMap;

void main()
{
	float depthValue = texture(depthMap, fragTexCoord).r;
	// Sample color from texture
    outColor = vec4(vec3(depthValue), 1.0) * texture(uTexture, fragTexCoord);
}
