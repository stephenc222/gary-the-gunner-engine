#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;


// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
  fragmentColor = vertexColor;

  // gl_Position.xyz = vertexPosition_modelspace;
  // gl_Position.w = 1.0;
  
}
