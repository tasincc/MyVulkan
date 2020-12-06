#include "VulkanPipeline.h"
#include "Log.h"

namespace MVK
{
	PipelineBase::~PipelineBase()
	{
		if (mPipeline)
		{
			vkDestroyPipeline(mPipelineLayoutsPtr->GetDevicePtr()->GetHandle(), mPipeline, nullptr);
		}
	}

	std::shared_ptr<VulkanGraphicPipeline> VulkanGraphicPipeline::Create(const std::shared_ptr<VulkanPipelineLayout>& pipeline_layout, const std::shared_ptr<VulkanRenderPass>& render_pass, const VkGraphicsPipelineCreateInfo& create_info)
	{
		std::shared_ptr<VulkanGraphicPipeline> ret = std::make_shared<VulkanGraphicPipeline>();
		ret->m_render_pass_ptr = render_pass;
		ret->mPipelineLayoutsPtr = pipeline_layout;

		VkResult result = vkCreateGraphicsPipelines(pipeline_layout->GetDevicePtr()->GetHandle(), VK_NULL_HANDLE, 1, &create_info, nullptr, &ret->mPipeline);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create graphic pipeline");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}