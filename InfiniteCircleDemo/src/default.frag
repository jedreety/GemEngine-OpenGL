#version 330 core

// Output color
out vec4 fragment_colour;

in vec3 Normals;   // Normal vector passed from vertex shader


void main(void) {

    // Normalize the normal vector
    vec3 norm = normalize(Normals);

    // Map the normal components from [-1, 1] to [0, 1]
    vec3 normalColor = (norm + 1.0) / 2.0;  // Normalize to RGB range

    // Set the fragment color with alpha = 1 (fully opaque)
    fragment_colour = vec4(normalColor, 1.0);
}
