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
		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
		SwapChainSupportDetails m_SwapChainSupport;
		std::shared_ptr<VulkanDevice> m_Device;
		VkFormat m_ImageFormat;
		VkExtent2D m_Extent;
	public:
		~VulkanSwapChain();
		static std::shared_ptr<VulkanSwapChain> createSwapChain(GLFWwindow* window, std::shared_ptr<VulkanPhysicalDevice> physcialDevice, std::shared_ptr<Surface> surface, std::shared_ptr<VulkanDevice> device);
		SwapChainSupportDetails querySwapChainSupport(std::shared_ptr<VulkanPhysicalDevice> physcialDevice, std::shared_ptr<Surface> surface);
		VkSurfaceFormatKHR choseSwapChianFormat();
		VkExtent2D choseSwapChianExtent(GLFWwindow* window);
		VkPresentModeKHR choseSwapChianPresentMode();
		VkSwapchainKHR GetHandle() const { return m_SwapChain; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const  override { return m_Device; }
		VkFormat GetImageFormat() const { return m_ImageFormat; }
		const VkExtent2D GetExtent() const { return m_Extent; }
	};
}

#endif
