#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include "VulkanDevice.h"
#include <memory>

namespace MVK
{
	class ShaderModule :public DeviceObjectBase
	{
	private:
		VkShaderModule mShaderModule{ nullptr };
		VkShaderStageFlags mShaderStage;
		std::shared_ptr<VulkanDevice> mDevice{ nullptr };
	public:
		~ShaderModule();
		VkShaderModule& GetHandle() { return mShaderModule; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		static std::shared_ptr<ShaderModule> Create(const std::shared_ptr<VulkanDevice> device, const char* path, VkShaderStageFlags m_shader_stage);
	};
}

#endif
