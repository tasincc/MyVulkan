#include "VulkanDescriptorSet.h"
#include "Log.h"

namespace MVK
{
	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		if (mDesciptorSet != nullptr)
		{
			vkFreeDescriptorSets(mDescriptorPool->GetDevicePtr()->GetHandle(), mDescriptorPool->GetHandle(), 1, &mDesciptorSet);
		}
	}

	std::shared_ptr<VulkanDescriptorSet> VulkanDescriptorSet::Create(const std::shared_ptr<VulkanDescriptorPool>& descriptorPool, const std::shared_ptr<VulkanDescriptorSetLayout> descriptorSetLayout)
	{
		std::shared_ptr<VulkanDescriptorSet> ret = std::make_shared<VulkanDescriptorSet>();
		ret->mDescriptorPool = descriptorPool;
		ret->mDescriptorSetLayout = descriptorSetLayout;
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool->GetHandle();
		VkDescriptorSetLayout layout = descriptorSetLayout->GetHandle();
		allocInfo.pSetLayouts = &(layout);
		allocInfo.descriptorSetCount = 1;

		VkResult result = vkAllocateDescriptorSets(descriptorPool->GetDevicePtr()->GetHandle(), &allocInfo, &ret->mDesciptorSet);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create decriptor set");
		}
		return ret;
	}
}