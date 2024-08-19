#version 330
precision highp float;

in vec2 uv;
out vec4 fragColor;

#define ESCAPE_RADIUS 10
uniform int max_iterations;

uniform float zoom;
uniform vec2 location;
uniform vec2 resolution;
uniform int color_mode;

vec3 heatmapColor(float t) {
    vec3 color = vec3(0.0);
    if (t < 0.25) {
        // Blue to cyan
        color = mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0), t / 0.25);
    } else if (t < 0.5) {
        // Cyan to green
        color = mix(vec3(0.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), (t - 0.25) / 0.25);
    } else if (t < 0.75) {
        // Green to yellow
        color = mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0), (t - 0.5) / 0.25);
    } else if (t < 0.875) {
	// Yellow to red
	color = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), (t - 0.75) / 0.25);
    } else {
	// Red to black
	color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), (t - 0.875) / 0.125);
    }
    return color;
}

vec3 grayscaleColor(float t) {
    return vec3(t);
}

vec3 rainbowColor(float t) {
    vec3 color = vec3(0.0);
    if (t < 1.0 / 7.0) {
        // Red to orange
        color = mix(vec3(1.0, 0.0, 0.0), vec3(1.0, 0.5, 0.0), t * 7.0);
    } else if (t < 2.0 / 7.0) {
        // Orange to yellow
        color = mix(vec3(1.0, 0.5, 0.0), vec3(1.0, 1.0, 0.0), (t - 1.0 / 7.0) * 7.0);
    } else if (t < 3.0 / 7.0) {
        // Yellow to green
        color = mix(vec3(1.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), (t - 2.0 / 7.0) * 7.0);
    } else if (t < 4.0 / 7.0) {
        // Green to cyan
        color = mix(vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0), (t - 3.0 / 7.0) * 7.0);
    } else if (t < 5.0 / 7.0) {
        // Cyan to blue
        color = mix(vec3(0.0, 1.0, 1.0), vec3(0.0, 0.0, 1.0), (t - 4.0 / 7.0) * 7.0);
    } else if (t < 6.0 / 7.0) {
        // Blue to violet
        color = mix(vec3(0.0, 0.0, 1.0), vec3(0.5, 0.0, 1.0), (t - 5.0 / 7.0) * 7.0);
    } else {
        // Violet to red
        color = mix(vec3(0.5, 0.0, 1.0), vec3(1.0, 0.0, 0.0), (t - 6.0 / 7.0) * 7.0);
    }
    return color;
}



void main() {
    vec2 uv = uv;

    vec2 aspect = vec2(1.0, resolution.x / resolution.y);
    vec2 c = ((uv / zoom) * 4.0) - vec2(2.5 / zoom, 2.0 / zoom)  - location;
    c *= aspect;

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
	switch (color_mode) {
	    case 0:
		fragColor = vec4(heatmapColor(t), 1.0);
		break;
	    case 1:
		fragColor = vec4(rainbowColor(t), 1.0);
		break;
	    case 2:
		fragColor = vec4(grayscaleColor(t), 1.0);
		break;
	}
    }
}

