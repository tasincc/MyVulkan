#include "VulkanDevice.h"
#include "Log.h"
#include "VulkanQueue.h"

namespace MVK
{

	VulkanDevice::~VulkanDevice()
	{
		vkDestroyDevice(mLogicalDevice, nullptr);
	}

	uint32_t VulkanDevice::findDeviceQueue(std::shared_ptr<Surface> surface, VkQueueFlagBits requestedQueueTypes) const
	{
		if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(mQueueFamilyProperties.size()); i++)
			{
				if ((mQueueFamilyProperties[i].queueFlags & requestedQueueTypes) && ((mQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
				{
					return i;
				}
			}
		}


		if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(mQueueFamilyProperties.size()); i++)
			{
				if ((mQueueFamilyProperties[i].queueFlags & requestedQueueTypes) && ((mQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((mQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
				{
					return i;
				}
			}
		}

		if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(mQueueFamilyProperties.size()); i++)
			{
				if (mQueueFamilyProperties[i].queueFlags & requestedQueueTypes)
				{
					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevicePtr->GetHandle(), i, surface->GetHandle(), &presentSupport);
					if (presentSupport)
					{
						return i;
					}
				}
			}
		}

		for (uint32_t i = 0; i < static_cast<uint32_t>(mQueueFamilyProperties.size()); i++)
		{
			if (mQueueFamilyProperties[i].queueFlags & requestedQueueTypes)
			{
				return i;
			}
		}
	}

	std::shared_ptr<VulkanDevice>  VulkanDevice::Create(std::shared_ptr<VulkanPhysicalDevice> physicalDevice, std::shared_ptr<Surface> surface, VkPhysicalDeviceFeatures enableFeatures, std::vector<const char*> enableExtensions, VkQueueFlags requestedQueueTypes)
	{
		std::shared_ptr<VulkanDevice> ret = std::make_shared<VulkanDevice>();
		ret->mPhysicalDevicePtr = physicalDevice;

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetHandle(), &queueFamilyCount, nullptr);
		ret->mQueueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetHandle(), &queueFamilyCount, ret->mQueueFamilyProperties.data());

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice->GetHandle(), nullptr, &extensionCount, nullptr);
		ret->mSupporExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice->GetHandle(), nullptr, &extensionCount, ret->mSupporExtensions.data());

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		const float defaultQueuePriority(0.0f);
		if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
		{
			ret->mQueueFamilyIndices.graphicsFamily = ret->findDeviceQueue(surface, VK_QUEUE_GRAPHICS_BIT);
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = ret->mQueueFamilyIndices.graphicsFamily.value();
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(createInfo);
		}

		if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			ret->mQueueFamilyIndices.computeFamily = ret->findDeviceQueue(surface, VK_QUEUE_COMPUTE_BIT);
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = 1;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(createInfo);
		}

		if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			ret->mQueueFamilyIndices.transferFamilty = ret->findDeviceQueue(surface, VK_QUEUE_TRANSFER_BIT);
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = 1;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(createInfo);
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

		if (vkCreateDevice(ret->mPhysicalDevicePtr->GetHandle(), &deviceCreateInfo, nullptr, &ret->mLogicalDevice) != VK_SUCCESS)
		{
			CORE_ERROR("failed to create logical device");
			exit(EXIT_FAILURE);
		}

		return ret;
	}

	bool VulkanDevice::isExtensionSupport(const char* extension) const
	{
		//return std::find(m_SupporExtensions.begin(), m_SupporExtensions.end(), extension) != m_SupporExtensions.end();
		return true;
	}

	std::vector<uint32_t> VulkanDevice::GetQueueFamilyIndicesArray() const
	{
		std::vector<uint32_t> ret;
		if (mQueueFamilyIndices.graphicsFamily.has_value())
		{
			ret.push_back(mQueueFamilyIndices.graphicsFamily.value());
		}

		if (mQueueFamilyIndices.computeFamily.has_value())
		{
			if(mQueueFamilyIndices.graphicsFamily.has_value())
			{
				ret.push_back(mQueueFamilyIndices.computeFamily.value());
			}
			else if (mQueueFamilyIndices.computeFamily.value() != mQueueFamilyIndices.graphicsFamily.value())
			{
				ret.push_back(mQueueFamilyIndices.computeFamily.value());
			}
		}

		if (mQueueFamilyIndices.transferFamilty.has_value())
		{
			if (!mQueueFamilyIndices.graphicsFamily.has_value())
			{
				ret.push_back(mQueueFamilyIndices.transferFamilty.value());
			}
			else if (mQueueFamilyIndices.transferFamilty.value() != mQueueFamilyIndices.graphicsFamily.value())
			{
				ret.push_back(mQueueFamilyIndices.transferFamilty.value());
			}
		}

		return ret;
	}
}
