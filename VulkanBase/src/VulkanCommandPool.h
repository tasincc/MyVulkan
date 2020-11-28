#ifndef VULKAN_COMMANDPOOL_H
#define VULKAN_COMMANDPOOL_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include "VulkanQueue.h"
#include <memory>

namespace MVK
{
	class VulkanCommandPool :public DeviceObjectBase
	{
	private:
		VkCommandPool m_command_pool{ nullptr };
		std::shared_ptr<VulkanQueue> m_graphic_queue_ptr;
	public:
		~VulkanCommandPool();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_graphic_queue_ptr->GetDevicePtr(); }
		static std::shared_ptr<VulkanCommandPool> CreateCommandPool(const std::shared_ptr<VulkanQueue> graphicQueue, VkCommandPoolCreateFlags flag);
	};
}

#endif
