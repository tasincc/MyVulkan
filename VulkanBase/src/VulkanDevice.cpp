#include "VulkanDevice.h"
#include "Log.h"
#include "VulkanQueue.h"

namespace MVK
{

	VulkanDevice::~VulkanDevice()
	{
		//vkDestroyCommandPool(m_logicalDevice, m_commondPool, nullptr);
		vkDestroyDevice(m_logicalDevice, nullptr);
	}

	uint32_t VulkanDevice::findDeviceQueue(std::shared_ptr<Surface> surface, VkQueueFlagBits requestedQueueTypes) const
	{
		if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(m_QueueFamilyProperties.size()); i++)
			{
				if ((m_QueueFamilyProperties[i].queueFlags & requestedQueueTypes) && ((m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
				{
					return i;
				}
			}
		}


		if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(m_QueueFamilyProperties.size()); i++)
			{
				if ((m_QueueFamilyProperties[i].queueFlags & requestedQueueTypes) && ((m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
				{
					return i;
				}
			}
		}

		if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(m_QueueFamilyProperties.size()); i++)
			{
				if (m_QueueFamilyProperties[i].queueFlags & requestedQueueTypes)
				{
					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice_ptr->GetHandle(), i, surface->GetHandle(), &presentSupport);
					if (presentSupport)
					{
						return i;
					}
				}
			}
		}

		for (uint32_t i = 0; i < static_cast<uint32_t>(m_QueueFamilyProperties.size()); i++)
		{
			if (m_QueueFamilyProperties[i].queueFlags & requestedQueueTypes)
			{
				return i;
			}
		}
	}

	std::shared_ptr<VulkanDevice>  VulkanDevice::createLogicDevice(std::shared_ptr<VulkanPhysicalDevice> physicalDevice, std::shared_ptr<Surface> surface, VkPhysicalDeviceFeatures enableFeatures, std::vector<const char*> enableExtensions, VkQueueFlags requestedQueueTypes)
	{
		std::shared_ptr<VulkanDevice> ret = std::make_shared<VulkanDevice>();
		ret->m_physicalDevice_ptr = physicalDevice;

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetHandle(), &queueFamilyCount, nullptr);
		ret->m_QueueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetHandle(), &queueFamilyCount, ret->m_QueueFamilyProperties.data());

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice->GetHandle(), nullptr, &extensionCount, nullptr);
		ret->m_SupporExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice->GetHandle(), nullptr, &extensionCount, ret->m_SupporExtensions.data());

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		const float defaultQueuePriority(0.0f);
		if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
		{
			ret->m_queueFamilyIndices.graphicsFamily = ret->findDeviceQueue(surface,VK_QUEUE_GRAPHICS_BIT);
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = ret->m_queueFamilyIndices.graphicsFamily.value();
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(createInfo);
		}
		else
		{
			//ret->m_queueFamilyIndices.graphicsFamily = VK_NULL_HANDLE;
		}

		if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			ret->m_queueFamilyIndices.computeFamily = ret->findDeviceQueue(surface,VK_QUEUE_COMPUTE_BIT);
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = 1;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(createInfo);
		}
		else
		{
			//ret->m_queueFamilyIndices.computeFamily = ret->m_queueFamilyIndices.graphicsFamily;
		}

		if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			ret->m_queueFamilyIndices.transferFamilty = ret->findDeviceQueue(surface,VK_QUEUE_TRANSFER_BIT);
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = 1;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(createInfo);
		}
		else
		{
			//ret->m_queueFamilyIndices.transferFamilty = ret->m_queueFamilyIndices.graphicsFamily;
		}

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		deviceCreateInfo.pEnabledFeatures = &enableFeatures;

		std::vector<const char*> deviceExtensions(enableExtensions);
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		if (deviceExtensions.size() > 0)
		{
			for (const char* extension : deviceExtensions)
			{
				if (!ret->isExtensionSupport(extension))
				{
					CORE_ERROR("extension: {0} is not support", extension);
					exit(EXIT_FAILURE);
				}
			}

			deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
			deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		}
		//if () {
		//	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		//	createInfo.ppEnabledLayerNames = validationLayers.data();
		//}
		//deviceCreateInfo.enabledLayerCount = 0;

		if (vkCreateDevice(ret->m_physicalDevice_ptr->GetHandle(), &deviceCreateInfo, nullptr, &ret->m_logicalDevice) != VK_SUCCESS)
		{
			CORE_ERROR("failed to create logical device");
			exit(EXIT_FAILURE);
		}
		if (ret->m_queueFamilyIndices.graphicsFamily.has_value())
		{
			ret->m_graphicQueue = MVK::VulkanQueue::createVulkanQueue(ret, ret->m_queueFamilyIndices.graphicsFamily.value());
		}
		if (ret->m_queueFamilyIndices.presentFamily.has_value())
		{
			ret->m_presentQueue = MVK::VulkanQueue::createVulkanQueue(ret, ret->m_queueFamilyIndices.presentFamily.value());
		}
		if (ret->m_queueFamilyIndices.computeFamily.has_value())
		{
			ret->m_computeQueue = MVK::VulkanQueue::createVulkanQueue(ret, ret->m_queueFamilyIndices.computeFamily.value());
		}
		if (ret->m_queueFamilyIndices.transferFamilty.has_value())
		{
			ret->m_transferQueue = MVK::VulkanQueue::createVulkanQueue(ret, ret->m_queueFamilyIndices.transferFamilty.value());
		}
		return ret;
	}

	bool VulkanDevice::isExtensionSupport(const char* extension) const
	{
		//return std::find(m_SupporExtensions.begin(), m_SupporExtensions.end(), extension) != m_SupporExtensions.end();
		return true;
	}

	std::vector<uint32_t> VulkanDevice::GetQueueIndex() const
	{
		std::vector<uint32_t> ret;
		if (m_queueFamilyIndices.graphicsFamily.has_value())
		{
			ret.push_back(m_queueFamilyIndices.graphicsFamily.value());
		}

		if (m_queueFamilyIndices.presentFamily.has_value() && m_queueFamilyIndices.presentFamily != m_queueFamilyIndices.graphicsFamily.value())
		{
			ret.push_back(m_queueFamilyIndices.presentFamily.value());
		}

		if (m_queueFamilyIndices.computeFamily.has_value() && m_queueFamilyIndices.presentFamily != m_queueFamilyIndices.graphicsFamily.value())
		{
			ret.push_back(m_queueFamilyIndices.computeFamily.value());
		}

		if (m_queueFamilyIndices.transferFamilty.has_value() && m_queueFamilyIndices.presentFamily != m_queueFamilyIndices.computeFamily.value() && m_queueFamilyIndices.presentFamily != m_queueFamilyIndices.graphicsFamily.value())
		{
			ret.push_back(m_queueFamilyIndices.transferFamilty.value());
		}

		return ret;
	}
}
