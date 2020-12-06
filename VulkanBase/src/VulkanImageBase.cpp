#include "VulkanImageBase.h"

namespace MVK
{
	VkImageMemoryBarrier ImageBase::GetImageMemoryBarrir(
		VkImageAspectFlags aspect_mask,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout, VkImageLayout newLayout) const
	{
		VkImageMemoryBarrier ret{};
		ret.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		ret.srcAccessMask = srcAccessMask;
		ret.dstAccessMask = dstAccessMask;
		ret.oldLayout = oldLayout;
		ret.newLayout = newLayout;
		ret.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		ret.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		ret.image = mImage;
		ret.subresourceRange = GetSubresourceRange(aspect_mask);
		return ret;
	}

	VkImageMemoryBarrier ImageBase::GetImageMemoryBarrir(
		const VkImageSubresourceRange& region,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout, VkImageLayout newLayout) const
	{
		VkImageMemoryBarrier ret{};
		ret.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		ret.srcAccessMask = srcAccessMask;
		ret.dstAccessMask = dstAccessMask;
		ret.oldLayout = oldLayout;
		ret.newLayout = newLayout;
		ret.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		ret.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		ret.image = mImage;
		ret.subresourceRange = region;
		return ret;
	}

	std::vector<VkImageMemoryBarrier> ImageBase::GetImageMemoryBarrirs(
		const std::vector<VkImageSubresourceRange>& regions,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout, VkImageLayout newLayout) const
	{
		std::vector<VkImageMemoryBarrier> ret(regions.size());
		for (uint32_t i = 0; i < static_cast<uint32_t>(regions.size()); i++)
		{
			VkImageMemoryBarrier& barrierInfo = ret[i];
			barrierInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrierInfo.srcAccessMask = srcAccessMask;
			barrierInfo.dstAccessMask = dstAccessMask;
			barrierInfo.oldLayout = oldLayout;
			barrierInfo.newLayout = newLayout;
			barrierInfo.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrierInfo.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrierInfo.image = mImage;
			barrierInfo.subresourceRange = regions[i];
		}
		return ret;
	}

	std::vector<VkImageMemoryBarrier> ImageBase::GetImageMemoryBarrirs(
		const std::vector<VkImageSubresourceLayers>& regions,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout, VkImageLayout newLayout) const
	{
		std::vector<VkImageMemoryBarrier> ret(regions.size());
		for (uint32_t i = 0; i < static_cast<uint32_t>(regions.size()); i++)
		{
			VkImageMemoryBarrier& barrierInfo = ret[i];
			barrierInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrierInfo.srcAccessMask = srcAccessMask;
			barrierInfo.dstAccessMask = dstAccessMask;
			barrierInfo.oldLayout = oldLayout;
			barrierInfo.newLayout = newLayout;
			barrierInfo.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrierInfo.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrierInfo.image = mImage;
			barrierInfo.subresourceRange.aspectMask = regions[i].aspectMask;
			barrierInfo.subresourceRange.baseArrayLayer = regions[i].baseArrayLayer;
			barrierInfo.subresourceRange.layerCount = regions[i].layerCount;
			barrierInfo.subresourceRange.baseMipLevel = regions[i].mipLevel;
			barrierInfo.subresourceRange.levelCount = 1;
		}
		return ret;
	}
}