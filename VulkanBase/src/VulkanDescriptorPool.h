#ifndef VULKAN_DESCRIPTOR_POOL_H
#define VULKAN_DESCRIPTOR_POOL_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanDescriptorPool :public DeviceObjectBase
	{
	private:
		std::shared_ptr<VulkanDevice> mDevice{ nullptr };
		VkDescriptorPool mDescriptorPool{ nullptr };
	public:
		static std::shared_ptr<VulkanDescriptorPool> Create(const std::shared_ptr<VulkanDevice>& device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);

		~VulkanDescriptorPool();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		VkDescriptorPool GetHandle() const { return mDescriptorPool; }
	};
}

#endif