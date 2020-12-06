#include "VulkanImageSampler.h"

namespace MVK
{
	std::shared_ptr<VulkanImageSampler> VulkanImageSampler::Create(const std::shared_ptr<VulkanDevice>& device, VkSamplerCreateInfo& createInfo)
	{
		std::shared_ptr<VulkanImageSampler> ret = std::make_shared<VulkanImageSampler>();
		ret->mDevice = device;
		vkCreateSampler(device->GetHandle(), &createInfo, nullptr, &ret->mSampler);
		return ret;
	}

	std::shared_ptr<VulkanImageSampler> VulkanImageSampler::Create(
		const std::shared_ptr<VulkanDevice>& device,
		VkFilter                magFilter,
		VkFilter                minFilter,
		VkSamplerMipmapMode     mipmapMode,
		VkSamplerAddressMode    addressMode,
		VkBool32                anisotropyEnable,
		float                   maxAnisotropy,
		float                   maxLoad)
	{
		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = magFilter;
		createInfo.minFilter = minFilter;
		createInfo.mipmapMode = mipmapMode;
		createInfo.addressModeU = addressMode;
		createInfo.addressModeV = addressMode;
		createInfo.addressModeW = addressMode;
		createInfo.anisotropyEnable = anisotropyEnable;
		createInfo.maxAnisotropy = maxAnisotropy;
		createInfo.unnormalizedCoordinates = false;
		createInfo.compareEnable = VK_FALSE;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
		createInfo.minLod = 0.f;
		createInfo.maxLod = maxLoad;
		createInfo.mipLodBias = 0.0f;

		return Create(device, createInfo);
	}

	VulkanImageSampler::~VulkanImageSampler()
	{
		if (mSampler)
		{
			vkDestroySampler(mDevice->GetHandle(), mSampler, nullptr);
		}
	}
}