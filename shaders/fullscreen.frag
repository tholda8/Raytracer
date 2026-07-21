#version 460 core
in vec2 uv;
out vec4 fragColor;

uniform sampler2D resultImage;
uniform int ApplyGamma; // 0 = off, 1 = on
uniform float Gamma; // e.g. 2.2

void main() {
    vec4 col = texture(resultImage, uv);
    if (ApplyGamma == 1) {
        // assume texture contains linear HDR/color values; apply gamma correction for display
        vec3 g = pow(max(vec3(0.0), col.rgb), vec3(1.0 / Gamma));
        fragColor = vec4(g, col.a);
    } else {
        fragColor = col;
    }
}
