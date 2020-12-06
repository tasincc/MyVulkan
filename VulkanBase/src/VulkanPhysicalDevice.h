#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#include <vector>
#include <memory>
#include "vulkan/vulkan.h"
#include "VulkanInstance.h"

namespace MVK
{
	class VulkanPhysicalDevice
	{
	private:
		VkPhysicalDevice mPhysicalDevice;
		VkPhysicalDeviceProperties mPhysicalDeviceProperties;
		VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
		VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;

		void Initialize(const std::shared_ptr<VulkanInstance> instance, VkPhysicalDevice physicalDevice);
	public:
		static std::vector<std::shared_ptr<VulkanPhysicalDevice>> GetAllPhysicalDevice(const std::shared_ptr<VulkanInstance> instance);

		VkPhysicalDevice GetHandle() const { return mPhysicalDevice; }
		const VkPhysicalDeviceProperties GetProperties() const { return mPhysicalDeviceProperties; }
		const VkPhysicalDeviceMemoryProperties GetMemoryProperties() const { return mPhysicalDeviceMemoryProperties; }
		const VkPhysicalDeviceFeatures GetFeatures() const { return mPhysicalDeviceFeatures; }
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propeyties);
	};
}

#endif // !VULKANPHYSICALDEVICE_H
