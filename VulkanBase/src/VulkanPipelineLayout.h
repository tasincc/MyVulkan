#ifndef VULKAN_PIPELINE_LAYOUT_H
#define VULKAN_PIPELINE_LAYOUT_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include "VulkanDescriptorSetLayout.h"
#include <memory>
#include <vector>

namespace MVK
{
	class VulkanPipelineLayout : public DeviceObjectBase
	{
	private:
		VkPipelineLayout m_pipeline_layout;
		std::vector<std::shared_ptr<VulkanDescriptorSetLayout>> m_descriptor_layout_ptrs;
		std::shared_ptr<VulkanDevice> m_device_ptr;
	public:
		~VulkanPipelineLayout();
		VkPipelineLayout GetHandle() const { return m_pipeline_layout; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device_ptr; }
		static std::shared_ptr<VulkanPipelineLayout> CreatePipelineLayout(std::shared_ptr<VulkanDevice> device, std::vector<std::shared_ptr<VulkanDescriptorSetLayout>> descriptorlayouts);
	};
}

#endif // PIPELINE_LAYOUT_H
