#ifndef VULKAN_BUFFER_BASE_H
#define VULKAN_BUFFER_BASE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanBufferBase :public DeviceObjectBase
	{
	protected:
		VkBuffer m_buffer{ nullptr };
		VkDeviceSize m_size{ 0 };
	};
}

#endif // VULKAN_BUFFER_BASE_H