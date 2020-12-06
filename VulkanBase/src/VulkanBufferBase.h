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
		VkBuffer mBuffer{ nullptr };
		VkDeviceSize mSize{ 0 };
	public:
		VkBuffer GetHandle() const { return mBuffer; }
		VkDeviceSize GetSize() const { return mSize; }
	};
}

#endif // VULKAN_BUFFER_BASE_H