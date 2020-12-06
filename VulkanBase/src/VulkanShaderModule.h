#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include "VulkanDevice.h"
#include <memory>

namespace MVK
{
	class VulkanShaderModule :public DeviceObjectBase
	{
	private:
		VkShaderModule mShaderModule;
		VkShaderStageFlagBits mShaderStage;
		std::shared_ptr<VulkanDevice> mDevice{ nullptr };
	public:
		~VulkanShaderModule();
		VkShaderModule& GetHandle() { return mShaderModule; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(const char* name = "main") const;
		static std::shared_ptr<VulkanShaderModule> Create(const std::shared_ptr<VulkanDevice> device, const char* path, VkShaderStageFlagBits m_shader_stage);
	};
}

#endif
