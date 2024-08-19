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

I32
main()
{
	GLFWwindow *window	= 0x0;
	Controller controller	= {};


	window = window_init(1440, 840, "Hello World!");
	if (!window) {
		print_error("failed to initialize window");
		return 1;
	}

	imgui_initialize(window);
	controller_link_glfw(window, &controller);
	stbi_set_flip_vertically_on_load(true);




	Mesh plane = gpu_buffer_mesh(plane_geometry, plane_connectivity, 4, 2);

	U32 mb_sid = 0;
	B32 loaded = load_glprogram("mb", &mb_sid);
	if (!loaded) {
		print_error("failed to initialize shader program");
		return 1;
	}

	/* controls */
	I32 mb_max_iterations = 100;
	F32 mb_zoom = 1.0f;
	vec2 mb_location = {0.0f, 0.0f};


	F64 dt = 0.0;
	while (!glfwWindowShouldClose(window)) {
		F64 start_time = glfwGetTime();

		get_input(window, &controller);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUseProgram(mb_sid);
		shader_set_I32(mb_sid, "max_iterations", mb_max_iterations);
		shader_set_F32(mb_sid, "zoom", mb_zoom);
		shader_set_vec2(mb_sid, "location", mb_location);
		glBindVertexArray(plane.vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		imgui_start_frame();
		ImGui::SetNextWindowSize(ImVec2(500, 50));
		ImGui::Begin("Process Information", NULL, ImGuiWindowFlags_NoResize);
		ImGui::Text("Frame duration: %.3f ms | FPS: %.1f",
			    dt * 1000.0f, 1.0 / dt);
		ImGui::End();
		ImGui::Begin("Controls", 0x0, 0x0);
		ImGui::InputInt("Max Iterations", &mb_max_iterations);
		mb_max_iterations = Clamp(1, mb_max_iterations, 1000);
		ImGui::InputFloat("Zoom", &mb_zoom);
		ImGui::SliderFloat2("Location", mb_location, -2.0f, 2.0f);
		ImGui::End();
		imgui_end_frame();

		glfwSwapBuffers(window);
		glfwPollEvents();
		dt = glfwGetTime() - start_time;
	}

	mesh_die(&plane);

	imgui_die();
	window_die(window);

	return 0;
}

