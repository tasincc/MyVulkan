#include "SwapChainImage.h"

namespace MVK
{
	std::vector<std::shared_ptr<SwapChainImage>> SwapChainImage::createSwapChainImage(const std::shared_ptr<VulkanSwapChain> swapChain)
	{
		std::vector<std::shared_ptr<SwapChainImage>> ret;

		uint32_t imageCount;
		vkGetSwapchainImagesKHR(swapChain->GetDevicePtr()->GetHandle(), swapChain->GetHandle(), &imageCount, nullptr);
		std::vector<VkImage> swapChainImages(imageCount);
		vkGetSwapchainImagesKHR(swapChain->GetDevicePtr()->GetHandle(), swapChain->GetHandle(), &imageCount, swapChainImages.data());

		ret.resize(imageCount);
		for (uint32_t i = 0; i < imageCount; i++)
		{
			ret[i] = std::make_shared<SwapChainImage>();
			ret[i]->m_Image = swapChainImages[i];
			ret[i]->m_swapchain = swapChain;

			ret[i]->m_format = swapChain->GetImageFormat();
			ret[i]->m_Extent = { swapChain->GetExtent().width,swapChain->GetExtent().height };
			ret[i]->m_Type = VK_IMAGE_TYPE_2D;
			ret[i]->m_mipLevel = 1;
			ret[i]->m_arrayLayer = 1;
		}

		return ret;
	}
}