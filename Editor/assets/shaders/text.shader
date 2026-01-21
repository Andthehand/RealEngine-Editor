#type vertex
#version 430 core
layout (location = 0) in vec2 vertex; // <vec2 pos>

out VS_OUT {
    vec2 TexCoords;
    flat int index;
} vs_out;

// Binding 0
layout (std140, binding = 0) uniform Camera {
    mat4 viewProjection;
};

struct GlyphData {
    mat4 transform;
    int letter;
};

layout (std430, binding = 1) buffer TextData {
    vec4 textColor;
    GlyphData glyphs[];
};

void main() {
    gl_Position = viewProjection * glyphs[gl_InstanceID].transform * vec4(vertex.xy, 0.0, 1.0);
    vs_out.index = gl_InstanceID;
    vs_out.TexCoords = vertex.xy;
    vs_out.TexCoords.y= 1.0f - vs_out.TexCoords.y;
}

#type fragment
#version 430 core
out vec4 color;

in VS_OUT {
    vec2 TexCoords;
    flat int index;
} fs_in;

uniform sampler2DArray text;

struct GlyphData {
    mat4 transform;
    int letter;
};

layout (std430, binding = 1) buffer TextData {
    vec4 textColor;
    GlyphData glyphs[];
};

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vec3(fs_in.TexCoords.xy, glyphs[fs_in.index].letter)).r);
    
    if(sampled.a < 0.1)
        discard;

    color = textColor * sampled;
    
    // color = textColor;
}
