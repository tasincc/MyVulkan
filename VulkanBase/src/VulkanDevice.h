#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

#include <optional>
#include <vector>
#include <memory>
#include "vulkan/vulkan.h"
#include "VulkanPhysicalDevice.h"
#include "Surface.h"

namespace MVK
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> computeFamily;
		std::optional<uint32_t> transferFamilty;
	};

	class VulkanDevice
	{
	private:
		VkDevice mLogicalDevice = VK_NULL_HANDLE;
		std::shared_ptr<VulkanPhysicalDevice> mPhysicalDevicePtr{ nullptr };
		QueueFamilyIndices mQueueFamilyIndices;
		std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
		std::vector<VkExtensionProperties> mSupporExtensions;
	public:
		~VulkanDevice();
		VkDevice GetHandle() const { return mLogicalDevice; }
		const std::shared_ptr<VulkanPhysicalDevice> GetPhysicalDevicePtr() const { return mPhysicalDevicePtr; }
		const QueueFamilyIndices& GetQueueFamilyIndices() const { return mQueueFamilyIndices; }
		std::vector<uint32_t> GetQueueFamilyIndicesArray() const;
		static std::shared_ptr<VulkanDevice> Create(std::shared_ptr<VulkanPhysicalDevice> physicalDevice, std::shared_ptr<Surface> surface, VkPhysicalDeviceFeatures enableFeatures, std::vector<const char*> enableExtensions, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT);
	private:
		uint32_t findDeviceQueue(std::shared_ptr<Surface> surface, VkQueueFlagBits requestedQueueTypes) const;
		bool isExtensionSupport(const char* extension) const;
	};

}

#endif // !VULKANDEVICE_H
