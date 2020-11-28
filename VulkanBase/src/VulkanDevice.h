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
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> computeFamily;
		std::optional<uint32_t> transferFamilty;
	};

	class VulkanQueue;

	class VulkanDevice
	{
	public:
		VkDevice m_logicalDevice = VK_NULL_HANDLE;
		std::shared_ptr<VulkanPhysicalDevice> m_physicalDevice_ptr{ nullptr };
		QueueFamilyIndices m_queueFamilyIndices;
		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		std::vector<VkExtensionProperties> m_SupporExtensions;
		std::shared_ptr<MVK::VulkanQueue> m_graphicQueue{ nullptr };
		std::shared_ptr<MVK::VulkanQueue> m_presentQueue{ nullptr };
		std::shared_ptr<MVK::VulkanQueue> m_computeQueue{ nullptr };
		std::shared_ptr<MVK::VulkanQueue> m_transferQueue{ nullptr };
	public:
		~VulkanDevice();
		static std::shared_ptr<VulkanDevice> createLogicDevice(std::shared_ptr<VulkanPhysicalDevice> physicalDevice, std::shared_ptr<Surface> surface, VkPhysicalDeviceFeatures enableFeatures, std::vector<const char*> enableExtensions, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT);
		uint32_t findDeviceQueue(std::shared_ptr<Surface> surface, VkQueueFlagBits requestedQueueTypes) const;
		bool isExtensionSupport(const char* extension) const;
		VkDevice GetHandle() const { return m_logicalDevice; }
		const std::shared_ptr<VulkanQueue> GetGraphicQueue() const { return m_graphicQueue; }
		const std::shared_ptr<VulkanQueue> GetPresentQueue() const { return m_presentQueue; }
		std::vector<uint32_t> GetQueueIndex() const;
		const std::shared_ptr<VulkanPhysicalDevice> GetPhysicalDevicePtr() const { return m_physicalDevice_ptr; }
	};

}

#endif // !VULKANDEVICE_H
