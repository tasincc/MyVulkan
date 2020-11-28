#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"

namespace MVK
{
	class ImageBase : public DeviceObjectBase
	{
	protected:
		VkImage m_Image{ nullptr };

		VkFormat m_format{};
		VkExtent3D m_Extent{};
		VkImageType m_Type{};
		uint32_t m_mipLevel{};
		uint32_t m_arrayLayer{};
	public:
		VkImage GetHandle() const { return m_Image; }
		const VkExtent3D& GetExtent() const { return m_Extent; }
		VkFormat GetFormat() const { return m_format; }
		VkImageType GetType() const { return m_Type; }
		uint32_t GetMipLevels() const { return m_mipLevel; }
		uint32_t GetArrayLayers()const { return m_arrayLayer; }
		VkImageSubresourceRange GetSubresourceRange(VkImageAspectFlags aspect_mask) const { return VkImageSubresourceRange{ aspect_mask ,0,m_mipLevel,0,m_arrayLayer }; }
	};
}
#endif