#ifndef VULKAN_COMMAND_BUUFER
#define VULKAN_COMMAND_BUUFER

#include "vulkan/vulkan.h"
#include "VulkanCommandPool.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanBuffer.h"
#include "DeviceObjectBase.h"
#include "VulkanImage.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanDescriptorSet.h"
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
		VkResult Submit(const VulkanSemaphoreStageGroup& waitSemaphores,
			const VulkanSemaphoreGroup& signalSemaphores,
			const std::shared_ptr<VulkanFence>& fence);

		void BeginRenderPass(const std::shared_ptr<VulkanRenderPass>& renderPass,
			const std::shared_ptr<VulkanFramebuffer> frameBuffer,
			const std::vector<VkClearValue>& clearValues,
			VkSubpassContents subpassContent);
		void EndRenderPass();
		void CmdBindPipeline(const std::shared_ptr<PipelineBase>& pipeline, VkPipelineBindPoint bindPoint);
		void CmdBindVertexBuffer(const std::vector<std::shared_ptr<VulkanBuffer>>& buffers, VkDeviceSize offset, uint32_t start_slot = 0);
		void CmdBindIndexBuffer(const std::shared_ptr<VulkanBuffer>& buffer, VkDeviceSize offset, VkIndexType indexType);
		void CmdBindDescriptorSet(VkPipelineBindPoint bindPoint, uint32_t firstSet,
			const std::shared_ptr<VulkanPipelineLayout>& layout, 
			const std::vector<std::shared_ptr<VulkanDescriptorSet>>& descriptorSets, 
			const std::vector<uint32_t>& offset);
		void CmdSetViewport(const VkViewport& viewport);
		void CmdSetScissor(const VkRect2D& scissor);

		void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);


		void CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, std::vector<VkMemoryBarrier> memoryBarriers, std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers, std::vector<VkImageMemoryBarrier> imageMemoryBarriers) const;

		void CmdCopyBuffer(std::shared_ptr<VulkanBuffer> src, std::shared_ptr<VulkanBuffer> dest, const std::vector<VkBufferCopy>& regions) const;
		void CmdCopyBuffer(std::shared_ptr<VulkanBuffer> src, std::shared_ptr<VulkanBuffer> dest) const;
		void CmdCopyBuffer(std::shared_ptr<VulkanBuffer> buffer, std::shared_ptr<VulkanImage> image, const VkBufferImageCopy& region, VkImageLayout layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) const;

		void CmdGenerateMipmaps(std::shared_ptr<VulkanImage> image, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldLayout, VkImageLayout newLayout) const;
		void CmdBlitImage(std::shared_ptr<VulkanImage> src, std::shared_ptr<VulkanImage> dest, const std::vector<VkImageBlit>& blitInfo, VkFilter filter) const;
	};
}

#endif
