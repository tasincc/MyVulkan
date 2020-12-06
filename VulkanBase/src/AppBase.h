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
#include "VulkanImage.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"
#include "VulkanQueue.h"
#include "VulkanCommandPool.h"

class AppBase
{
protected:
	GLFWwindow* mWindow{ nullptr };
	std::shared_ptr<MVK::VulkanInstance> mInstance;
	std::shared_ptr<MVK::Surface> mSurface;

	std::shared_ptr<MVK::VulkanPhysicalDevice> mVulkanPhysicalDevice;
	std::shared_ptr<MVK::VulkanDevice> mVulkanDevice;
	std::shared_ptr<MVK::VulkanSwapChain> mSwapChain;

	std::vector<std::shared_ptr<MVK::SwapChainImage>> mSwapchainImages;
	std::vector<std::shared_ptr<MVK::VulkanImageView>> mSwapchainImageViews;
	std::shared_ptr<MVK::VulkanImage> mDepth;
	std::shared_ptr<MVK::VulkanImageView> mDepthView;
protected:
	std::string m_AppName = "Hello";
	uint32_t m_ApiVersion = VK_MAKE_VERSION(1, 0, 0);
	bool m_Validation = true;
	VkPhysicalDeviceFeatures mEnabledFeatures{};
	std::vector<const char*> mEnableDeviceExtensions;
	std::vector<const char*> mEnabledInstanceExtensions;
	VkQueueFlags mRequestQueue = VK_QUEUE_GRAPHICS_BIT;
public:
	AppBase();
	~AppBase();
	virtual void Initialize();
	void DrawFrame();
};

#endif 