#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <complex.h>

#include <glad.h>
#include <glad.c>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <linmath.h>
#include <stb_image.h>

#define ENABLE_PRINT  1
#define ENABLE_ASSERT 1

#include "libc/libc.h"
#include "platform/platform.h"

#include "meshes.h"

global_variable const char* COLOR_MODES[] = {
	"Heatmap",
	"Rainbow",
	"Greyscale"
};

struct Program {
	GLFWwindow	*window;
	Controller	controller;

	Mesh	plane;
	U32	sid;

	I32	max_iterations;
	F32	zoom;
	vec2	location;

	I32	color_mode;

	vec2	click_location;
	B32	dragging;

	F32	dt;
};

global_variable Program P = {
	.window		= 0x0,
	.controller	= {},
	.plane		= {},
	.sid		= 0,
	.max_iterations = 100,
	.zoom		= 1.0f,
	.location	= {0.0f, 0.0f},
	.color_mode	= 0,
	.click_location = {0.0f, 0.0f},
	.dragging	= false,
};


internal void
frame()
{
	F64 start_time = glfwGetTime();

	get_input(P.window, &P.controller);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	I32 width_, height_;
	glfwGetFramebufferSize(P.window, &width_, &height_);
	F32 width = (F32)width_;
	F32 height = (F32)height_;

	if (button_pressed(&P.controller.mouse.left)) {
		P.click_location[0] = (2.0f * P.controller.mouse.xpos / width) - 1.0f;
		P.click_location[1] = 1.0f - (2.0f * P.controller.mouse.ypos / height);
		P.dragging = true;
	}
	if (button_is_down(&P.controller.mouse.left) && P.dragging) {
		vec2 current_location = {0.0f, 0.0f};
		current_location[0] = (2.0f * P.controller.mouse.xpos / width) - 1.0f;
		current_location[1] = 1.0f - (2.0f * P.controller.mouse.ypos / height);
		vec2 delta = {0.0f, 0.0f};
		delta[0] = current_location[0] - P.click_location[0];
		delta[1] = current_location[1] - P.click_location[1];
		P.location[0] -= delta[1] * 1/P.zoom;
		P.location[1] -= delta[0] * 1/P.zoom;
		P.click_location[0] = current_location[0];
		P.click_location[1] = current_location[1];
	}
	if (button_released(&P.controller.mouse.left)) {
		P.dragging = false;
	}

	if (P.controller.mouse.scroll != 0.0f) {
		P.zoom += P.controller.mouse.scroll * 0.05f * (1.0f + P.zoom);
		P.controller.mouse.scroll = 0.0f;
	}

	P.zoom = Clamp(P.zoom, 0.1f, 100.0f);


	glUseProgram(P.sid);
	shader_set_vec2(P.sid, "resolution", (vec2){(F32)width, (F32)height});
	shader_set_I32(P.sid, "max_iterations", P.max_iterations);
	shader_set_F32(P.sid, "zoom", P.zoom);
	shader_set_I32(P.sid, "color_mode", P.color_mode);
	shader_set_vec2(P.sid, "location", P.location);
	glBindVertexArray(P.plane.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	imgui_start_frame();
	ImGui::SetNextWindowSize(ImVec2(500, 100));
	ImGui::Begin("The Mandelbrot Set", NULL, ImGuiWindowFlags_NoResize);
	ImGui::Text("Frame duration: %.3f ms | FPS: %.1f",
		    P.dt * 1000.0f, 1.0 / P.dt);
	ImGui::Separator();
	ImGui::InputInt("Max Iterations", &P.max_iterations);
	P.color_mode = Clamp(0, P.color_mode, (I32)(ArrayCount(COLOR_MODES) - 1));
	P.max_iterations = Clamp(1, P.max_iterations, 10000);

	ImGui::Combo("Color Mode", &P.color_mode, COLOR_MODES,
		     IM_ARRAYSIZE(COLOR_MODES));


	ImGui::End();
	imgui_end_frame();

	glfwSwapBuffers(P.window);
	glfwPollEvents();
	P.dt = glfwGetTime() - start_time;
}

I32
main()
{
	P.window = window_init(1440, 840, "Hello World!");
	if (!P.window) {
		print_error("failed to initialize window");
		return 1;
	}
	imgui_initialize(P.window);
	controller_link_glfw(P.window, &P.controller);

	P.plane = gpu_buffer_mesh(plane_geometry, plane_connectivity, 4, 2);

	B32 loaded = load_glprogram("mb", &P.sid);
	if (!loaded) {
		print_error("failed to initialize shader P");
		return 1;
	}

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(frame, 0, true);
#else
	while (!glfwWindowShouldClose(window)) {
		frame();
	}
#endif

	mesh_die(&P.plane);
	glprogram_die(P.sid);

	imgui_die();
	window_die(P.window);

	return 0;
}

