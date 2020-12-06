#include "VulkanShaderModule.h"
#include <fstream>
#include "Log.h"

namespace MVK
{
	VulkanShaderModule::~VulkanShaderModule()
	{
		if (mShaderModule)
		{
			vkDestroyShaderModule(mDevice->GetHandle(), mShaderModule, nullptr);
		}
	}

	std::shared_ptr<VulkanShaderModule> VulkanShaderModule::Create(const std::shared_ptr<VulkanDevice> device, const char* path, VkShaderStageFlagBits m_shader_stage)
	{
		std::shared_ptr<VulkanShaderModule> ret = std::make_shared<VulkanShaderModule>();
		ret->mDevice = device;
		ret->mShaderStage = m_shader_stage;

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
		else
		{
			CORE_ERROR("failed to open:{}", path);
		}

		if (shaderCode)
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shaderSize;
			createInfo.pCode = reinterpret_cast<uint32_t*>(shaderCode);
			VkResult result = vkCreateShaderModule(device->GetHandle(), &createInfo, nullptr, &ret->mShaderModule);
			if (result != VK_SUCCESS)
			{
				CORE_ERROR("failed to create shader module,shader path:", path);
				exit(EXIT_FAILURE);
			}
		}
		return ret;
	}

	VkPipelineShaderStageCreateInfo VulkanShaderModule::GetShaderStageCreateInfo(const char* name /* "main" */) const
	{
		VkPipelineShaderStageCreateInfo shaderStageCI{};
		shaderStageCI.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageCI.module = mShaderModule;
		shaderStageCI.stage = mShaderStage;
		shaderStageCI.pName = name;
		return shaderStageCI;
	}
}