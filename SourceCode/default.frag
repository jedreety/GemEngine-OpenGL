#version 330
// specify we are indeed using modern opengl

out vec4 fragment_colour; // output of our shader
in vec3 frag_pos; // input of our shader

void main(void) {
	fragment_colour = vec4(frag_pos * 0.75f + 0.5f, 1.0f); // set the output colour based on the vertex position
}