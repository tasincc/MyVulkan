#include "Surface.h"
#include "Log.h"

namespace MVK
{
	std::shared_ptr<Surface> Surface::crateSurface(const std::shared_ptr<VulkanInstance> instance, GLFWwindow* window)
	{
		std::shared_ptr<Surface> surface = std::make_shared<Surface>();
		surface->m_Window = window;
		surface->m_Instance = instance;
		VkResult result = glfwCreateWindowSurface(instance->GetHandle(), window, nullptr, &surface->m_Surface);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create surface");
			exit(EXIT_FAILURE);
		}
		return surface;
	}

	Surface::~Surface()
	{
		if (m_Surface != nullptr)
		{
			vkDestroySurfaceKHR(m_Instance->GetHandle(), m_Surface, nullptr);
		}
	}
}