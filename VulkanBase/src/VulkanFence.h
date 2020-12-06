#ifndef VULKAN_FENCE_H
#define VULKAN_FENCE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanFence :public DeviceObjectBase
	{
	private:
		VkFence mFence{nullptr};
		std::shared_ptr<VulkanDevice> mDevice;
	public:
		static std::shared_ptr<VulkanFence> Create(const std::shared_ptr<VulkanDevice>& device, VkFenceCreateFlags flags = 0);
		~VulkanFence();
		VkFence GetHandle() const { return mFence; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }

		VkResult Reset();
		VkResult Wait(uint32_t timeOut = UINT32_MAX);
	};
}
#endif