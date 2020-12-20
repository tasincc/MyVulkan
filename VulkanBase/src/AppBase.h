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
#include "Camera.h"
#include "FrameRate.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"

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

	std::shared_ptr<MVK::VulkanPresentQueue> mPresentQueue{ nullptr };

	std::string m_AppName = "Hello";

	uint32_t m_ApiVersion = VK_MAKE_VERSION(1, 0, 0);

	bool m_Validation = true;

	VkPhysicalDeviceFeatures mEnabledFeatures{};

	std::vector<const char*> mEnableDeviceExtensions;

	std::vector<const char*> mEnabledInstanceExtensions;

	VkQueueFlags mRequestQueue = VK_QUEUE_GRAPHICS_BIT;

	uint32_t mImageIndex;

	uint32_t mCurrentFrame = 1;

	Camera mCamera;

	FrameRate mFrameRate;
	
	std::vector<std::shared_ptr<MVK::VulkanSemaphore>> mPresentSemaphores;
	std::vector<std::shared_ptr<MVK::VulkanSemaphore>> mRenderSemaphores;
	std::vector<std::shared_ptr<MVK::VulkanFence>> mInFlightFences;
	std::vector<std::shared_ptr<MVK::VulkanFence>> mImageInFlights;
public:
	AppBase();
	~AppBase();
	virtual void Initialize();
	void DrawFrame();
protected:
	virtual void commitCommandBuffer() = 0;
private:
	void renderBegin();
	void renderFinish();
};

#endif 