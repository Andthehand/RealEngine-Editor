#type vertex
#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in uint aTexIndex;

// Binding 0
layout (std140) uniform Camera {
    mat4 viewProjection;
};

vec2 TexCoordLookup[4] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),
    vec2(0.0, 1.0)
);

out vec4 vColor;
out vec2 vTexCoord;
out flat uint vTexIndex;

void main() {
    // Assign texture coordinates based on gl_VertexID (for quad: 0-3)
    vTexCoord = TexCoordLookup[gl_VertexID % 4];
    vTexIndex = aTexIndex;
    vColor = aColor;

    gl_Position = viewProjection * vec4(aPos, 1.0);
}

#type fragment
#version 420 core

in vec4 vColor;
in vec2 vTexCoord;
in flat uint vTexIndex;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main() {
    vec4 textureColor = vColor;
    textureColor *= texture(u_Textures[vTexIndex], vTexCoord);

    if(textureColor.a < 0.1)
        discard;

    FragColor = textureColor;
}
