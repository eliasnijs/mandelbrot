
struct ButtonState {
	B32 is_down;
	B32 was_down;
};

struct MouseState {
	F64 xpos;
	F64 ypos;
	F64 xpos_prev;
	F64 ypos_prev;
	F64 scroll;
	F64 scroll_prev;
};

struct Controller {
	MouseState  mouse;

	ButtonState move_left;
	ButtonState move_right;
	ButtonState move_up;
	ButtonState move_down;
	ButtonState move_forward;
	ButtonState move_backward;

	ButtonState pause;
};

internal B32
button_released(ButtonState *button)
{
	return !button->is_down && button->was_down;
}

internal B32
button_is_down(ButtonState *button) {
	return button->is_down;
}

internal B32
button_pressed(ButtonState *button)
{
	return button->is_down && !button->was_down;
}

internal B32
button_is_up(ButtonState *button)
{
	return !button->is_down;
}

internal void
update_button_state(ButtonState *button, B32 is_down)
{
	button->was_down = button->is_down;
	button->is_down  = is_down;
}

internal void
scroll_callback(GLFWwindow* window, F64 xoffset, F64 yoffset)
{
	Controller *c = (Controller*)glfwGetWindowUserPointer(window);
	c->mouse.scroll_prev = c->mouse.scroll;
	c->mouse.scroll = yoffset;
}

internal void
get_input(GLFWwindow* window, Controller *controller)
{
	update_button_state(&controller->move_left,	glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	update_button_state(&controller->move_right,	glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
	update_button_state(&controller->move_up,	glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
	update_button_state(&controller->move_down,	glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	update_button_state(&controller->move_forward,	glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	update_button_state(&controller->move_backward, glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	update_button_state(&controller->pause,		glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS);

	F64 new_xpos, new_ypos;
	glfwGetCursorPos(window, &new_xpos, &new_ypos);
	controller->mouse.xpos_prev = controller->mouse.xpos;
	controller->mouse.ypos_prev = controller->mouse.ypos;
	controller->mouse.xpos = new_xpos;
	controller->mouse.ypos = new_ypos;
}

internal B32
controller_link_glfw(GLFWwindow *window, Controller *controller)
{
	glfwSetWindowUserPointer(window, controller);
	glfwSetScrollCallback(window, scroll_callback);
	return true;
}

