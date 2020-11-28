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
		VkPhysicalDevice m_PhysicalDevice;
		VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
		VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;
		VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures;

		void Initialize(const std::shared_ptr<VulkanInstance> instance, VkPhysicalDevice physicalDevice);
	public:
		static std::vector<std::shared_ptr<VulkanPhysicalDevice>> GetAllPhysicalDevice(const std::shared_ptr<VulkanInstance> instance);

		VkPhysicalDevice GetHandle() const { return m_PhysicalDevice; }
		const VkPhysicalDeviceProperties GetProperties() const { return m_PhysicalDeviceProperties; }
		const VkPhysicalDeviceMemoryProperties GetMemoryProperties() const { return m_PhysicalDeviceMemoryProperties; }
		const VkPhysicalDeviceFeatures GetFeatures() const { return m_PhysicalDeviceFeatures; }
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propeyties);
	};
}

#endif // !VULKANPHYSICALDEVICE_H
