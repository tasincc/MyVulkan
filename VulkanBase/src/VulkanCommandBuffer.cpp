#include "VulkanCommandBuffer.h"
#include "Log.h"

namespace MVK
{
	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		if (mCommandBuffer != nullptr)
		{
			vkFreeCommandBuffers(mCommandPool->GetDevicePtr()->GetHandle(), mCommandPool->GetHandle(), 1, &mCommandBuffer);
		}
	}

	std::shared_ptr<MVK::VulkanCommandBuffer> VulkanCommandBuffer::Create(const std::shared_ptr<VulkanCommandPool>& commandPool, VkCommandBufferLevel level /*= VK_COMMAND_BUFFER_LEVEL_PRIMARY*/)
	{
		std::shared_ptr<VulkanCommandBuffer> ret = std::make_shared<VulkanCommandBuffer>();
		ret->mCommandPool = commandPool;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = level;
		allocInfo.commandBufferCount = 1;
		allocInfo.commandPool = commandPool->GetHandle();

		VkResult result = vkAllocateCommandBuffers(commandPool->GetDevicePtr()->GetHandle(), &allocInfo, &ret->mCommandBuffer);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create command buffer");
		}

		return ret;
	}

	VkResult VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags flag)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//beginInfo.flags = flag;

		return vkBeginCommandBuffer(mCommandBuffer, &beginInfo);
	}

	VkResult VulkanCommandBuffer::End()
	{
		return	vkEndCommandBuffer(mCommandBuffer);
	}

	VkResult VulkanCommandBuffer::Submit(const VulkanSemaphoreStageGroup& waitSemaphores,
		const VulkanSemaphoreGroup& signalSemaphores,
		const std::shared_ptr<VulkanFence>& fence)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffer;

		//todo
		if (waitSemaphores.GetCount())
		{
			submitInfo.waitSemaphoreCount = waitSemaphores.GetCount();
			submitInfo.pWaitSemaphores = waitSemaphores.GetSemaphorePtr();
			submitInfo.pWaitDstStageMask = waitSemaphores.GetPipelineStageFlags();
		}

		if (signalSemaphores.GetCount())
		{
			submitInfo.signalSemaphoreCount = signalSemaphores.GetCount();
			submitInfo.pSignalSemaphores = signalSemaphores.GetSemaphorePtr();
		}

		return vkQueueSubmit(mCommandPool->GetQueuePtr()->GetHandle(), 1, &submitInfo, fence ? fence->GetHandle() : VK_NULL_HANDLE);
	}

	void VulkanCommandBuffer::BeginRenderPass(const std::shared_ptr<VulkanRenderPass>& renderPass, const std::shared_ptr<VulkanFramebuffer> frameBuffer, const std::vector<VkClearValue>& clearValues, VkSubpassContents subpassContent)
	{
		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass->GetHandle();
		beginInfo.framebuffer = frameBuffer->GetHandle();
		beginInfo.renderArea.offset = { 0,0 };
		beginInfo.renderArea.extent = frameBuffer->GetExtent();
		beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		beginInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(mCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::EndRenderPass()
	{
		vkCmdEndRenderPass(mCommandBuffer);
	}

	void VulkanCommandBuffer::CmdBindPipeline(const std::shared_ptr<PipelineBase>& pipeline, VkPipelineBindPoint bindPoint)
	{
		vkCmdBindPipeline(mCommandBuffer, bindPoint, pipeline->GetHandle());
	}

	void VulkanCommandBuffer::CmdBindVertexBuffer(const std::vector<std::shared_ptr<VulkanBuffer>>& buffers, VkDeviceSize offset, uint32_t start_slot)
	{
		std::vector<VkBuffer> handles(buffers.size());
		for (uint32_t i = 0; i < static_cast<uint32_t>(buffers.size()); i++)
		{
			handles[i] = buffers[i]->GetHandle();
		}
		vkCmdBindVertexBuffers(mCommandBuffer, start_slot, static_cast<uint32_t>(handles.size()), handles.data(), &offset);
	}

	void VulkanCommandBuffer::CmdBindIndexBuffer(const std::shared_ptr<VulkanBuffer>& buffer, VkDeviceSize offset, VkIndexType indexType)
	{
		vkCmdBindIndexBuffer(mCommandBuffer, buffer->GetHandle(), offset, indexType);
	}

	void VulkanCommandBuffer::CmdBindDescriptorSet(VkPipelineBindPoint bindPoint, uint32_t firstSet, const std::shared_ptr<VulkanPipelineLayout>& layout, const std::vector<std::shared_ptr<VulkanDescriptorSet>>& descriptorSets,const std::vector<uint32_t>& offset)
	{
		std::vector<VkDescriptorSet> handles(descriptorSets.size());
		for (uint32_t i = 0; i < static_cast<uint32_t>(descriptorSets.size()); i++)
		{
			handles[i] = descriptorSets[i]->GetHandle();
		}
		vkCmdBindDescriptorSets(mCommandBuffer, bindPoint, layout->GetHandle(), firstSet, static_cast<uint32_t>(handles.size()), handles.data(),static_cast<uint32_t>(offset.size()), offset.data());
	}

	void VulkanCommandBuffer::CmdSetViewport(const VkViewport& viewport)
	{
		vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
	}

	void VulkanCommandBuffer::CmdSetScissor(const VkRect2D& scissor)
	{
		vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);
	}

	void VulkanCommandBuffer::CmdDraw(uint32_t vertexCount,uint32_t instanceCount,uint32_t firstVertex,uint32_t firstInstance)
	{
		vkCmdDraw(mCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void VulkanCommandBuffer::CmdDrawIndexed(uint32_t indexCount,uint32_t instanceCount,uint32_t firstIndex,int32_t vertexOffset,uint32_t firstInstance)
	{
		vkCmdDrawIndexed(mCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void VulkanCommandBuffer::CmdPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
		std::vector<VkMemoryBarrier> memoryBarriers,
		std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers,
		std::vector<VkImageMemoryBarrier> imageMemoryBarriers) const
	{
		vkCmdPipelineBarrier(mCommandBuffer,
			srcStageMask, dstStageMask, 0,
			memoryBarriers.size(), memoryBarriers.data(),
			bufferMemoryBarriers.size(), bufferMemoryBarriers.data(),
			imageMemoryBarriers.size(), imageMemoryBarriers.data());
	}

	void VulkanCommandBuffer::CmdCopyBuffer(std::shared_ptr<VulkanBuffer> src,
		std::shared_ptr<VulkanBuffer> dest, const std::vector<VkBufferCopy>& regions) const
	{
		vkCmdCopyBuffer(mCommandBuffer, src->GetHandle(), dest->GetHandle(), regions.size(), regions.data());
	}

	void VulkanCommandBuffer::CmdCopyBuffer(std::shared_ptr<VulkanBuffer> src, std::shared_ptr<VulkanBuffer> dest) const
	{
		VkBufferCopy copyRegion{};
		copyRegion.size = src->GetSize();
		vkCmdCopyBuffer(mCommandBuffer, src->GetHandle(), dest->GetHandle(), 1, &copyRegion);
	}

	void VulkanCommandBuffer::CmdCopyBuffer(std::shared_ptr<VulkanBuffer> buffer,
		std::shared_ptr<VulkanImage> image, const VkBufferImageCopy& region, VkImageLayout layout) const
	{
		vkCmdCopyBufferToImage(mCommandBuffer, buffer->GetHandle(), image->GetHandle(), layout, 1, &region);
	}

	void VulkanCommandBuffer::CmdGenerateMipmaps(std::shared_ptr<VulkanImage> image,
		VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout, VkImageLayout newLayout) const
	{
		if (image->GetMipLevels() == 1)
		{
			VkImageSubresourceRange subresource{};
			subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresource.baseMipLevel = 0;
			subresource.levelCount = image->GetMipLevels();
			subresource.baseArrayLayer = 0;
			subresource.layerCount = image->GetArrayLayers();
			CmdPipelineBarrier(srcStageMask, dstStageMask, {}, {}, image->GetImageMemoryBarrirs({ subresource },
				srcAccessMask, dstAccessMask, oldLayout, newLayout));
			return;
		}

		int32_t mipWidth = image->GetExtent().width;
		int32_t mipHeight = image->GetExtent().height;
		int32_t mipDepth = image->GetExtent().depth;

		for (uint32_t i = 1; i < image->GetMipLevels(); i++)
		{
			VkImageBlit bilt{};
			bilt.srcOffsets[0] = { 0,0,0 };
			bilt.srcOffsets[1] = { mipWidth,mipHeight,mipDepth };
			bilt.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bilt.srcSubresource.baseArrayLayer = 0;
			bilt.srcSubresource.layerCount = image->GetArrayLayers();
			bilt.srcSubresource.mipLevel = i - 1;
			mipWidth = std::max(mipWidth / 2, 1);
			mipHeight = std::max(mipHeight / 2, 1);
			bilt.dstOffsets[0] = { 0,0,0 };
			bilt.dstOffsets[1] = { mipWidth,mipHeight,mipDepth };
			bilt.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bilt.dstSubresource.baseArrayLayer = 0;
			bilt.dstSubresource.layerCount = image->GetArrayLayers();
			bilt.dstSubresource.mipLevel = i;

			//mipmap i - 1�任����dst_optimal��src_optimal����Ϊblit��src
			CmdPipelineBarrier(i == 1 ? srcStageMask : VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				{}, {}, image->GetImageMemoryBarrirs({ bilt.srcSubresource },
					i == 1 ? srcAccessMask : VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_ACCESS_TRANSFER_READ_BIT,
					i == 1 ? oldLayout : VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL));

			//mipmap i �任���ֵ�dst_optimal����Ϊblit��dst
			CmdPipelineBarrier(i == 1 ? srcStageMask : VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				{}, {}, image->GetImageMemoryBarrirs({ bilt.dstSubresource },
					i == 1 ? srcAccessMask : 0,
					VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL));

			CmdBlitImage(image, image, { bilt }, VK_FILTER_LINEAR);


			CmdPipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask,
				{}, {}, image->GetImageMemoryBarrirs({ bilt.srcSubresource },
					VK_ACCESS_TRANSFER_READ_BIT,
					dstAccessMask,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					newLayout));


			if (i == image->GetMipLevels() - 1)
			{
				CmdPipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask,
					{}, {}, image->GetImageMemoryBarrirs({ bilt.dstSubresource },
						VK_ACCESS_TRANSFER_WRITE_BIT,
						dstAccessMask,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						newLayout));
			}
		}
	}

	void VulkanCommandBuffer::CmdBlitImage(std::shared_ptr<VulkanImage> src, std::shared_ptr<VulkanImage> dest, const std::vector<VkImageBlit>& blitInfo, VkFilter filter) const
	{
		vkCmdBlitImage(mCommandBuffer, src->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dest->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, blitInfo.size(), blitInfo.data(), filter);
	}
}
