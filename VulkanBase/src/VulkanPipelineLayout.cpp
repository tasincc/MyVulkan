#include "VulkanPipelineLayout.h"
#include "Log.h"

namespace MVK
{
	VulkanPipelineLayout::~VulkanPipelineLayout()
	{
		if (mPipelineLayout)
		{
			vkDestroyPipelineLayout(mDevicePtr->GetHandle(), mPipelineLayout, nullptr);
		}
	}

	std::shared_ptr<VulkanPipelineLayout> VulkanPipelineLayout::Create(std::shared_ptr<VulkanDevice> device,const std::vector<std::shared_ptr<VulkanDescriptorSetLayout>>& descriptorlayouts, const std::vector<VkPushConstantRange>& pushConstantRanges)
	{
		std::shared_ptr<VulkanPipelineLayout> ret = std::make_shared<VulkanPipelineLayout>();
		ret->mDevicePtr = device;
		ret->mDescriptorLayoutPtrs = descriptorlayouts;

		std::vector<VkDescriptorSetLayout> descriptorSetLayoutsHandle(descriptorlayouts.size());
		for (uint32_t i = 0; i < static_cast<uint32_t>(descriptorlayouts.size()); i++)
		{
			descriptorSetLayoutsHandle[i] = descriptorlayouts[i]->GetHandle();
		}

		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayoutsHandle.size());
		createInfo.pSetLayouts = descriptorSetLayoutsHandle.data();
		createInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
		createInfo.pPushConstantRanges = pushConstantRanges.data();

		VkResult result = vkCreatePipelineLayout(device->GetHandle(), &createInfo, nullptr, &ret->mPipelineLayout);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create pipeline layout");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}