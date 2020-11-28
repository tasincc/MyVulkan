#ifndef APPBASE_H
#define APPBASE_H

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <string>
#include <vector>
#include <memory>

#include "VulkanInstance.h"
#include "VulkanDebug.h"
#include "VulkanDevice.h"
#include "Surface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"
#include "SwapChainImage.h"
#include "ImageView.h"
#include "VulkanRenderPass.h"
#include "VulkanQueue.h"
#include "VulkanCommandPool.h"

class AppBase
{
public:
	GLFWwindow* m_Window{ nullptr };
	std::shared_ptr<MVK::VulkanInstance> m_Instance;
	std::shared_ptr<MVK::VulkanPhysicalDevice> m_VulkanPhysicalDevice;
	std::shared_ptr<MVK::VulkanDevice> m_VulkanDevice;
	std::shared_ptr<MVK::Surface> m_surface;
	std::shared_ptr<MVK::VulkanSwapChain> m_swapChain;
	std::vector<std::shared_ptr<MVK::SwapChainImage>> m_swapchain_images;
	std::vector<std::shared_ptr<MVK::ImageView>> m_swapchain_image_views;


	VkPhysicalDeviceFeatures m_EnabledFeatures{};
	std::vector<const char*> m_EnableDeviceExtensions;
	std::vector<const char*> m_EnabledInstanceExtensions;
public:
	std::string m_AppName = "Hello";
	uint32_t m_ApiVersion = VK_MAKE_VERSION(1, 0, 0);
	bool m_Validation = true;
public:
	AppBase();
	~AppBase();
	virtual void Initialize();
};

#endif 