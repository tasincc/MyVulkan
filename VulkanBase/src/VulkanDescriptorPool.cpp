#include "VulkanDescriptorPool.h"
#include "Log.h"

namespace MVK
{
	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		if (mDescriptorPool != nullptr)
		{
			vkDestroyDescriptorPool(mDevice->GetHandle(), mDescriptorPool, nullptr);
		}
	}

	std::shared_ptr<VulkanDescriptorPool> VulkanDescriptorPool::Create(const std::shared_ptr<VulkanDevice>& device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes)
	{
		std::shared_ptr<VulkanDescriptorPool> ret = std::make_shared<VulkanDescriptorPool>();
		ret->mDevice = device;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = maxSets;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();

		VkResult result = vkCreateDescriptorPool(device->GetHandle(), &poolInfo, nullptr, &ret->mDescriptorPool);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create descriptor pool");
		}

		return ret;
	}
}