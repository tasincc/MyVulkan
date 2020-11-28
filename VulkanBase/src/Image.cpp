#include "Image.h"
#include "Log.h"

namespace MVK
{
	Image::~Image()
	{
		if (m_Image)
		{
			vkDestroyImage(m_device->GetHandle(), m_Image, nullptr);
			vkFreeMemory(m_device->GetHandle(), m_memory, nullptr);
		}
	}

	std::shared_ptr<Image> Image::Create(const std::shared_ptr<VulkanDevice>& device, VkImageCreateInfo& createInfo, VkMemoryPropertyFlags properties)
	{
		std::shared_ptr<Image> ret = std::make_shared<Image>();
		ret->m_device = device;
		ret->m_format = createInfo.format;
		ret->m_Extent = createInfo.extent;
		ret->m_Type = createInfo.imageType;
		ret->m_mipLevel = createInfo.mipLevels;
		ret->m_arrayLayer = createInfo.arrayLayers;
		VkResult result = vkCreateImage(device->GetHandle(), &createInfo, nullptr, &ret->m_Image);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("ss");
			exit(EXIT_FAILURE);
		}
		VkMemoryRequirements memRequire;
		vkGetImageMemoryRequirements(device->GetHandle(), ret->m_Image, &memRequire);

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
		vkBindImageMemory(device->GetHandle(), ret->m_Image, ret->m_memory, 0);
		return ret;
	}

	std::shared_ptr<Image> Image::CreateTexture2D(
		const std::shared_ptr<VulkanDevice>& device,
		uint32_t width,
		uint32_t height,
		uint32_t mip_level,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkSampleCountFlagBits sample,
		VkMemoryPropertyFlags properties
	) {
		VkImageCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		create_info.imageType = VK_IMAGE_TYPE_2D;
		create_info.extent = { width, height, 1 };
		create_info.mipLevels = mip_level;
		create_info.arrayLayers = 1;
		create_info.format = format;
		create_info.tiling = tiling;
		create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		create_info.usage = usage;
		create_info.samples = sample;
		create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		return Create(device, create_info, properties);
	}
}