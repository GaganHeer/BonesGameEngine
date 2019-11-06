// Request GLSL 3.3
#version 330
void main(){
    // Not really needed, OpenGL does it anyway
    gl_FragDepth = gl_FragCoord.z;
}
