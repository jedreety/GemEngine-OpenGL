#version 330 core

// Output color
out vec4 fragment_colour;

// Input from vertex shader
in vec2 TexCoord;  // Texture coordinates passed from vertex shader
in vec3 Normals;   // Normal vector passed from vertex shader

// Uniforms
uniform sampler2DArray texture_array;  // Texture array (optional)

void main(void) {
    // Sample the texture using texture coordinates
    vec4 textureColor = texture(texture_array, vec3(TexCoord, 1));

    // Normalize the normal vector
    vec3 norm = normalize(Normals);

    // Map the normal components from [-1, 1] to [0, 1]
    vec3 normalColor = (norm + 1.0) / 2.0;  // Normalize to RGB range

    // Optionally, you can blend the texture color with the normal color
    // In this case, we combine them by averaging the colors
    vec3 finalColor = mix(textureColor.rgb, normalColor, 0.3);  // 50% blend

    // Set the fragment color with alpha = 1 (fully opaque)
    fragment_colour = vec4(finalColor, 1.0);
}
