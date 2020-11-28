#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "ImageBase.h"
#include "SwapChainImage.h"
#include <memory>

namespace MVK
{
	class ImageView
	{
	private:
		std::shared_ptr<ImageBase> m_Image;
		VkImageView m_ImageView{ nullptr };
	public:
		~ImageView();
		static std::shared_ptr<ImageView> createImageView(
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
		static std::shared_ptr<ImageView> createImageView(const std::shared_ptr<SwapChainImage> image);

	};
}

#endif
