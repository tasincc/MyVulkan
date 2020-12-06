#include "VulkanRenderPass.h"
#include "Log.h"

namespace MVK
{
	VulkanRenderPass::~VulkanRenderPass()
	{
		if (mRenderPass)
		{
			vkDestroyRenderPass(mDevice->GetHandle(), mRenderPass, nullptr);
		}
	}

	std::shared_ptr<VulkanRenderPass> VulkanRenderPass::Create(const std::shared_ptr<VulkanDevice>& device, const VkRenderPassCreateInfo& createInfo)
	{
		std::shared_ptr<VulkanRenderPass> ret = std::make_shared<VulkanRenderPass>();
		ret->mDevice = device;

		VkResult result = vkCreateRenderPass(device->GetHandle(), &createInfo, nullptr, &ret->mRenderPass);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create render pass");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}