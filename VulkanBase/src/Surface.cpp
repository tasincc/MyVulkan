#include "Surface.h"
#include "Log.h"

namespace MVK
{
	std::shared_ptr<Surface> Surface::Create(const std::shared_ptr<VulkanInstance> instance, GLFWwindow* window)
	{
		std::shared_ptr<Surface> surface = std::make_shared<Surface>();
		surface->mWindow = window;
		surface->mInstance = instance;
		VkResult result = glfwCreateWindowSurface(instance->GetHandle(), window, nullptr, &surface->mSurface);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create surface");
			exit(EXIT_FAILURE);
		}
		return surface;
	}

	Surface::~Surface()
	{
		if (mSurface != nullptr)
		{
			vkDestroySurfaceKHR(mInstance->GetHandle(), mSurface, nullptr);
		}
	}
}