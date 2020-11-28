#include "VulkanPipelineLayout.h"
#include "Log.h"

namespace MVK
{
	VulkanPipelineLayout::~VulkanPipelineLayout()
	{
		if (m_pipeline_layout)
		{
			vkDestroyPipelineLayout(m_device_ptr->GetHandle(), m_pipeline_layout, nullptr);
		}
	}

	std::shared_ptr<VulkanPipelineLayout> VulkanPipelineLayout::CreatePipelineLayout(std::shared_ptr<VulkanDevice> device, std::vector<std::shared_ptr<VulkanDescriptorSetLayout>> descriptorlayouts)
	{
		std::shared_ptr<VulkanPipelineLayout> ret = std::make_shared<VulkanPipelineLayout>();
		ret->m_device_ptr = device;
		ret->m_descriptor_layout_ptrs = descriptorlayouts;

		std::vector<VkDescriptorSetLayout> descriptorSetLayoutsHandle(descriptorlayouts.size());
		for (uint32_t i = 0; i < static_cast<uint32_t>(descriptorlayouts.size()); i++)
		{
			descriptorSetLayoutsHandle[i] = descriptorlayouts[i]->GetHandle();
		}

		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayoutsHandle.size());
		createInfo.pSetLayouts = descriptorSetLayoutsHandle.data();

		VkResult result = vkCreatePipelineLayout(device->GetHandle(), &createInfo, nullptr, &ret->m_pipeline_layout);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create pipeline layout");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}