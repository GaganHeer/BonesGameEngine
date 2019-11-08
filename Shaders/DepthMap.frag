#version 330 core
out vec4 outColor;
  
in vec2 fragTexCoord;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, fragTexCoord).r;
    outColor = vec4(vec3(depthValue), 1.0);
}  