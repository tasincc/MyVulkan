#ifndef VULKAN_COMMAND_BUFFER
#define VULKAN_COMMAND_BUUFER

#include "vulkan/vulkan.h"
#include "VulkanCommandPool.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanBuffer.h"
#include "DeviceObjectBase.h"
#include "VulkanImage.h"
#include <memory>

namespace MVK
{
	class VulkanCommandBuffer :DeviceObjectBase
	{
	private:
		std::shared_ptr<VulkanCommandPool> mCommandPool{ nullptr };
		VkCommandBuffer mCommandBuffer{ nullptr };
	public:
		~VulkanCommandBuffer();
		static std::shared_ptr<VulkanCommandBuffer> Create(const std::shared_ptr<VulkanCommandPool>& commandPool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mCommandPool->GetDevicePtr(); }

		VkResult Begin(VkCommandBufferUsageFlags flag = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		VkResult End();
		VkResult Submit(const VulkanSemaphoreStageGroup& waitSemaphores, const VulkanSemaphoreGroup& signalSemaphores, const std::shared_ptr<VulkanFence>& fence);

		void CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, std::vector<VkMemoryBarrier> memoryBarriers, std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers, std::vector<VkImageMemoryBarrier> imageMemoryBarriers) const;

		void CmdCopyBuffer(std::shared_ptr<VulkanBuffer> src, std::shared_ptr<VulkanBuffer> dest, const std::vector<VkBufferCopy>& regions) const;
		void CmdCopyBuffer(std::shared_ptr<VulkanBuffer> src, std::shared_ptr<VulkanBuffer> dest) const;
		void CmdCopyBuffer(std::shared_ptr<VulkanBuffer> buffer, std::shared_ptr<VulkanImage> image, const VkBufferImageCopy& region, VkImageLayout layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) const;

		void CmdGenerateMipmaps(std::shared_ptr<VulkanImage> image, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldLayout, VkImageLayout newLayout) const;
		void CmdBlitImage(std::shared_ptr<VulkanImage> src, std::shared_ptr<VulkanImage> dest,const std::vector<VkImageBlit>& blitInfo, VkFilter filter) const;
	};
}

#endif
