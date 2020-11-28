#include "VulkanCommandPool.h"
#include "Log.h"

namespace MVK
{
	VulkanCommandPool::~VulkanCommandPool()
	{
		if (m_command_pool)
		{
			vkDestroyCommandPool(m_graphic_queue_ptr->GetDevicePtr()->GetHandle(), m_command_pool, nullptr);
		}
	}

	std::shared_ptr<VulkanCommandPool> VulkanCommandPool::CreateCommandPool(const std::shared_ptr<VulkanQueue> graphicQueue, VkCommandPoolCreateFlags flag)
	{
		std::shared_ptr<VulkanCommandPool> ret = std::make_shared<VulkanCommandPool>();
		ret->m_graphic_queue_ptr = graphicQueue;

		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = graphicQueue->GetFamilyIndex();
		createInfo.flags = flag; //VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT

		VkResult result = vkCreateCommandPool(graphicQueue->GetDevicePtr()->GetHandle(), &createInfo, nullptr, &ret->m_command_pool);
		if (result != VK_SUCCESS)
		{
			CORE_ERROR("failed to create command pool");
			exit(EXIT_FAILURE);
		}

		return ret;
	}
}