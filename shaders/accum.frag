#version 460 core
in vec2 uv;
out vec4 fragColor;

uniform sampler2D currTex;
uniform sampler2D prevAccTex;
uniform int FrameCount; // 1-based

void main() {
    vec3 curr = texture(currTex, uv).rgb;
    if (FrameCount <= 1) {
        fragColor = vec4(curr, 1.0);
    } else {
        vec3 prev = texture(prevAccTex, uv).rgb;
        float f = float(FrameCount);
        vec3 newAvg = prev + (curr - prev) / f;
        fragColor = vec4(newAvg, 1.0);
    }
}
