#version 330

layout(location = 0) in vec3 vertex_position; // vertex position attribute

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main(void) {

	gl_Position = projectionMatrix * viewMatrix * vec4(vertex_position, 1.0); // set vertex position
}