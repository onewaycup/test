#version 460 core
layout (location = 0) in vec2 vertex_position;
layout (location = 2) in mat4 instanceMatrix;

uniform float x_pos;
uniform float y_pos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = vec4(vertex_position.x + x_pos, vertex_position.y + y_pos, 0.0, 1.0);
}



//void main()
//{
//    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
//    TexCoords = aTexCoords;
//}