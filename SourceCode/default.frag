#version 330
// specify we are indeed using modern opengl

out vec4 fragment_colour; // output of our shader

void main(void) {
	fragment_colour = vec4(0.8f, 0.3f, 0.02f, 1.0f); // set the output colour based on the vertex position
}