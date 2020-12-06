#ifndef VULKAN_DESCRIPTOR_SET_LAYOUT_H
#define VULKAN_DESCRIPTOR_SET_LAYOUT_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanDescriptorSetLayout :public DeviceObjectBase
	{
	private:
		VkDescriptorSetLayout mDescriptorSetLayout{ nullptr };
		std::shared_ptr<VulkanDevice> mDevice;
	public:
		~VulkanDescriptorSetLayout();
		VkDescriptorSetLayout GetHandle() const { return mDescriptorSetLayout; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		static std::shared_ptr<VulkanDescriptorSetLayout> CreaeteDescriptorSetLayout(const std::shared_ptr<VulkanDevice>& device, const std::vector<VkDescriptorSetLayoutBinding>& layoutBinding);
	};
}

#endif
