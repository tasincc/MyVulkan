#include "VulkanRenderPass.h"
#include "Log.h"

namespace MVK
{
	VulkanRenderPass::~VulkanRenderPass()
	{
		if (m_render_pass)
		{
			vkDestroyRenderPass(m_device->GetHandle(), m_render_pass, nullptr);
		}
	}

	std::shared_ptr<VulkanRenderPass> VulkanRenderPass::CreateRenderPass(const std::shared_ptr<VulkanDevice> device, const VkRenderPassCreateInfo& createInfo)
	{
		std::shared_ptr<VulkanRenderPass> ret = std::make_shared<VulkanRenderPass>();
		ret->m_device = device;

		VkResult result = vkCreateRenderPass(device->GetHandle(), &createInfo, nullptr, &ret->m_render_pass);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create render pass");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}