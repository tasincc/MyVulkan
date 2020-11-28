#include "VulkanDescriptorSetLayout.h"
#include "Log.h"

namespace MVK
{
	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (m_descriptor_set_layout)
		{
			vkDestroyDescriptorSetLayout(m_device->GetHandle(), m_descriptor_set_layout, nullptr);
		}
	}

	std::shared_ptr<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::CreaeteDescriptorSetLayout(const std::shared_ptr<VulkanDevice>& device, const std::vector<VkDescriptorSetLayoutBinding>& layoutBinding)
	{
		std::shared_ptr<VulkanDescriptorSetLayout> ret = std::make_shared<VulkanDescriptorSetLayout>();
		ret->m_device = device;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBinding.size());
		layoutInfo.pBindings = layoutBinding.data();

		if (vkCreateDescriptorSetLayout(device->GetHandle(), &layoutInfo, nullptr, &ret->m_descriptor_set_layout) != VK_SUCCESS) {
			CORE_ERROR("failed to create descriptor set layout");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}