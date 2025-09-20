#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

// Binding 0
layout (std140) uniform Camera {
    mat4 viewProjection;
};

out vec4 Color;

void main() {
   Color = aColor;

   gl_Position = viewProjection * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

in vec4 Color;

out vec4 FragColor;

void main() {
   FragColor = Color;
}