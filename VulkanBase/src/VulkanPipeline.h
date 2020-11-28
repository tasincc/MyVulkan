#ifndef VULKAN_GRAPHIC_PIPELINE_H
#define VULKAN_GRAPHIC_PIPELINE_H

#include "ShaderModule.h"
#include "DeviceObjectBase.h"
#include "VulkanPipelineLayout.h"
#include "VulkanRenderPass.h"
#include <memory>
#include <vector>

namespace MVK
{
	class PipelineBase :public DeviceObjectBase
	{
	protected:
		VkPipeline m_pipeline{ nullptr };
		std::shared_ptr<VulkanPipelineLayout> m_pipeline_layouts_ptr{ nullptr };
	public:
		~PipelineBase();
		VkPipeline GetHandle() const { return m_pipeline; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_pipeline_layouts_ptr->GetDevicePtr(); }
	};

	class VulkanGraphicPipeline :PipelineBase
	{
	private:
		std::shared_ptr<VulkanRenderPass> m_render_pass_ptr;
	public:
		static std::shared_ptr<VulkanGraphicPipeline> CreateGraphicPipeline(const std::shared_ptr<VulkanPipelineLayout>& pipeline_layout, const std::shared_ptr<VulkanRenderPass>& render_pass, const VkGraphicsPipelineCreateInfo& create_info);
	};

	class VulkanComputePipeline :PipelineBase
	{
	};
}

#endif // VULKAN_GRAPHIC_PIPELINE_H
