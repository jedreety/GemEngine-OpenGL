#version 330
// specify we are indeed using modern opengl

layout(location = 0) in vec3 vertex_position; // vertex position attribute
layout(location = 1) in vec2 aTexCoord;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 TexCoord; // Pass texture coordinates to fragment shader

void main(void) {
	TexCoord = aTexCoord;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex_position, 1.0); // set vertex position
}