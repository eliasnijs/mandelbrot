
internal void
simple_error_callback(I32 error, const char* description) {
	fprintf(stderr, "GLFW error: %s\n", description);
}

internal void
framebuffer_size_callback(GLFWwindow* window, I32 width, I32 height) {
	glViewport(0, 0, width, height);
}

EM_BOOL
resize_callback(int eventType, const EmscriptenUiEvent *e, void *win_) {
	GLFWwindow *win = (GLFWwindow*)win_;
	F64 width, height;
	emscripten_get_element_css_size("#canvas", &width, &height);
	glfwSetWindowSize(win, width, height);
	return EM_TRUE;
}

internal GLFWwindow *
window_init(I32 width, I32 height, const char* title) {
	glfwSetErrorCallback(simple_error_callback);
	if (!glfwInit()) {
		print_error("Failed to initialize GLFW");
		return 0x0;
	}

	/* glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); */
	/* glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); */

#ifdef __EMSCRIPTEN__
	F64 width_, height_;
	emscripten_get_element_css_size("#canvas", &width_, &height_);
	width = (I32)width_;
	height = (I32)height_;
#endif

	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		print_error("Failed to create window");
		glfwTerminate();
		return 0x0;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window,
				       EM_FALSE, resize_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		print_error("Failed to initialize GLAD");
		glfwTerminate();
		return 0x0;
	}

	return window;
}

internal void
window_die(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

