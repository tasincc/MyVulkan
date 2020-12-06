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
		VkPipelineLayout mPipelineLayout;
		std::vector<std::shared_ptr<VulkanDescriptorSetLayout>> mDescriptorLayoutPtrs;
		std::shared_ptr<VulkanDevice> mDevicePtr;
	public:
		~VulkanPipelineLayout();
		VkPipelineLayout GetHandle() const { return mPipelineLayout; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevicePtr; }
		static std::shared_ptr<VulkanPipelineLayout> Create(std::shared_ptr<VulkanDevice> device, std::vector<std::shared_ptr<VulkanDescriptorSetLayout>> descriptorlayouts);
	};
}

#endif // PIPELINE_LAYOUT_H
