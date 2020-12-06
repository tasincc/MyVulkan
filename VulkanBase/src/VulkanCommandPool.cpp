#include "VulkanCommandPool.h"
#include "Log.h"

namespace MVK
{
	VulkanCommandPool::~VulkanCommandPool()
	{
		if (mCommandPool)
		{
			vkDestroyCommandPool(mQueuePtr->GetDevicePtr()->GetHandle(), mCommandPool, nullptr);
		}
	}

	std::shared_ptr<VulkanCommandPool> VulkanCommandPool::Create(const std::shared_ptr<VulkanQueue>& queue, VkCommandPoolCreateFlags flag /*VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT*/)
	{
		std::shared_ptr<VulkanCommandPool> ret = std::make_shared<VulkanCommandPool>();
		ret->mQueuePtr = queue;

		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = queue->GetFamilyIndex();
		createInfo.flags = flag;

		VkResult result = vkCreateCommandPool(queue->GetDevicePtr()->GetHandle(), &createInfo, nullptr, &ret->mCommandPool);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create command pool");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}