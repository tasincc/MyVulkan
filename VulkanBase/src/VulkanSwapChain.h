#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include "vulkan/vulkan.h"
#include "VulkanInstance.h"
#include <memory>
#include <vector>
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "Surface.h"
#include "DeviceObjectBase.h"
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
		void Present();
	private:
		VkSurfaceFormatKHR choseSwapChianFormat();
		SwapChainSupportDetails querySwapChainSupport(const std::shared_ptr<VulkanPhysicalDevice>& physcialDevice,const std::shared_ptr<Surface>& surface);
		VkExtent2D choseSwapChianExtent(GLFWwindow* window);
		VkPresentModeKHR choseSwapChianPresentMode();
	};
}

#endif
