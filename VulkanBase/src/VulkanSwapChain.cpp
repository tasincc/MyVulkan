#include "VulkanSwapChain.h"
#include "Config.h"
#include <algorithm>
#include "Log.h"

namespace MVK
{
	SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(const std::shared_ptr<VulkanPhysicalDevice>& physcialDevice, const std::shared_ptr<Surface>& surface)
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
		for (const auto& availableFormat : mSwapchainSupport.surfaceFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}
		return mSwapchainSupport.surfaceFormats[0];
	}

	VkExtent2D VulkanSwapChain::choseSwapChianExtent(GLFWwindow* window)
	{
		if (mSwapchainSupport.sufaceCapabilities.currentExtent.width != UINT32_MAX)
		{
			return mSwapchainSupport.sufaceCapabilities.currentExtent;
		}
		else
		{
			VkExtent2D extent;
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			extent.height = std::max(mSwapchainSupport.sufaceCapabilities.minImageExtent.height, std::min(mSwapchainSupport.sufaceCapabilities.maxImageExtent.height, static_cast<uint32_t>(height)));
			extent.width = std::max(mSwapchainSupport.sufaceCapabilities.minImageExtent.width, std::min(mSwapchainSupport.sufaceCapabilities.maxImageExtent.width, static_cast<uint32_t>(width)));
			return extent;
		}
	}

	VkPresentModeKHR VulkanSwapChain::choseSwapChianPresentMode()
	{
		if (!Vsync)
		{
			for (const auto& availableMode : mSwapchainSupport.presentModes)
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

	std::shared_ptr<VulkanSwapChain> VulkanSwapChain::Create(GLFWwindow* window,
		const std::shared_ptr<VulkanPhysicalDevice>& physcialDevice,
		const std::shared_ptr<Surface>& surface,
		const std::shared_ptr<VulkanDevice>& device)
	{
		std::shared_ptr<VulkanSwapChain> ret = std::make_shared<VulkanSwapChain>();
		ret->mSwapchainSupport = ret->querySwapChainSupport(physcialDevice, surface);
		VkExtent2D extent = ret->choseSwapChianExtent(window);
		VkPresentModeKHR presentMode = ret->choseSwapChianPresentMode();
		VkSurfaceFormatKHR surfaceFormat = ret->choseSwapChianFormat();
		uint32_t imageCount = ret->mSwapchainSupport.sufaceCapabilities.minImageCount + 1;
		if (ret->mSwapchainSupport.sufaceCapabilities.maxImageCount > 0 && imageCount > ret->mSwapchainSupport.sufaceCapabilities.maxImageCount)
		{
			imageCount = ret->mSwapchainSupport.sufaceCapabilities.maxImageCount;
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

		/*  present queue no equal graphic queue
		std::vector<uint32_t> queueFamilyIndices = device->GetQueueFamilyIndicesArray();
		if ()
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
			createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}
		*/
		// default
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		createInfo.preTransform = ret->mSwapchainSupport.sufaceCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		VkResult result = vkCreateSwapchainKHR(device->GetHandle(), &createInfo, nullptr, &ret->mSwapchain);
		if (result != VK_SUCCESS)
		{
			exit(EXIT_FAILURE);
		}
		ret->mDevice = device;
		ret->mImageFormat = surfaceFormat.format;
		ret->mExtent = extent;
		return ret;
	}

	VulkanSwapChain::~VulkanSwapChain()
	{
		vkDestroySwapchainKHR(mDevice->GetHandle(), mSwapchain, nullptr);
	}

	VkResult VulkanSwapChain::AcquireNextImage(uint32_t* frameIndex, const std::shared_ptr<VulkanSemaphore>& semaphore, const std::shared_ptr<VulkanFence>& fence)
	{
		return vkAcquireNextImageKHR(GetDevicePtr()->GetHandle(), mSwapchain, UINT32_MAX, semaphore ? semaphore->GetHandle() : VK_NULL_HANDLE, fence ? fence->GetHandle() : VK_NULL_HANDLE, frameIndex);
	}

	VkResult VulkanSwapChain::Present(uint32_t currentFrame, const std::shared_ptr<MVK::VulkanPresentQueue>& presentQueue, const VulkanSemaphoreGroup& semaphoreGroup)
	{
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pImageIndices = &currentFrame;
		presentInfo.pSwapchains = &mSwapchain;
		presentInfo.swapchainCount = 1;
		presentInfo.waitSemaphoreCount = semaphoreGroup.GetCount();
		presentInfo.pWaitSemaphores = semaphoreGroup.GetSemaphorePtr();

		return vkQueuePresentKHR(presentQueue->GetHandle(), &presentInfo);
	}
}