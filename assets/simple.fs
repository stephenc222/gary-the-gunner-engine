// #version 330 core
// // out vec3 color;
// // Output data ; will be interpolated for each fragment.
// in vec3 fragmentColor;
// out vec3 color;

// void main(){
//   // color = vec3(1,0,0);
//   // Output color = color specified in the vertex shader,
//   // interpolated between all 3 surrounding vertices
//   color = fragmentColor;


// }

#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

    // Output color = color of the texture at the specified UV
    color = texture( myTextureSampler, UV ).rgb;
}