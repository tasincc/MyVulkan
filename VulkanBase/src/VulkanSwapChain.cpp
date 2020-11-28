#include "VulkanSwapChain.h"
#include "Config.h"
#include <algorithm>

namespace MVK
{
	SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(std::shared_ptr<VulkanPhysicalDevice> physcialDevice, std::shared_ptr<Surface> surface)
	{
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physcialDevice->GetHandle(), surface->GetHandle(), &details.sufaceCapabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physcialDevice->GetHandle(), surface->GetHandle(), &formatCount, nullptr);
		details.surfaceFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physcialDevice->GetHandle(), surface->GetHandle(), &formatCount, details.surfaceFormats.data());

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physcialDevice->GetHandle(), surface->GetHandle(), &presentModeCount, nullptr);
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physcialDevice->GetHandle(), surface->GetHandle(), &presentModeCount, details.presentModes.data());

		return details;
	}

	VkSurfaceFormatKHR VulkanSwapChain::choseSwapChianFormat()
	{
		for (const auto& availableFormat : m_SwapChainSupport.surfaceFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}
		return m_SwapChainSupport.surfaceFormats[0];
	}

	VkExtent2D VulkanSwapChain::choseSwapChianExtent(GLFWwindow* window)
	{
		if (m_SwapChainSupport.sufaceCapabilities.currentExtent.width != UINT32_MAX)
		{
			return m_SwapChainSupport.sufaceCapabilities.currentExtent;
		}
		else
		{
			VkExtent2D extent;
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			extent.height = std::max(m_SwapChainSupport.sufaceCapabilities.minImageExtent.height, std::min(m_SwapChainSupport.sufaceCapabilities.maxImageExtent.height, static_cast<uint32_t>(height)));
			extent.width = std::max(m_SwapChainSupport.sufaceCapabilities.minImageExtent.width, std::min(m_SwapChainSupport.sufaceCapabilities.maxImageExtent.width, static_cast<uint32_t>(width)));
			return extent;
		}
	}

	VkPresentModeKHR VulkanSwapChain::choseSwapChianPresentMode()
	{
		if (!Vsync)
		{
			for (const auto& availableMode : m_SwapChainSupport.presentModes)
			{
				if (availableMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return  VK_PRESENT_MODE_MAILBOX_KHR;
				}
			}
		}
		else
		{
			return VK_PRESENT_MODE_FIFO_KHR;
		}
	}

	std::shared_ptr<VulkanSwapChain> VulkanSwapChain::createSwapChain(GLFWwindow* window, std::shared_ptr<VulkanPhysicalDevice> physcialDevice, std::shared_ptr<Surface> surface, std::shared_ptr<VulkanDevice> device)
	{
		std::shared_ptr<VulkanSwapChain> ret = std::make_shared<VulkanSwapChain>();
		ret->m_SwapChainSupport = ret->querySwapChainSupport(physcialDevice, surface);
		VkExtent2D extent = ret->choseSwapChianExtent(window);
		VkPresentModeKHR presentMode = ret->choseSwapChianPresentMode();
		VkSurfaceFormatKHR surfaceFormat = ret->choseSwapChianFormat();
		uint32_t imageCount = ret->m_SwapChainSupport.sufaceCapabilities.minImageCount + 1;
		if (ret->m_SwapChainSupport.sufaceCapabilities.maxImageCount > 0 && imageCount > ret->m_SwapChainSupport.sufaceCapabilities.maxImageCount)
		{
			imageCount = ret->m_SwapChainSupport.sufaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface->GetHandle();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		std::vector<uint32_t> queueFamilyIndices = device->GetQueueIndex();
		if (device->GetGraphicQueue() != device->GetGraphicQueue())
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
			createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = ret->m_SwapChainSupport.sufaceCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		VkResult result = vkCreateSwapchainKHR(device->GetHandle(), &createInfo, nullptr, &ret->m_SwapChain);
		if (result != VK_SUCCESS)
		{
			exit(EXIT_FAILURE);
		}
		ret->m_Device = device;
		ret->m_ImageFormat = surfaceFormat.format;
		ret->m_Extent = extent;
		return ret;
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDestroySwapchainKHR(m_Device->GetHandle(), m_SwapChain, nullptr);
	}
}