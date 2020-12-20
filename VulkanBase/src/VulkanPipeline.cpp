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

	VkPipelineInputAssemblyStateCreateInfo VulkanGraphicPipeline::GetPipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkPipelineTessellationStateCreateFlags flags, VkBool32 primitiveRestartEnable)
	{
		VkPipelineInputAssemblyStateCreateInfo ret{};
		ret.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		ret.primitiveRestartEnable = VK_FALSE;
		ret.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		return ret;
	}

	VkPipelineRasterizationStateCreateInfo VulkanGraphicPipeline::GetPipelineRasterizationStateCreateInfo(VkPolygonMode polygonMode,VkCullModeFlags cullMode,VkFrontFace frontFace)
	{
		VkPipelineRasterizationStateCreateInfo ret{};
		ret.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		ret.polygonMode = polygonMode;
		ret.lineWidth = 1.0f;
		ret.cullMode = cullMode;
		ret.frontFace = frontFace;
		ret.depthClampEnable = VK_FALSE;
		ret.depthBiasClamp = VK_FALSE;
		ret.rasterizerDiscardEnable = VK_FALSE;
		return ret;
	}

	VkPipelineDepthStencilStateCreateInfo VulkanGraphicPipeline::GetPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable,VkBool32 depthWriteEnable,VkCompareOp depthCompareOp)
	{
		VkPipelineDepthStencilStateCreateInfo ret{};
		ret.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		ret.depthTestEnable = depthTestEnable;
		ret.depthWriteEnable = depthWriteEnable;
		ret.depthCompareOp = depthCompareOp;
		ret.back.compareOp = VK_COMPARE_OP_ALWAYS;
		ret.depthBoundsTestEnable = VK_FALSE;
		ret.stencilTestEnable = VK_FALSE;
		return ret;
	}
}