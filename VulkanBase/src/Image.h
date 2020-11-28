#ifndef IMAGE_H
#define IMAGE_H

#include "vulkan/vulkan.h"
#include <memory>
#include "VulkanDevice.h"
#include "ImageBase.h"
#include "VulkanQueue.h"

namespace MVK
{
	class Image : public ImageBase
	{
	private:
		std::shared_ptr<VulkanDevice> m_device;
		VkDeviceMemory m_memory;
	public:
		~Image();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		static std::shared_ptr<Image> Create(const std::shared_ptr<VulkanDevice>& device, VkImageCreateInfo& createInfo, VkMemoryPropertyFlags properties);
		static std::shared_ptr<Image> CreateTexture2D(
			const std::shared_ptr<VulkanDevice>& device,
			uint32_t width,
			uint32_t height,
			uint32_t mip_level,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkSampleCountFlagBits sample,
			VkMemoryPropertyFlags propertiese
		);
	};
}
#endif