#include "VulkanImage.h"
#include "Log.h"

namespace MVK
{
	VulkanImage::~VulkanImage()
	{
		if (mImage)
		{
			vkDestroyImage(m_device->GetHandle(), mImage, nullptr);
			vkFreeMemory(m_device->GetHandle(), m_memory, nullptr);
		}
	}

	std::shared_ptr<VulkanImage> VulkanImage::Create(const std::shared_ptr<VulkanDevice>& device, VkImageCreateInfo& createInfo, VkMemoryPropertyFlags properties)
	{
		std::shared_ptr<VulkanImage> ret = std::make_shared<VulkanImage>();
		ret->m_device = device;
		ret->mFormat = createInfo.format;
		ret->mExtent = createInfo.extent;
		ret->mType = createInfo.imageType;
		ret->mMipLevel = createInfo.mipLevels;
		ret->mArrayLayer = createInfo.arrayLayers;
		VkResult result = vkCreateImage(device->GetHandle(), &createInfo, nullptr, &ret->mImage);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("ss");
			exit(EXIT_FAILURE);
		}
		VkMemoryRequirements memRequire;
		vkGetImageMemoryRequirements(device->GetHandle(), ret->mImage, &memRequire);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequire.size;
		allocInfo.memoryTypeIndex = device->GetPhysicalDevicePtr()->FindMemoryType(memRequire.memoryTypeBits, properties);

		result = vkAllocateMemory(device->GetHandle(), &allocInfo, nullptr, &ret->m_memory);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("SS");
			exit(EXIT_FAILURE);
		}
		vkBindImageMemory(device->GetHandle(), ret->mImage, ret->m_memory, 0);
		return ret;
	}

	std::shared_ptr<VulkanImage> VulkanImage::CreateTexture2D(
		const std::shared_ptr<VulkanDevice>& device,
		VkExtent2D extent,
		uint32_t mip_level,
		VkFormat format,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties
	) {
		VkImageCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		create_info.imageType = VK_IMAGE_TYPE_2D;
		create_info.extent = { extent.width,extent.height, 1 };
		create_info.mipLevels = mip_level;
		create_info.arrayLayers = 1;
		create_info.format = format;
		create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		create_info.usage = usage;
		create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		return Create(device, create_info, properties);
	}

	static inline uint32_t simple_ctz(uint32_t x) {
		uint32_t ret{ 1 };
		while (x >> ret) ++ret;
		return ret;
	}

	uint32_t VulkanImage::QueryMipLevel(uint32_t w)
	{
		return simple_ctz(w);
	}

	uint32_t VulkanImage::QueryMipLevel(VkExtent2D w)
	{
		return simple_ctz(w.width | w.height);
	}

	uint32_t VulkanImage::QueryMipLevel(VkExtent3D w)
	{
		return simple_ctz(w.width | w.height | w.depth);
	}
}