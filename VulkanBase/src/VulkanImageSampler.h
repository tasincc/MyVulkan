#ifndef VULKAN_IMAGE_SAMPLER_H
#define VULKAN_IMAGE_SAMPLER_H

#include "vulkan/vulkan.h"
#include "DeviceObjectBase.h"
#include <memory>

namespace MVK
{
	class VulkanImageSampler :DeviceObjectBase
	{
	private:
		VkSampler mSampler{ nullptr };
		std::shared_ptr<VulkanDevice> mDevice;
	public:
		static std::shared_ptr<VulkanImageSampler> Create(const std::shared_ptr<VulkanDevice>& device,
			VkFilter                magFilter,
			VkFilter                minFilter,
			VkSamplerMipmapMode     mipmapMode,
			VkSamplerAddressMode    addressMode,
			VkBool32                anisotropyEnable,
			float                   maxAnisotropy,
			float                   maxLoad);

		static std::shared_ptr<VulkanImageSampler> Create(const std::shared_ptr<VulkanDevice>& device, VkSamplerCreateInfo& createInfo);

		~VulkanImageSampler();
		const std::shared_ptr<VulkanDevice>& GetDevicePtr() const { return mDevice; }
		VkSampler GetHandle() const { return mSampler; }
	};
}

#endif
