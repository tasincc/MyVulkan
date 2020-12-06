#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "VulkanImageBase.h"
#include "SwapChainImage.h"
#include <memory>

namespace MVK
{
	class VulkanImageView
	{
	private:
		std::shared_ptr<ImageBase> mImage;
		VkImageView mImageView{ nullptr };
	public:
		~VulkanImageView();
		static std::shared_ptr<VulkanImageView> Create(
			const std::shared_ptr<ImageBase> image,
			VkImageViewType viewType,
			VkFormat format,
			VkImageAspectFlags aspectMask,
			uint32_t baseMipLevel = 0,
			uint32_t mipLevels = 1,
			uint32_t baseArrayLayer = 0,
			uint32_t layerCount = 1,
			const VkComponentMapping& componetMap = {
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY
			}
		);

		static std::shared_ptr<VulkanImageView> Create(const std::shared_ptr<ImageBase>& image, VkImageViewType view_type,
			VkImageAspectFlags aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT);

		static std::shared_ptr<VulkanImageView> Create(const std::shared_ptr<SwapChainImage> image);

		VkImageView GetHandle() const { return mImageView; }

	};
}

#endif
