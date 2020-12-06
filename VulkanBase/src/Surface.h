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
		GLFWwindow* mWindow;
		std::shared_ptr<VulkanInstance> mInstance;
		VkSurfaceKHR mSurface{ nullptr };
	public:
		~Surface();
		static std::shared_ptr<Surface> Create(const std::shared_ptr<VulkanInstance> instance, GLFWwindow* window);
		VkSurfaceKHR GetHandle() const { return mSurface; }
	};
}
#endif