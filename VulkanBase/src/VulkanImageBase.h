#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"

namespace MVK
{
	class ImageBase : public DeviceObjectBase
	{
	protected:
		VkImage mImage{ nullptr };

		VkFormat mFormat{};
		VkExtent3D mExtent{};
		VkImageType mType{};
		uint32_t mMipLevel{};
		uint32_t mArrayLayer{};
	public:
		VkImage GetHandle() const { return mImage; }
		const VkExtent3D& GetExtent() const { return mExtent; }
		VkFormat GetFormat() const { return mFormat; }
		VkImageType GetType() const { return mType; }
		uint32_t GetMipLevels() const { return mMipLevel; }
		uint32_t GetArrayLayers()const { return mArrayLayer; }
		VkImageSubresourceRange GetSubresourceRange(VkImageAspectFlags aspect_mask) const { return VkImageSubresourceRange{ aspect_mask ,0,mMipLevel,0,mArrayLayer }; }

		VkImageMemoryBarrier GetImageMemoryBarrir(
			VkImageAspectFlags aspect_mask,
			VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
			VkImageLayout oldLayout, VkImageLayout newLayout) const;

		VkImageMemoryBarrier GetImageMemoryBarrir(
			const VkImageSubresourceRange& region,
			VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
			VkImageLayout oldLayout, VkImageLayout newLayout) const;

		std::vector<VkImageMemoryBarrier> GetImageMemoryBarrirs(
			const std::vector<VkImageSubresourceRange>& regions,
			VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
			VkImageLayout oldLayout, VkImageLayout newLayout) const;

		std::vector<VkImageMemoryBarrier> GetImageMemoryBarrirs(
			const std::vector<VkImageSubresourceLayers>& copy,
			VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
			VkImageLayout oldLayout, VkImageLayout newLayout) const;
	};
}
#endif