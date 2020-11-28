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
		VkDescriptorSetLayout m_descriptor_set_layout{ nullptr };
		std::shared_ptr<VulkanDevice> m_device;
	public:
		~VulkanDescriptorSetLayout();
		VkDescriptorSetLayout GetHandle() const { return m_descriptor_set_layout; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		static std::shared_ptr<VulkanDescriptorSetLayout> CreaeteDescriptorSetLayout(const std::shared_ptr<VulkanDevice>& device, const std::vector<VkDescriptorSetLayoutBinding>& layoutBinding);
	};
}

#endif
