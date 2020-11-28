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
		VkShaderModule m_shader_module{ nullptr };
		VkShaderStageFlags m_shader_stage;
		std::shared_ptr<VulkanDevice> m_device{ nullptr };
	public:
		~ShaderModule();
		VkShaderModule& GetHandle() { return m_shader_module; }
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return m_device; }
		static std::shared_ptr<ShaderModule> CreateShaderModule(const std::shared_ptr<VulkanDevice> device, const char* path, VkShaderStageFlags m_shader_stage);
	};
}

#endif
