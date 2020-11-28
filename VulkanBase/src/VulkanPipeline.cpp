#include "VulkanPipeline.h"
#include "Log.h"

namespace MVK
{
	PipelineBase::~PipelineBase()
	{
		if (m_pipeline)
		{
			vkDestroyPipeline(m_pipeline_layouts_ptr->GetDevicePtr()->GetHandle(), m_pipeline, nullptr);
		}
	}

	std::shared_ptr<VulkanGraphicPipeline> VulkanGraphicPipeline::CreateGraphicPipeline(const std::shared_ptr<VulkanPipelineLayout>& pipeline_layout, const std::shared_ptr<VulkanRenderPass>& render_pass, const VkGraphicsPipelineCreateInfo& create_info)
	{
		std::shared_ptr<VulkanGraphicPipeline> ret = std::make_shared<VulkanGraphicPipeline>();
		ret->m_render_pass_ptr = render_pass;
		ret->m_pipeline_layouts_ptr = pipeline_layout;

		VkResult result = vkCreateGraphicsPipelines(pipeline_layout->GetDevicePtr()->GetHandle(), VK_NULL_HANDLE, 1, &create_info, nullptr, &ret->m_pipeline);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create graphic pipeline");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}