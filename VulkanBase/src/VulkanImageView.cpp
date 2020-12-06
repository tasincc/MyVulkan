#include "VulkanImageView.h"
#include "Log.h"

namespace MVK
{
	std::shared_ptr<VulkanImageView> VulkanImageView::Create
	(
		const std::shared_ptr<ImageBase> image,
		VkImageViewType viewType,
		VkFormat format,
		VkImageAspectFlags aspectMask,
		uint32_t baseMipLevel,
		uint32_t mipLevels,
		uint32_t baseArrayLayer,
		uint32_t layerCount,
		const VkComponentMapping& componetMap
	)
	{
		std::shared_ptr<VulkanImageView> ret = std::make_shared<VulkanImageView>();
		ret->mImage = image;

		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image->GetHandle();
		createInfo.viewType = viewType;
		createInfo.format = format;

		createInfo.subresourceRange.aspectMask = aspectMask;
		createInfo.subresourceRange.levelCount = mipLevels;
		createInfo.subresourceRange.baseMipLevel = baseMipLevel;
		createInfo.subresourceRange.layerCount = layerCount;
		createInfo.subresourceRange.baseArrayLayer = baseArrayLayer;

		createInfo.components = componetMap;

		VkResult result = vkCreateImageView(image->GetDevicePtr()->GetHandle(), &createInfo, nullptr, &ret->mImageView);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create Image view");
			exit(EXIT_FAILURE);
		}

		return ret;
	}

	std::shared_ptr<VulkanImageView> VulkanImageView::Create(const std::shared_ptr<ImageBase>& image, VkImageViewType view_type,
		VkImageAspectFlags aspect_mask)
	{
		return Create(image, view_type, image->GetFormat(), aspect_mask, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
	}

	std::shared_ptr<VulkanImageView> VulkanImageView::Create(const std::shared_ptr<SwapChainImage> image)
	{
		return Create(image, VK_IMAGE_VIEW_TYPE_2D, image->GetFormat(), VK_IMAGE_ASPECT_COLOR_BIT, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
	}

	VulkanImageView::~VulkanImageView()
	{
		if (mImageView)
		{
			vkDestroyImageView(mImage->GetDevicePtr()->GetHandle(), mImageView, nullptr);
		}
	}
}