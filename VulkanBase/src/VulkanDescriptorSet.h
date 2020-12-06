#ifndef VULKAN_DESCRIPTOR_SET_H
#define VULKAN_DESCRIPTOR_SET_H

#include "vulkan/vulkan.hpp"
#include "DeviceObjectBase.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include <memory>

namespace MVK
{
	class VulkanDescriptorSet :public DeviceObjectBase
	{
	private:
		VkDescriptorSet mDesciptorSet{ nullptr };
		std::shared_ptr<VulkanDescriptorPool> mDescriptorPool;
		std::shared_ptr<VulkanDescriptorSetLayout> mDescriptorSetLayout;
	public:
		static std::shared_ptr<VulkanDescriptorSet> Create(const std::shared_ptr<VulkanDescriptorPool>& descriptorPool, const std::shared_ptr<VulkanDescriptorSetLayout> descriptorSetLayout);
		~VulkanDescriptorSet();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDescriptorPool->GetDevicePtr(); }
		VkDescriptorSet GetHandle() const { return mDesciptorSet; }
	};
}
#endif