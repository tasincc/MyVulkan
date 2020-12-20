#ifndef CAMERA_H
#define CAMERA_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GLFW/glfw3.h>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec3 m_up;
	glm::vec3 m_right;
	float m_yaw;
	float m_pitch;
	float m_moveSpeed;
	float m_mouseSensitivity;
	float m_zoom;
public:
	Camera() = default;
	void Initialize(glm::vec3 position, glm::vec3 up, float yaw = YAW, float pitch = PITCH);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetPerspective();
	void Control(GLFWwindow* window, float delatTime);
private:
	void updateCameraVec();
};

#endif // !CAMERA_H
