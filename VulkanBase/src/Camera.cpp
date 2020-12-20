#include "Camera.h"
#include "Config.h"

void Camera::Initialize(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_moveSpeed = SPEED;
	m_mouseSensitivity = SENSITIVITY;
	m_zoom = ZOOM;
	m_cameraPos = position;
	m_cameraUp = up;
	m_yaw = yaw;
	m_pitch = pitch;
	updateCameraVec();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_up);
}

glm::mat4 Camera::GetPerspective()
{
	//param: fov aspect near far
	glm::mat4 perspective = glm::perspective(glm::radians(m_zoom), (float)WIDTH / (float)HEIGHT, 0.001f, 100.0f);
	perspective[1][1] *= -1.0f;
	return perspective;
}

void Camera::Control(GLFWwindow* window, float delatTime) {
	static glm::dvec2 last_mouse_pos;
	glm::dvec2 cur_pos;
	glfwGetCursorPos(window, &cur_pos.x, &cur_pos.y);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float velocity = m_moveSpeed * delatTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_cameraPos += velocity * m_cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_cameraPos -= velocity * m_cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_cameraPos -= velocity * m_right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_cameraPos += velocity * m_right;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_cameraPos += velocity * m_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_cameraPos -= velocity * m_up;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		float xoffset = (cur_pos.x - last_mouse_pos.x) * m_mouseSensitivity;
		float yoffset = (last_mouse_pos.y - cur_pos.y) * m_mouseSensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;
		m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
	}

	updateCameraVec();
	last_mouse_pos = cur_pos;
}

void Camera::updateCameraVec() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_cameraFront = glm::normalize(front);
	//m_cameraFront 
	m_right = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
	m_up = glm::normalize(glm::cross(m_right, m_cameraFront));
}
