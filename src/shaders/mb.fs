#version 330
precision highp float;

in  vec2 uv;
out vec4 fragColor;

#define ESCAPE_RADIUS 2
uniform int max_iterations;

void main() {
    vec2 c = uv * 4.0 - vec2(2.5, 2.0);
    vec2 z = vec2(0.0);
    float iter = 0.0;

    for (int i = 0; i < max_iterations; i++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;

        if (length(z) > ESCAPE_RADIUS) {
            break;
        }

        iter++;
    }

    if (iter == float(max_iterations)) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float t = iter / float(max_iterations);
        fragColor = vec4(t * t, t * t, t * t * t, 1.0);
    }
}
