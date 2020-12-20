#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include "vulkan/vulkan.h"
#include "VulkanInstance.h"
#include <memory>
#include <vector>
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "Surface.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "DeviceObjectBase.h"
#include "VulkanQueue.h"
#include "GLFW/glfw3.h"

namespace MVK
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR sufaceCapabilities;
		std::vector<VkPresentModeKHR> presentModes;
		std::vector<VkSurfaceFormatKHR> surfaceFormats;
	};

	class VulkanSwapChain :public DeviceObjectBase
	{
	private:
		VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
		std::shared_ptr<VulkanDevice> mDevice;
		SwapChainSupportDetails mSwapchainSupport;
		VkFormat mImageFormat;
		VkExtent2D mExtent;
	public:
		~VulkanSwapChain();
		static std::shared_ptr<VulkanSwapChain> Create(GLFWwindow* window,const std::shared_ptr<VulkanPhysicalDevice>& physcialDevice,const std::shared_ptr<Surface>& surface,const std::shared_ptr<VulkanDevice>& device);
		VkSwapchainKHR GetHandle() const { return mSwapchain; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const  override { return mDevice; }
		VkFormat GetImageFormat() const { return mImageFormat; }
		const VkExtent2D GetExtent() const { return mExtent; }
		VkResult AcquireNextImage(uint32_t* frameIndex,const std::shared_ptr<VulkanSemaphore>& semaphore,const std::shared_ptr<VulkanFence>& fence);
		VkResult Present(uint32_t currentFrame,const std::shared_ptr<VulkanPresentQueue>& presentQueue ,const VulkanSemaphoreGroup& semaphoreGroup);
	private:
		VkSurfaceFormatKHR choseSwapChianFormat();
		SwapChainSupportDetails querySwapChainSupport(const std::shared_ptr<VulkanPhysicalDevice>& physcialDevice,const std::shared_ptr<Surface>& surface);
		VkExtent2D choseSwapChianExtent(GLFWwindow* window);
		VkPresentModeKHR choseSwapChianPresentMode();
	};
}

#endif
