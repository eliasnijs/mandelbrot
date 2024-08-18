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

struct Program {
};


struct Mesh {
	B32 buffered;

	U32 vao;
	U32 vbo;
	U32 ebo;
};


internal Mesh
gpu_buffer_mesh(F32 *g, U32 *c, U32 v, U32 f)
{
	U32 vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, v * 5 * sizeof(F32), g, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, f * 3 * sizeof(U32), c,
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(F32),
			      (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(F32),
			      (void*)(3 * sizeof(F32)));
	glEnableVertexAttribArray(1);


	return { true, vao, vbo, ebo };
}

internal void
mesh_die(Mesh *mesh)
{
	if (mesh->buffered) {
		glDeleteVertexArrays(1, &mesh->vao);
		glDeleteBuffers(1, &mesh->vbo);
		glDeleteBuffers(1, &mesh->ebo);
	}
}


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


	F64 dt = 0.0;
	I32 mb_max_iterations = 100;
	while (!glfwWindowShouldClose(window)) {
		F64 start_time = glfwGetTime();

		get_input(window, &controller);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mb_sid);
		shader_set_I32(mb_sid, "max_iterations", mb_max_iterations);
		glBindVertexArray(plane.vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		imgui_start_frame();
		ImGui::SetNextWindowSize(ImVec2(500, 50));
		ImGui::Begin("Process Information", NULL, ImGuiWindowFlags_NoResize);
		ImGui::Text("Frame duration: %.3f ms | FPS: %.1f",
			    dt * 1000.0f, 1.0 / dt);
		ImGui::End();
		ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);
		ImGui::SliderInt("Max Iterations", &mb_max_iterations, 25, 200);
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

