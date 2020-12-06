#include "SwapChainImage.h"

namespace MVK
{
	std::vector<std::shared_ptr<SwapChainImage>> SwapChainImage::Create(const std::shared_ptr<VulkanSwapChain>& swapChain)
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
			ret[i]->mImage = swapChainImages[i];
			ret[i]->mSwapchain = swapChain;

			ret[i]->mFormat = swapChain->GetImageFormat();
			ret[i]->mExtent = { swapChain->GetExtent().width,swapChain->GetExtent().height };
			ret[i]->mType = VK_IMAGE_TYPE_2D;
			ret[i]->mMipLevel = 1;
			ret[i]->mArrayLayer = 1;
		}

		return ret;
	}
}