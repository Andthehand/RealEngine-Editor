#type vertex
#version 430 core
layout (location = 0) in vec2 vertex; // <vec2 pos>

out VS_OUT {
    vec2 TexCoords;
	flat vec4 Color;
    flat int index;
} vs_out;

// Binding 0
layout (std140, binding = 0) uniform Camera {
    mat4 viewProjection;
};

struct GlyphData {
	vec2 Position;
	vec2 Size;

	vec2 UV[4];
};

layout (std430, binding = 1) buffer TextData {
		mat4 Transform;
		vec4 Color;
		GlyphData Glyphs[];
};

void main() {
	vec2 localPos = Glyphs[gl_InstanceID].Position + vertex * Glyphs[gl_InstanceID].Size;
	vec4 worldPos = Transform * vec4(localPos, 0.0, 1.0);
	gl_Position = viewProjection * worldPos;

    vs_out.index = gl_InstanceID;
	vs_out.Color = Color;
    vs_out.TexCoords = Glyphs[gl_InstanceID].UV[gl_VertexID];
}

#type fragment
#version 430 core
out vec4 o_Color;

in VS_OUT {
    vec2 TexCoords;
	flat vec4 Color;
    flat int index;
} fs_in;

layout (binding = 0) uniform sampler2D u_FontAtlas;

float screenPxRange() {
	const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(fs_in.TexCoords);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
	vec3 msd = texture(u_FontAtlas, fs_in.TexCoords).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
    o_Color = mix(bgColor, fs_in.Color, opacity);
	if (o_Color.a == 0.0)
		discard;

//    vec4 bgColor = vec4(fs_in.TexCoords, 0.0f, 1.0f);
//    o_Color = bgColor;
//	if (o_Color.a == 0.0)
//		discard;
}
