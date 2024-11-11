#version 330
// specify we are indeed using modern opengl

out vec4 fragment_colour; // output of our shader

in vec2 TexCoord; // Receive texture coordinates from vertex shader
uniform sampler2DArray texture_array; // texture array

void main(void) {
	fragment_colour = texture(texture_array, vec3(TexCoord, 1));
}