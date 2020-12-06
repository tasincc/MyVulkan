#ifndef INSTANCE_H
#define INSTANCE_H

#include "vulkan/vulkan.h"
#include <memory>

namespace MVK
{
	constexpr const char* kValidationLayers[] = { "VK_LAYER_KHRONOS_validation" };

	class VulkanInstance
	{
	private:
		VkInstance mInstance{ nullptr };
	public:
		static std::shared_ptr<VulkanInstance> Create(const char* appName, bool enableValidation = false);
		~VulkanInstance();
		VkInstance GetHandle() const { return mInstance; }
	};
}


#endif // !INSTANCE_H
