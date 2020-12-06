#ifndef IMAGE_H
#define IMAGE_H

#include "vulkan/vulkan.h"
#include <memory>
#include "VulkanDevice.h"
#include "VulkanImageBase.h"
#include "VulkanQueue.h"

namespace MVK
{
	class VulkanImage : public ImageBase
	{
	private:
		std::shared_ptr<VulkanDevice> m_device;
		VkDeviceMemory m_memory;
	public:
		~VulkanImage();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		static std::shared_ptr<VulkanImage> Create(const std::shared_ptr<VulkanDevice>& device, VkImageCreateInfo& createInfo, VkMemoryPropertyFlags properties);
		static std::shared_ptr<VulkanImage> CreateTexture2D(
			const std::shared_ptr<VulkanDevice>& device,
			VkExtent2D extent,
			uint32_t mip_level,
			VkFormat format,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags propertiese
		);
		static uint32_t QueryMipLevel(uint32_t w);
		static uint32_t QueryMipLevel(VkExtent2D w);
		static uint32_t QueryMipLevel(VkExtent3D w);
	};
}
#endif