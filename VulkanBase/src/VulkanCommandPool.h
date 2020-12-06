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
		VkCommandPool mCommandPool{ nullptr };
		std::shared_ptr<VulkanQueue> mQueuePtr;
	public:
		~VulkanCommandPool();
		static std::shared_ptr<VulkanCommandPool> Create(const std::shared_ptr<VulkanQueue>& queue, VkCommandPoolCreateFlags flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mQueuePtr->GetDevicePtr(); }
		const std::shared_ptr<VulkanQueue>& GetQueuePtr() const { return mQueuePtr; }
		VkCommandPool GetHandle() const { return mCommandPool; }
	};
}

#endif
