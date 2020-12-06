#include "VulkanFrameBuffer.h"
#include "Log.h"

namespace MVK
{
	std::shared_ptr<VulkanFramebuffer> VulkanFramebuffer::Create(const std::shared_ptr<VulkanRenderPass>& renderPass, const std::vector<std::shared_ptr<VulkanImageView>>& imageViews, const VkExtent2D& extent, uint32_t layers)
	{
		std::shared_ptr<VulkanFramebuffer> ret = std::make_shared<VulkanFramebuffer>();
		ret->mRenderpass = renderPass;
		ret->mImageViews = imageViews;
		ret->mExtent = extent;
		ret->mLayer = layers;

		std::vector<VkImageView> attachments;
		for (const auto& imageView : imageViews)
		{
			attachments.push_back(imageView->GetHandle());
		}

		VkFramebufferCreateInfo framebufferCI{};
		framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCI.renderPass = renderPass->GetHandle();
		framebufferCI.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferCI.pAttachments = attachments.data();
		framebufferCI.width = extent.width;
		framebufferCI.height = extent.height;
		framebufferCI.layers = layers;

		VkResult result = vkCreateFramebuffer(renderPass->GetDevicePtr()->GetHandle(), &framebufferCI, nullptr, &ret->mFramebuffer);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create framebuffer");
		}

		return ret;
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		if (mFramebuffer != nullptr)
		{
			vkDestroyFramebuffer(GetDevicePtr()->GetHandle(), mFramebuffer, nullptr);
		}
	}
}