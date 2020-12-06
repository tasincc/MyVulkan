#include "VulkanPhysicalDevice.h"
#include "Log.h"

namespace MVK
{
	std::vector<std::shared_ptr<VulkanPhysicalDevice>> VulkanPhysicalDevice::GetAllPhysicalDevice(std::shared_ptr<VulkanInstance> instance)
	{
		std::vector<std::shared_ptr<VulkanPhysicalDevice>> ret;
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> devices(deviceCount);
		if (vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, devices.data()) != VK_SUCCESS)
		{
			CORE_ERROR("ERROR::failed to enumerate device");
			exit(EXIT_FAILURE);
		}

		for (uint32_t i = 0; i < deviceCount; i++)
		{
			std::shared_ptr < VulkanPhysicalDevice> physicalDevice = std::make_shared<VulkanPhysicalDevice>();
			physicalDevice->Initialize(instance, devices[i]);
			ret.push_back(physicalDevice);
		}
		return ret;
	}

	void VulkanPhysicalDevice::Initialize(const std::shared_ptr<VulkanInstance> instance, VkPhysicalDevice physicalDevice)
	{
		mPhysicalDevice = physicalDevice;

		vkGetPhysicalDeviceProperties(physicalDevice, &mPhysicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &mPhysicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &mPhysicalDeviceMemoryProperties);
	}

	VkFormat VulkanPhysicalDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, format, &props);
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}
	}

	VkFormat VulkanPhysicalDevice::FindDepthFormat()
	{
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	uint32_t VulkanPhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propeyties)
	{
		for (uint32_t i = 0; i < mPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (mPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & propeyties) == propeyties)
			{
				return i;
			}
		}
	}
}