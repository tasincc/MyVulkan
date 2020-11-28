#include "ShaderModule.h"
#include <fstream>
#include "Log.h"

namespace MVK
{
	ShaderModule::~ShaderModule()
	{
		if (m_shader_module)
		{
			vkDestroyShaderModule(m_device->GetHandle(), m_shader_module, nullptr);
		}
	}

	std::shared_ptr<ShaderModule> ShaderModule::CreateShaderModule(const std::shared_ptr<VulkanDevice> device, const char* path, VkShaderStageFlags m_shader_stage)
	{
		std::shared_ptr<ShaderModule> ret = std::make_shared<ShaderModule>();
		ret->m_device;
		ret->m_shader_stage = m_shader_stage;

		size_t shaderSize;
		char* shaderCode{ nullptr };

		std::ifstream is(path, std::ios::binary | std::ios::in | std::ios::ate);
		if (is.is_open())
		{
			shaderSize = is.tellg();
			is.seekg(0, std::ios::beg);
			shaderCode = new char[shaderSize];
			is.read(shaderCode, shaderSize);
			is.close();
		}

		if (shaderCode)
		{
			VkShaderModuleCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shaderSize;
			createInfo.pCode = reinterpret_cast<uint32_t*>(shaderCode);

			VkResult result = vkCreateShaderModule(device->GetHandle(), &createInfo, nullptr, &ret->GetHandle());
			if (result != VK_SUCCESS)
			{
				CORE_ERROR("failed to create shader module,shader path:", path);
				exit(EXIT_FAILURE);
			}
		}
		return ret;
	}
}