#ifndef SURFACE_H
#define SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include "VulkanInstance.h"

namespace MVK
{
	class Surface
	{
	private:
		GLFWwindow* m_Window;
		std::shared_ptr<VulkanInstance> m_Instance;
		VkSurfaceKHR m_Surface{ nullptr };
	public:
		~Surface();
		static std::shared_ptr<Surface> crateSurface(const std::shared_ptr<VulkanInstance> instance, GLFWwindow* window);
		VkSurfaceKHR GetHandle() const { return m_Surface; }
	};
}
#endif